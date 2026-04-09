#include "arch/include/arch_usart.h"

/* Dichiarazioni delle funzioni di implementazione (specifiche per arch) */
extern void arch_uart_impl_init(void);
extern int arch_uart_impl_open(uint8_t id, arch_uart_config_t *cfg);
extern int arch_uart_impl_read(uint8_t id, void *buffer, size_t size);
extern int arch_uart_impl_write(uint8_t id, const void *buffer, size_t size);
extern int arch_uart_impl_set_callback(uint8_t id, arch_uart_cb_t cb, uintptr_t ctx);

int arch_uart_init(void)
{
    if (!arch_has_uart())
        return -1;
    arch_uart_impl_init();
    return 0;
}

int arch_uart_open(uint8_t id, arch_uart_config_t *cfg)
{
    if (!arch_has_uart())
        return -1;
    if (cfg && cfg->bs.mode != ARCH_BUS_MODE_MASTER)
        return -1;
    return arch_uart_impl_open(id, cfg);
}

int arch_uart_read(uint8_t id, void *buffer, size_t size)
{
    return arch_uart_impl_read(id, buffer, size);
}

int arch_uart_write(uint8_t id, const void *buffer, size_t size)
{
    return arch_uart_impl_write(id, buffer, size);
}

int arch_uart_set_callback(uint8_t id, arch_uart_cb_t cb, uintptr_t ctx)
{
    return arch_uart_impl_set_callback(id, cb, ctx);
}