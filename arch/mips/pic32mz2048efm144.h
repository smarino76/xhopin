#ifndef PIC32MZ2048EFM144_H
#define PIC32MZ2048EFM144_H

// Header file specifici per l'implementazione dell'MCU.
// TODO: Creare questi file per il PIC32MZ e includerli.
// #include "impl/pic32mz_cpu.h"
// #include "impl/pic32mz_systick.h"

/* ============================================================================
 * CPU frequency - MUST match your board's actual clock speed
 * ============================================================================
 * PIC32MZ2048EFM144 typical configurations:
 *   - 200 MHz: Standard high-performance mode (default)
 *   - 252 MHz: Maximum speed operation (requires specific config)
 *   - 180 MHz: Extended temperature range operation
 *
 * Change this according to your hardware configuration.
 * ============================================================================
 */
#ifndef CPU_HZ
#define CPU_HZ 200000000UL   /* 200 MHz - change if your board is different */
#endif

// RAM: 512 KB = 524288 bytes
#define MEM_RAM_SIZE    524288
#define ARCH_RAMDISK_BLOCK_SIZE      32
#define ARCH_RAMDISK_BLOCK_COUNT     16384   // 524288 / 32 = 16384
#define ARCH_MAX_STORAGE_DEVICES     4       // for storage drivers
#define CPUs                          1       // number of cpus
#define KERNEL_STACK                 256
#define MAX_KERNEL_DEVICES           32

/* ============================================================================
 * Hardware Capabilities Flags
 * ============================================================================
 * Based on the PIC32MZ2048EFM144 features from the datasheet.
 * Many more peripherals are available, but we'll initially flag only
 * the ones supported by our kernel.
 */
#define CAPS_SPI            (1u << 0)
#define CAPS_I2C            (1u << 1)
#define CAPS_UART           (1u << 2)
// Partially supported or not initially used, but available
#define CAPS_ADC            (1u << 3)
#define CAPS_TMR            (1u << 4)
#define CAPS_CORE_TIMER     (1u << 5)
#define CAPS_RTCC           (1u << 6)
#define CAPS_POWER          (1u << 7)
#define CAPS_RCON           (1u << 8)
#define CAPS_PMP            (1u << 9)
#define CAPS_EBI            (1u << 10)
#define CAPS_SQI            (1u << 11)
#define CAPS_CAN            (1u << 12)
#define CAPS_ETHERNET       (1u << 13)
#define CAPS_USB            (1u << 14)
#define CAPS_CRYPTO         (1u << 15)
#define CAPS_CMP            (1u << 16)
#define CAPS_CVR            (1u << 17)
#define CAPS_ICAP           (1u << 18)   // Input Capture
#define CAPS_OCMP           (1u << 19)   // Output Compare
#define CAPS_NVM            (1u << 20)   // Non-Volatile Memory control
#define CAPS_DMA            (1u << 21)
#define CAPS_RAM            (1u << 22)   // RAM Memory

// Total Machine Capabilities (list of all available hardware)
#define MACHINE_CAPS    (CAPS_SPI | CAPS_I2C | CAPS_UART | CAPS_ADC | CAPS_TMR | \
                         CAPS_CORE_TIMER | CAPS_RTCC | CAPS_POWER | CAPS_RCON | \
                         CAPS_PMP | CAPS_EBI | CAPS_SQI | CAPS_CAN | \
                         CAPS_ETHERNET | CAPS_USB | CAPS_CRYPTO | CAPS_CMP | \
                         CAPS_CVR | CAPS_ICAP | CAPS_OCMP | CAPS_NVM | CAPS_DMA | \
                         CAPS_RAM)

// ============================================================================
// Peripheral Instance Masks (for SPI, I2C, UART, Timers)
// ============================================================================
// The PIC32MZ2048EFM144 has up to 6 SPI, 5 I2C, and 6 UART modules.

// --- SPI Masks ---
#define SPI1_MASK   (1u << 0)
#define SPI2_MASK   (1u << 1)
#define SPI3_MASK   (1u << 2)
#define SPI4_MASK   (1u << 3)
#define SPI5_MASK   (1u << 4)
#define SPI6_MASK   (1u << 5)

#define MACHINE_SPI_MASK (SPI1_MASK | SPI2_MASK | SPI3_MASK | SPI4_MASK | SPI5_MASK | SPI6_MASK)

// --- I2C Masks ---
#define I2C1_MASK   (1u << 0)
#define I2C2_MASK   (1u << 1)
#define I2C3_MASK   (1u << 2)
#define I2C4_MASK   (1u << 3)
#define I2C5_MASK   (1u << 4)

#define MACHINE_I2C_MASK (I2C1_MASK | I2C2_MASK | I2C3_MASK | I2C4_MASK | I2C5_MASK)

// --- Timer Masks ---
#define TMR1_MASK   (1u << 0)
#define TMR2_MASK   (1u << 1)
#define TMR3_MASK   (1u << 2)
#define TMR4_MASK   (1u << 3)
#define TMR5_MASK   (1u << 4)
#define TMR6_MASK   (1u << 5)
#define TMR7_MASK   (1u << 6)
#define TMR8_MASK   (1u << 7)
#define TMR9_MASK   (1u << 8)

#define MACHINE_TMR_MASK (TMR1_MASK | TMR2_MASK | TMR3_MASK | TMR4_MASK | TMR5_MASK | \
                          TMR6_MASK | TMR7_MASK | TMR8_MASK | TMR9_MASK)

// --- UART Masks ---
#define UART1_MASK   (1u << 0)
#define UART2_MASK   (1u << 1)
#define UART3_MASK   (1u << 2)
#define UART4_MASK   (1u << 3)
#define UART5_MASK   (1u << 4)
#define UART6_MASK   (1u << 5)

#define MACHINE_UART_MASK (UART1_MASK | UART2_MASK | UART3_MASK | UART4_MASK | UART5_MASK | UART6_MASK)

// --- Core Timer (there is only one) ---
#define CORE_TIMER_MASK (1u << 0)
#define MACHINE_CORE_TIMER_MASK (CORE_TIMER_MASK)

// ============================================================================
// Kernel Configuration (choose which peripherals to enable)
// ============================================================================
// We are activating only SPI, I2C, UART, and the Core Timer for the kernel.
#define CONFIG_CAPS (CAPS_SPI | CAPS_I2C | CAPS_UART | CAPS_CORE_TIMER)

// Instance-specific configuration
#define CONFIG_SPI_MASK (SPI1_MASK)        // Only SPI1 active
#define CONFIG_I2C_MASK (I2C1_MASK)        // Only I2C1 active
#define CONFIG_CORE_TIMER_MASK (CORE_TIMER_MASK)
#define CONFIG_UART_MASK (UART1_MASK)      // Only UART1 active

// ============================================================================
// Active Capabilities (what is both available and configured)
// ============================================================================
#define ACTIVE_CAPS (MACHINE_CAPS & CONFIG_CAPS)
#define ACTIVE_SPI_MASK (MACHINE_SPI_MASK & CONFIG_SPI_MASK)
#define ACTIVE_I2C_MASK (MACHINE_I2C_MASK & CONFIG_I2C_MASK)
#define ACTIVE_CORE_TIMER_MASK (MACHINE_CORE_TIMER_MASK & CONFIG_CORE_TIMER_MASK)
#define ACTIVE_UART_MASK (MACHINE_UART_MASK & CONFIG_UART_MASK)

// --- Helper Macros to query capabilities ---
#define HAS_CAPS(caps)        ((ACTIVE_CAPS & (caps)) != 0)
#define HAS_SPI(mask)         ((ACTIVE_SPI_MASK & (mask)) != 0)
#define HAS_I2C(mask)         ((ACTIVE_I2C_MASK & (mask)) != 0)
#define HAS_CORE_TIMER(mask)  ((ACTIVE_CORE_TIMER_MASK & (mask)) != 0)
#define HAS_UART(mask)        ((ACTIVE_UART_MASK & (mask)) != 0)

#endif // PIC32MZ2048EFM144_H
