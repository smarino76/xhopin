#ifndef ARCH_UART_H
#define ARCH_UART_H

#include "../arch.h"
#include "../arch_bus.h"

/* Callback per UART */
typedef void (*arch_uart_cb_t)(uintptr_t);

/* Configurazione seriale (baud, parità, data, stop) */
typedef struct {
    arch_bus_config_t bs;   /* campo comune (modalità master/slave) */
    uint32_t baudRate;
    uint8_t parity;         /* 0 = none, 1 = even, 2 = odd */
    uint8_t dataBits;       /* 8 o 9 */
    uint8_t stopBits;       /* 1 o 2 */
} arch_uart_config_t;

/* Lifecycle */
int arch_uart_init(void);
int arch_uart_open(uint8_t id, arch_uart_config_t *cfg);

/* Operazioni */
int arch_uart_read(uint8_t id, void *buffer, size_t size);
int arch_uart_write(uint8_t id, const void *buffer, size_t size);

/* Callback (interrupt?driven) */
int arch_uart_set_callback(uint8_t id, arch_uart_cb_t cb, uintptr_t ctx);

#endif /* ARCH_UART_H */