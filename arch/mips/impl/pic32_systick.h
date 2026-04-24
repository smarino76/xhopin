/**
 * @file pic32_systick.h
 * @author Santiago Marino
 * @year 2026
 * @brief MIPS PIC32 concrete definition for system tick timer.
 * 
 * Uses the Core Timer (CP0 registers $9 = Count, $11 = Compare).
 * Counter increments at CPU_HZ / 2.
 */

#ifndef PIC32_SYSTICK_H
#define PIC32_SYSTICK_H

#include <stdint.h>

#if defined(__32MX470F512H__HARMONY__)
#include "../pic32mx470f512h.h"
#endif

#if defined(__32MZ2048EFM144__HARMONY__)
#include "../pic32mz2048efm144.h"
#endif



/* ============================================================================
 * Core Timer frequency (MIPS specific)
 * ============================================================================
 * The Core Timer increments at half the CPU clock speed.
 * ============================================================================ */
#define CORE_TIMER_HZ (CPU_HZ / 2)

/**
 * @brief Concrete definition of ArchSysTick for MIPS PIC32.
 */
struct ArchSysTick {
    uint32_t counter;      /* Last read value from CP0 $9 */
    uint32_t compare;      /* Last read value from CP0 $11 */
    uint32_t period;       /* Current period in cycles */
    uint32_t frequency;    /* CORE_TIMER_HZ */
    int initialized;       /* Initialization flag */
};

#endif /* PIC32_SYSTICK_H */
