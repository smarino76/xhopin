/*
 * Vettore di interrupt per il core timer.
 * Occupa solo 8 byte, salta all'ISR vera e propria.
 */
.include "arch/mips/pic32config.inc"
    
    
#ifdef PIC32MX    
.section .vector_0, "ax", @progbits
.global _core_timer_vector
_core_timer_vector:
    j       core_timer_isr
    nop
#endif //PIC32MX
    
    
    
#ifdef PIC32MZ
.section .vector_0, "ax", @progbits
.set noreorder
.set nomips16

# ATTENZIONE: IL SIMBOLO DEVE ESSERE ESATTAMENTE QUESTO!
.global __vector_dispatch_0
.ent __vector_dispatch_0
__vector_dispatch_0:
    j     core_timer_isr
    nop
.end __vector_dispatch_0    
#endif //PIC32MZ

    