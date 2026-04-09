/**
 * @file event.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementazione del sistema di eventi asincroni.
 * 
 * MODIFICHE RISPETTO ALLA VERSIONE PRECEDENTE:
 * - Decommentato `task_yield()` nel loop del dispatcher per evitare busy?wait.
 */

#include "event.h"
#include "arch/include/arch_cpu.h"
#include "kernel/scheduler/task.h"
#include "kernel/memory/mem_alloc.h"

#define MAX_HANDLERS 8

typedef struct {
    event_handler_t handler;
    void *ctx;
} handler_entry_t;

static handler_entry_t handlers[EVENT_COUNT][MAX_HANDLERS];
static uint8_t handler_count[EVENT_COUNT] = {0};

typedef struct event_entry {
    event_type_t type;
    void *data;
    struct event_entry *next;
} event_entry_t;

static event_entry_t *queue_head = NULL;
static event_entry_t *queue_tail = NULL;
static Task *dispatcher_task = NULL;

/* ----------------------------------------------------------------------------
 * Dispatcher degli eventi (eseguito in un task kernel separato)
 * -------------------------------------------------------------------------- */
static void event_dispatcher(void *arg) {
    (void)arg;
    event_entry_t *entry = NULL;
    while (1) {
        /* Sezione critica: estrae il primo evento dalla coda */
        uint32_t flags = cpu_irq_save();
        if (queue_head) {
            entry = queue_head;
            queue_head = entry->next;
            if (!queue_head) queue_tail = NULL;
        }
        cpu_irq_restore(flags);

        if (entry) {
            /* Chiama tutti i gestori registrati per questo tipo di evento */
            for (uint8_t i = 0; i < handler_count[entry->type]; i++) {
                handlers[entry->type][i].handler(entry->type, entry->data,
                                                 handlers[entry->type][i].ctx);
            }
            mem_free(entry);
        } else {
            /* Se non ci sono eventi, cede la CPU (evita busy?wait) */
            task_yield();
        }
    }
}

void event_system_init(void) {
    dispatcher_task = task_create(event_dispatcher, NULL, 1024);
    if (!dispatcher_task) {
        while (1);
    }
}

int event_register(event_type_t event, event_handler_t handler, void *ctx) {
    if (event >= EVENT_COUNT || handler == NULL) return -1;
    if (handler_count[event] >= MAX_HANDLERS) return -1;

    uint32_t flags = cpu_irq_save();
    handlers[event][handler_count[event]].handler = handler;
    handlers[event][handler_count[event]].ctx = ctx;
    handler_count[event]++;
    cpu_irq_restore(flags);
    return 0;
}

int event_unregister(event_type_t event, event_handler_t handler, void *ctx) {
    if (event >= EVENT_COUNT) return -1;

    uint32_t flags = cpu_irq_save();
    for (uint8_t i = 0; i < handler_count[event]; i++) {
        if (handlers[event][i].handler == handler && handlers[event][i].ctx == ctx) {
            /* Compatta l'array degli handler spostando quelli successivi */
            for (uint8_t j = i; j < handler_count[event] - 1; j++)
                handlers[event][j] = handlers[event][j+1];
            handler_count[event]--;
            cpu_irq_restore(flags);
            return 0;
        }
    }
    cpu_irq_restore(flags);
    return -1;
}

void event_trigger(event_type_t event, void *data) {
    if (event >= EVENT_COUNT) return;

    event_entry_t *entry = mem_alloc(sizeof(event_entry_t));
    if (!entry) return;

    entry->type = event;
    entry->data = data;
    entry->next = NULL;

    uint32_t flags = cpu_irq_save();
    if (queue_tail) {
        queue_tail->next = entry;
    } else {
        queue_head = entry;
    }
    queue_tail = entry;
    cpu_irq_restore(flags);
}