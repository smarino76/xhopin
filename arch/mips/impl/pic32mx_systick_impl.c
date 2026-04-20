/**
 * @file pic32mx_systick_impl.c
 * @author Santiago Marino
 * @year 2026
 * @brief MIPS PIC32MX implementation of system tick timer.
 */

#include "../../include/arch_systick.h"
#include "pic32mx_systick.h"
#include <xc.h>
#include <stdint.h>

/* ============================================================================
 * Singleton instance
 * ============================================================================ */
static struct ArchSysTick systick_instance = {0};

/* ============================================================================
 * Helper: read Core Timer Count (CP0 $9)
 * ============================================================================ */
static inline uint32_t core_timer_read_count(void)
{
    uint32_t count;
    __asm__ volatile (
        "mfc0 %0, $9\n"
        "ehb\n"
        : "=r" (count)
    );
    return count;
}

/* ============================================================================
 * Helper: read Core Timer Compare (CP0 $11)
 * ============================================================================ */
static inline uint32_t core_timer_read_compare(void)
{
    uint32_t compare;
    __asm__ volatile (
        "mfc0 %0, $11\n"
        "ehb\n"
        : "=r" (compare)
    );
    return compare;
}

/* ============================================================================
 * Helper: write Core Timer Compare (CP0 $11)
 * ============================================================================ */
static inline void core_timer_write_compare(uint32_t value)
{
    __asm__ volatile (
        "mtc0 %0, $11\n"
        "ehb\n"
        : : "r" (value)
    );
}

/* ============================================================================
 * Public API implementation
 * ============================================================================ */

ArchSysTick* arch_systick_get_instance(void)
{
    return &systick_instance;
}

uint32_t arch_systick_read(const ArchSysTick *tick)
{
    (void)tick;
    return core_timer_read_count();
}

uint32_t arch_systick_get_period(const ArchSysTick *tick)
{
    (void)tick;
    return core_timer_read_compare();
}

uint32_t arch_systick_get_frequency(const ArchSysTick *tick)
{
    if (!tick) return 0;
    return tick->frequency;
}

int arch_systick_init(ArchSysTick *tick, uint32_t period_cycles)
{
    if (!tick) return -1;
    
    tick->frequency = CORE_TIMER_HZ;
    tick->period = period_cycles;
    
    /* Set Compare register - DO NOT reset Count */
    core_timer_write_compare(period_cycles);
    
    tick->counter = 0;
    tick->compare = period_cycles;
    tick->initialized = 1;
    
    return 0;
}

void arch_systick_set_period(ArchSysTick *tick, uint32_t period_cycles)
{
    if (!tick) return;
    
    tick->period = period_cycles;
    
    uint32_t current_compare = core_timer_read_compare();
    core_timer_write_compare(current_compare + period_cycles);
    tick->compare = current_compare + period_cycles;
}

void arch_systick_enable_interrupt(ArchSysTick *tick)
{
    if (!tick) return;
    
    IPC0SET = (1 << _IPC0_CTIP_POSITION);
    IFS0CLR = (1 << _IFS0_CTIF_POSITION);
    IEC0SET = (1 << _IEC0_CTIE_POSITION);
}

void arch_systick_clear_interrupt(ArchSysTick *tick)
{
    if (!tick) return;
    IFS0CLR = (1 << _IFS0_CTIF_POSITION);
}

void arch_systick_start(ArchSysTick *tick)
{
    if (!tick) return;
    
    uint32_t count = core_timer_read_count();
    uint32_t compare = core_timer_read_compare();
    
    if (count >= compare) {
        core_timer_write_compare(count + tick->period);
    }
}

void arch_systick_stop(ArchSysTick *tick)
{
    if (!tick) return;
    IEC0CLR = (1 << _IEC0_CTIE_POSITION);
}

void arch_systick_tick(ArchSysTick *tick)
{
    if (!tick || !tick->initialized) return;
    
    uint32_t new_compare = core_timer_read_compare() + tick->period;
    core_timer_write_compare(new_compare);
    tick->compare = new_compare;
}