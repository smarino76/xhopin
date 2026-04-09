#include "arch/arch_core_timer.h"

#if defined(__32MX470F512H_HARMONY__)
#include <xc.h>               /* definisce i registri CP0 e i nomi simbolici */

#include "peripheral/coretimer/plib_coretimer.h"

void arch_core_timer_impl_init(void) {
    CORETIMER_Initialize();
    /* Imposta il periodo: scrivi 0xFA00 nel registro Compare (CP0 $11) */


    /* (Opzionale) Azzera il contatore (CP0 $9) */
    __asm__ volatile (
        "mtc0 $zero, $9\n"
    
    
    );   
    
//    __asm__ volatile (
//        "mtc0 $zero, $12, 1\n"    /*SRSCtl (CP0 register 12, select 1)*/
//        "mtc0 $zero, $12, 2\n"   /* # SRSMap (CP0 register 12, select 2)*/
//    );
    
    CORETIMER_Start();
        __asm__ volatile (
        "li   $t0, 0xFFA00\n"
        "mtc0 $t0, $11\n"
        : : : "t0"
    );
}

void arch_core_timer_impl_set_callback(arch_core_timer_cb_t callback, uintptr_t context) {
//    CORETIMER_CallbackSet(callback, context);
}


/* Abilita l'interrupt del core timer (senza usare Harmony) */
void arch_core_timer_impl_enable_interrupt(void) {
    /* Configura priorità 7, sottopriorità 0 */
    IPC0SET = (1 << _IPC0_CTIP_POSITION) | (0 << _IPC0_CTIS_POSITION);
    /* Pulisci flag */
    IFS0CLR = _IFS0_CTIF_MASK;
    /* Abilita interrupt */
    IEC0SET = _IEC0_CTIE_MASK;
        /* Azzera il contatore Count (CP0 $9) per partire da zero */
    __asm__ volatile (
        "mtc0 $zero, $9\n"
    );
}

#endif