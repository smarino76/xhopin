#include "impl/pic32mx_cpu.h"
#include "impl/pic32mx_systick.h"

#ifndef PIC32MX470F512H_H
#define PIC32MX470F512H_H

/* ============================================================================
 * CPU frequency - MUST match your board's actual clock speed
 * ============================================================================
 * PIC32MX470F512H typical configurations:
 *   - 80 MHz: 8MHz external crystal + PLL (default)
 *   - 48 MHz: USB ready
 *   - 72 MHz: Common alternative
 * 
 * Change this according to your hardware configuration.
 * ============================================================================
 */
#ifndef CPU_HZ
#define CPU_HZ 80000000UL   /* 80 MHz - change if your board is different */
#endif




#define MEM_RAM_SIZE    131072
#define ARCH_RAMDISK_BLOCK_SIZE      32
#define ARCH_RAMDISK_BLOCK_COUNT     1024
//#define ARCH_TASK_STACK_SIZE 256
#define ARCH_MAX_STORAGE_DEVICES  4 /* for storage drivers*/
#define CPUs    1   // number of cpus
#define KERNEL_STACK    256
#define MAX_KERNEL_DEVICES 32



#define CAPS_SPI            (1u << 0)
#define CAPS_ADC            (1u << 1)
#define CAPS_I2C            (1u << 2)
#define CAPS_CMP            (1u << 3)
#define CAPS_CVR            (1u << 4)
#define CAPS_ICAP           (1u << 5)
#define CAPS_NVM            (1u << 6)
#define CAPS_OCMP           (1u << 7)
#define CAPS_PMP            (1u << 8)
#define CAPS_POWER          (1u << 9)
#define CAPS_RAM            (1u << 10)
#define CAPS_RCON           (1u << 11)
#define CAPS_RTCC           (1u << 12)
#define CAPS_TMR            (1u << 13)
#define CAPS_CORE_TIMER     (1u << 14)
#define CAPS_UART           (1u << 15)

// MACHINE CAPABILITIES
#define MACHINE_CAPS    (CAPS_SPI | CAPS_ADC | CAPS_I2C | CAPS_CMP | CAPS_CVR | CAPS_ICAP | CAPS_NVM | CAPS_OCMP | CAPS_PMP | CAPS_POWER | CAPS_RAM | CAPS_RCON | CAPS_RTCC | CAPS_TMR | CAPS_CORE_TIMER | CAPS_UART)

// MASk SPI
#define SPI1_MASK   (1u << 0)
#define SPI2_MASK   (1u << 1)
#define MACHINE_SPI_MASK (SPI1_MASK | SPI2_MASK)

// MASK I2C
#define I2C1_MASK   (1u << 0)
#define I2C2_MASK   (1u << 1)
#define MACHINE_I2C_MASK (I2C1_MASK | I2C2_MASK)

// MASK TMR
#define TMR1_MASK   (1u << 0)
#define TMR2_MASK   (1u << 1)
#define MACHINE_TMR_MASK (TMR1_MASK | TMR2_MASK)

// MASK UART (aggiunta)
#define UART1_MASK   (1u << 0)
#define UART2_MASK   (1u << 1)
#define UART3_MASK   (1u << 2)
#define UART4_MASK   (1u << 3)
#define MACHINE_UART_MASK (UART1_MASK | UART2_MASK | UART3_MASK | UART4_MASK)

#define CORE_TIMER_MASK             (1u << 0)
#define MACHINE_CORE_TIMER_MASK     (CORE_TIMER_MASK)

// MACHINE CONFIGURATION
#define CONFIG_CAPS (CAPS_SPI | CAPS_I2C | CAPS_CORE_TIMER | CAPS_UART)

// INSTANCE MASK
#define CONFIG_SPI_MASK (SPI1_MASK)          // solo SPI1 attivo
#define CONFIG_I2C_MASK (I2C1_MASK)          // solo I2C1 attivo
#define CONFIG_CORE_TIMER_MASK (CORE_TIMER_MASK)

// INSTANCE MASK per UART (attiva solo UART1)
#define CONFIG_UART_MASK (UART1_MASK)

// ACTIVE
#define ACTIVE_CAPS (MACHINE_CAPS & CONFIG_CAPS)
#define ACTIVE_SPI_MASK (MACHINE_SPI_MASK & CONFIG_SPI_MASK)
#define ACTIVE_I2C_MASK (MACHINE_I2C_MASK & CONFIG_I2C_MASK)
#define ACTIVE_CORE_TIMER_MASK (MACHINE_CORE_TIMER_MASK & CONFIG_CORE_TIMER_MASK)
#define ACTIVE_UART_MASK (MACHINE_UART_MASK & CONFIG_UART_MASK)

// Macro per testare la presenza di capacità generiche
#define HAS_CAPS(caps) ((ACTIVE_CAPS & (caps)) != 0)

// Macro per testare la presenza di singole istanze
#define HAS_SPI(mask)  ((ACTIVE_SPI_MASK & (mask)) != 0)
#define HAS_I2C(mask)  ((ACTIVE_I2C_MASK & (mask)) != 0)
#define HAS_CORE_TIMER(mask) ((ACTIVE_CORE_TIMER_MASK & (mask)) != 0)
#define HAS_UART(mask) ((ACTIVE_UART_MASK & (mask)) != 0)


#endif