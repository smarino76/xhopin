#include "arch/arch_core_timer.h"
#include "arch/include/arch_systick.h"

#if defined(__32MX470F512H_HARMONY__)
#include <xc.h>

void arch_core_timer_impl_init(void)
{
    ArchSysTick *systick = arch_systick_get_instance();
    uint32_t period_cycles = 400000;
    arch_systick_init(systick, period_cycles);
}

void arch_core_timer_impl_set_callback(arch_core_timer_cb_t callback, uintptr_t context)
{
    /* Non usato */
}

void arch_core_timer_impl_enable_interrupt(void)
{
    ArchSysTick *systick = arch_systick_get_instance();
    arch_systick_enable_interrupt(systick);
}

#endif