// arch_core_timer.h
#include "arch.h"


#ifndef ARCH_CORE_TIMER_H
#define ARCH_CORE_TIMER_H

typedef void (*arch_core_timer_cb_t)(uint32_t, uintptr_t);

int arch_core_timer_init(void);
int arch_core_timer_set_callback(arch_core_timer_cb_t, uintptr_t);
void arch_core_timer_enable_interrupt(void);   /* nuova funzione per abilitare l'interrupt */


#endif
