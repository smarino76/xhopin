/**
 * @file task_exit.c
 * @author Santiago Marino
 * @year 2026
 * @brief Gestione della terminazione di un task.
 * 
 * Quando un task termina (es. la funzione entry ritorna), viene chiamato
 * task_exit_handler. Questo distrugge il task corrente (rilasciando il riferimento
 * alla VM tramite vm_ref_dec) e poi mette la CPU in idle in attesa del prossimo tick.
 */

#include "task_exit.h"
#include "arch/include/arch_cpu.h"
#include "kernel/scheduler/task.h"

void task_exit_handler(void) {
    /* Distrugge il task corrente (libera stack, task struct, e rilascia VM) */
    task_destroy_current();

    /* Mette la CPU in idle; il prossimo tick del core timer eseguirà lo scheduler */
    while (1) {
        cpu_idle();
    }
}