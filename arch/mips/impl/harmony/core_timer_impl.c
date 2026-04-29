#include "arch/arch_core_timer.h"
#include "arch/include/arch_systick.h"

/* ============================================================================
 * PIC32MX470F512H Support (Harmony)
 * ============================================================================
 */
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
    /* Non usato - il callback ? gestito da arch_systick */
}

void arch_core_timer_impl_enable_interrupt(void)
{
    ArchSysTick *systick = arch_systick_get_instance();
    arch_systick_enable_interrupt(systick);
}

#endif /* defined(__32MX470F512H_HARMONY__) */


/* ============================================================================
 * PIC32MZ2048EFM144 Support (Harmony)
 * ============================================================================
 * NOTE IMPORTANTI PER IL SUPPORTO PIC32MZ2048EFM144
 * ============================================================================
 *
 * 1. SISTEMA DI TIMER:
 *    Il PIC32MZ ha un core timer (Count/Compare) che funziona in modo
 *    simile al PIC32MX. La differenza principale ? che pu? operare fino
 *    a 252 MHz (vs 80 MHz del PIC32MX).
 *
 * 2. PERIODO DEL TIMER:
 *    Il valore period_cycles deve essere calcolato in base alla CPU_HZ.
 *    Esempio: per un interrupt ogni 1ms a 200 MHz:
 *    period_cycles = CPU_HZ / 1000 = 200000000 / 1000 = 200000
 *
 * 3. FUNZIONI HARMONY:
 *    Le funzioni del core timer sono accessibili tramite <xc.h> e i
 *    register SI sono identici tra PIC32MX e PIC32MZ:
 *    - CP0?UNT: Registro Count (R/O) - legge il contatore
 *    - CP0 COMPARE: Registro Compare (R/W) - imposta il valore di confronto
 *    - CP0?UNT: Registro Count (R/O) - legge il contatore (stesso registro)
 *
 * 4. ASTRAZIONE:
 *    L'implementazione ? identica al PIC32MX, il che dimostra che l'interfaccia
 *    del core timer ? standardizzata tra le due famiglie. Ci? permette al kernel
 *    di essere completamente indipendente dall'hardware sottostante.
 *
 * ============================================================================
 */
#if defined(__PIC32MZ2048EFM144_HARMONY__) || defined(__32MZ2048EFM144_HARMONY__)
#include <xc.h>

void arch_core_timer_impl_init(void)
{
    ArchSysTick *systick = arch_systick_get_instance();
    
    /*
     * Il periodo deve essere calcolato in base alla frequenza della CPU.
     * A 200 MHz, per un interrupt ogni 1ms: 200000000 / 1000 = 200000
     * A 252 MHz, per un interrupt ogni 1ms: 252000000 / 1000 = 252000
     * 
     * Il valore di default (400000) a 200 MHz d? un interrupt ogni 2ms.
     */
    uint32_t period_cycles = 400000;  // TODO: Calcolare in base a CPU_HZ
    arch_systick_init(systick, period_cycles);
}

void arch_core_timer_impl_set_callback(arch_core_timer_cb_t callback, uintptr_t context)
{
    /* Non usato - il callback ? gestito da arch_systick */
}

void arch_core_timer_impl_enable_interrupt(void)
{
    ArchSysTick *systick = arch_systick_get_instance();
    arch_systick_enable_interrupt(systick);
}

#endif /* defined(__PIC32MZ2048EFM144_HARMONY__) || defined(__32MZ2048EFM144_HARMONY__) */