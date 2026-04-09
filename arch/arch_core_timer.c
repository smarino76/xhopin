#include "arch_core_timer.h"


extern void arch_core_timer_impl_set_callback(arch_core_timer_cb_t, uintptr_t);
extern void arch_core_timer_impl_init(void);
extern void arch_core_timer_impl_enable_interrupt(void);


int arch_core_timer_init() {
    arch_core_timer_impl_init();
}

int arch_core_timer_set_callback(arch_core_timer_cb_t cb, uintptr_t ptr){
    
    if(!arch_has_core_timer())
        return -1;
    
    arch_core_timer_impl_set_callback(cb, ptr);
    return 0;
}


void arch_core_timer_enable_interrupt(void) {
    arch_core_timer_impl_enable_interrupt();
}