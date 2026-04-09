/**
 * @file uart_console.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementazione di una console su UART (usando arch_uart).
 */

#include "usart_console.h"
#include "arch/include/arch_usart.h"
#include "kernel/device/console/console.h"

/* --------------------------------------------------------------------------
 * Funzioni di I/O per la console UART
 * -------------------------------------------------------------------------- */
static int uart_write(console_dev_t *dev, const char *buf, size_t len) {
    (void)dev;
    return arch_uart_write(0, buf, len);  /* istanza UART 0 */
}

static int uart_read(console_dev_t *dev, char *buf, size_t max_len) {
    (void)dev;
    uint32_t i = 0;
    while (i < max_len) {
        if (arch_uart_read(0, (uint8_t*)&buf[i], 1) != 1)
            break;
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';
            break;
        }
        i++;
    }
    return i;
}

static console_ops_t uart_console_ops = {
    .write = uart_write,
    .read = uart_read
};

/* Dispositivo console statico (non allocato con mem_alloc) */
static console_dev_t uart_console = {
    .dev_base = {
        .name = "uart0",
        .class_dev = DEVICE_CLASS_CONSOLE,
        .class_ops = &uart_console_ops,
        .ctx = NULL,
        .ops = NULL,
        .allocated = 0
    },
    .ops = &uart_console_ops,
    .priv = NULL
};

void uart_console_init(void) {
    console_register(&uart_console);
}