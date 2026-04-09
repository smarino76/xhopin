/**
 * @file scheduler.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementazione dello scheduler round?robin preemptive.
 * 
 * MODIFICHE RISPETTO ALLA VERSIONE PRECEDENTE:
 * - Nelle funzioni `task_destroy_current` e `task_destroy_by_id`, la chiamata
 *   diretta a `vm_destroy` è stata sostituita con `vm_ref_dec`, in modo da
 *   rilasciare il riferimento del task alla VM e permettere al reference counting
 *   di decidere quando distruggere effettivamente la VM.
 * - Aggiunto il controllo per impedire auto?terminazione in `task_destroy_by_id`.
 * - Aggiunta la notifica `EVENT_TASK_TERMINATED` in `task_destroy_by_id`.
 */

#include "task.h"
#include "arch/include/arch_cpu.h"
#include "arch/arch_core_timer.h"
#include "kernel/context/task_exit.h"
#include "kernel/memory/mem_alloc.h"
#include "kernel/event/event.h"
#include "kernel/vm/vm_core.h"
#include <stdlib.h>

extern void cpu_idle(void);

static Task *ready_list = (Task *)0x0;
Task *current_task = (Task *)0x0;
static uint32_t next_task_id = 1;
uint8_t kernel_stack[KERNEL_STACK];

/* ----------------------------------------------------------------------------
 * Inserisce un task nella lista circolare (round?robin)
 * -------------------------------------------------------------------------- */
static void insert_task(Task *task) {
    if (ready_list == NULL) {
        ready_list = task;
        task->next = task;
    } else {
        task->next = ready_list->next;
        ready_list->next = task;
    }
}

/* ----------------------------------------------------------------------------
 * Crea un nuovo task
 * -------------------------------------------------------------------------- */
Task* task_create(void (*entry)(void *), void *arg, uint32_t stack_size) {
    Task *task = (Task*)mem_alloc(sizeof(Task));
    if (!task) return NULL;

    task->stack = mem_alloc(stack_size);
    if (!task->stack) {
        mem_free(task);
        return NULL;
    }
    task->stack_size = stack_size;
    task->entry = entry;
    task->arg = arg;
    task->id = next_task_id++;
    task->priv_data = NULL;

    /* Prepara lo stack e il contesto CPU */
    void *stack_top = (uint8_t*)task->stack + stack_size;
    cpu_context_init(&task->context, entry, arg, stack_top, task_exit_handler);

    insert_task(task);
    return task;
}

/* ----------------------------------------------------------------------------
 * Distrugge il task corrente (chiamato da task_exit_handler)
 * -------------------------------------------------------------------------- */
void task_destroy_current(void) {
    if (current_task == NULL) return;

    /* Rimuove current_task dalla lista circolare */
    Task *prev = ready_list;
    while (prev->next != current_task) {
        prev = prev->next;
        if (prev == ready_list) break;
    }
    if (prev->next == current_task) {
        prev->next = current_task->next;
        if (current_task == ready_list) {
            ready_list = (current_task->next == current_task) ? NULL : current_task->next;
        }
    }

    /* Notifica l'evento di terminazione */
    event_trigger(EVENT_TASK_TERMINATED, (void*)(uintptr_t)current_task->id);

    /* Rilascia la VM (decrementa il contatore) e poi stack e task */
    if (current_task->priv_data) {
        vm_ref_dec((VMContext*)current_task->priv_data);
    }
    mem_free(current_task->stack);
    mem_free(current_task);
    current_task = NULL;
}

/* ----------------------------------------------------------------------------
 * Distrugge un task dato il suo ID (thread-safe)
 * Ritorna 0 se successo, -1 se errore (task non trovato o auto?kill).
 * -------------------------------------------------------------------------- */
int task_destroy_by_id(uint32_t id) {
    uint32_t flags = cpu_irq_save();

    /* IMPEDISCE AUTO?TERMINAZIONE: un task non può uccidere se stesso */
    if (current_task && current_task->id == id) {
        cpu_irq_restore(flags);
        return -1;
    }

    if (ready_list == NULL) {
        cpu_irq_restore(flags);
        return -1;
    }

    /* Cerca il target nella lista circolare */
    Task *prev = ready_list;
    Task *curr = ready_list->next;
    Task *target = NULL;
    Task *prev_of_target = NULL;

    if (ready_list->id == id) {
        target = ready_list;
        prev_of_target = NULL;
    } else {
        while (curr != ready_list) {
            if (curr->id == id) {
                target = curr;
                prev_of_target = prev;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    if (target == NULL) {
        cpu_irq_restore(flags);
        return -1;
    }

    /* Rimozione dalla lista */
    if (target == ready_list) {
        if (target->next == target) {
            ready_list = NULL;
        } else {
            ready_list = target->next;
        }
    }
    if (prev_of_target) {
        prev_of_target->next = target->next;
    }

    int was_current = (current_task == target);

    /* Notifica evento di terminazione */
    event_trigger(EVENT_TASK_TERMINATED, (void*)(uintptr_t)target->id);

    /* Rilascia la VM (decrementa contatore) e poi stack e task */
    if (target->priv_data) {
        vm_ref_dec((VMContext*)target->priv_data);
    }
    if (target->stack) {
        mem_free(target->stack);
    }
    mem_free(target);

    if (was_current) {
        current_task = NULL;
    }

    cpu_irq_restore(flags);
    return 0;
}

/* ----------------------------------------------------------------------------
 * Switch al prossimo task (chiamato dall'ISR del core timer)
 * -------------------------------------------------------------------------- */
static void switch_to_next(void) {
    cpu_irq_disable();
    if (ready_list == NULL) {
        cpu_irq_enable();
        return;
    }
    Task *next = ready_list->next;
    if (next == current_task) {
        cpu_irq_enable();
        return;
    }
    ready_list = next;
    current_task = next;
    if (next == NULL) {
        cpu_irq_enable();
        return;
    }
    cpu_context_restore(&current_task->context);
}

void task_yield(void) {
    /* Non usato (preemption only) */
}

void timer_callback(uint32_t count, uintptr_t param) {
    switch_to_next();
}

static void idle_task(void *arg) {
    (void)arg;
    while (1) {
        cpu_idle();
    }
}

void scheduler_init(void) {
    Task *idle = task_create(idle_task, NULL, 1024);
    if (!idle) return;
    if (ready_list == (Task *)0) {
        ready_list = idle;
        idle->next = idle;
        current_task = idle;
    } else {
        current_task = (Task *)0;
    }
    arch_core_timer_enable_interrupt();
}