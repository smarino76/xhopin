/*
 * Vettore di interrupt per il core timer.
 * Occupa solo 8 byte, salta all'ISR vera e propria.
 */
.section .vector_0, "ax", @progbits
.global _core_timer_vector
_core_timer_vector:
    j       core_timer_isr
    nop