/**
 * @file pic32mx_systick.h
 * @author Santiago Marino
 * @year 2026
 * @brief MIPS PIC32MX concrete definition for system tick timer.
 * 
 * Uses the Core Timer (CP0 registers $9 = Count, $11 = Compare).
 * Counter increments at CPU_HZ / 2.
 */

#ifndef PIC32MX_SYSTICK_H
#define PIC32MX_SYSTICK_H

#include <stdint.h>
#include "../pic32mx470f512h.h"

/* ============================================================================
 * Core Timer frequency (MIPS specific)
 * ============================================================================
 * The Core Timer increments at half the CPU clock speed.
 * ============================================================================ */
#define CORE_TIMER_HZ (CPU_HZ / 2)

/**
 * @brief Concrete definition of ArchSysTick for MIPS PIC32MX.
 */
struct ArchSysTick {
    uint32_t counter;      /* Last read value from CP0 $9 */
    uint32_t compare;      /* Last read value from CP0 $11 */
    uint32_t period;       /* Current period in cycles */
    uint32_t frequency;    /* CORE_TIMER_HZ */
    int initialized;       /* Initialization flag */
};

#endif /* PIC32MX_SYSTICK_H */