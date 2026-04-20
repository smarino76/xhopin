/**
 * @file arch_systick.h
 * @author Santiago Marino
 * @year 2026
 * @brief Abstraction for system tick timer (scheduler tick source).
 * 
 * This file provides an opaque interface to the architecture-specific
 * timer that generates periodic interrupts for the scheduler.
 */

#ifndef ARCH_SYSTICK_H
#define ARCH_SYSTICK_H

#include <stdint.h>

/**
 * @brief Opaque type - concrete definition is architecture-specific.
 */
typedef struct ArchSysTick ArchSysTick;

/**
 * @brief Get the global system tick timer instance.
 * 
 * @return Pointer to the singleton ArchSysTick instance
 */
ArchSysTick* arch_systick_get_instance(void);

/**
 * @brief Read the current counter value.
 * 
 * @param tick Pointer to ArchSysTick instance
 * @return Current counter value (in timer ticks)
 */
uint32_t arch_systick_read(const ArchSysTick *tick);

/**
 * @brief Get the current compare/period value.
 * 
 * @param tick Pointer to ArchSysTick instance
 * @return Compare/period value in ticks
 */
uint32_t arch_systick_get_period(const ArchSysTick *tick);

/**
 * @brief Get the timer frequency in Hz.
 * 
 * @param tick Pointer to ArchSysTick instance
 * @return Frequency in Hz
 */
uint32_t arch_systick_get_frequency(const ArchSysTick *tick);

/**
 * @brief Initialize the system tick timer.
 * 
 * @param tick Pointer to ArchSysTick instance
 * @param period_cycles Number of cycles between interrupts
 * @return 0 on success, -1 on error
 */
int arch_systick_init(ArchSysTick *tick, uint32_t period_cycles);

/**
 * @brief Set the timer period.
 * 
 * IMPORTANT: Increments Compare, does NOT reset Count.
 * This preserves ability to measure elapsed time across interrupts.
 * 
 * @param tick Pointer to ArchSysTick instance
 * @param period_cycles New period in cycles
 */
void arch_systick_set_period(ArchSysTick *tick, uint32_t period_cycles);

/**
 * @brief Enable the system tick interrupt.
 * 
 * @param tick Pointer to ArchSysTick instance
 */
void arch_systick_enable_interrupt(ArchSysTick *tick);

/**
 * @brief Clear the interrupt flag.
 * 
 * Must be called in the interrupt handler before returning.
 * 
 * @param tick Pointer to ArchSysTick instance
 */
void arch_systick_clear_interrupt(ArchSysTick *tick);

/**
 * @brief Start the timer.
 * 
 * @param tick Pointer to ArchSysTick instance
 */
void arch_systick_start(ArchSysTick *tick);

/**
 * @brief Stop the timer (disable interrupt).
 * 
 * @param tick Pointer to ArchSysTick instance
 */
void arch_systick_stop(ArchSysTick *tick);

/**
 * @brief Tick handler - called from ISR.
 * 
 * Increments Compare register by period instead of resetting Count.
 * This function must be called from the architecture-specific ISR.
 * 
 * @param tick Pointer to ArchSysTick instance
 */
void arch_systick_tick(ArchSysTick *tick);

#endif /* ARCH_SYSTICK_H */