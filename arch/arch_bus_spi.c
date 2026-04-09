#include "arch_bus_spi.h"

extern void arch_spi_impl_init(void);
extern int arch_spi_impl_open(uint8_t, arch_spi_config_t*);
extern int arch_spi_impl_transfer(uint8_t, void*, size_t, void*, size_t);
extern int arch_spi_impl_set_callback(uint8_t, arch_spi_cb_t, uintptr_t);

// INIT
int arch_spi_init(void)
{
    if (!arch_has_spi())
        return -1;

    arch_spi_impl_init();
    return 0;
}

// OPEN
int arch_spi_open(uint8_t id, arch_spi_config_t* cfg)
{
    if (!arch_has_spi())
        return -1;

    if (cfg && cfg->bs.mode != ARCH_BUS_MODE_MASTER)
        return -1;

    return arch_spi_impl_open(id, cfg);
}

// TRANSFER
int arch_spi_transfer(uint8_t id,
                      void* tx,
                      size_t tx_size,
                      void* rx,
                      size_t rx_size)
{
    return arch_spi_impl_transfer(id, tx, tx_size, rx, rx_size);
}

// CALLBACK
int arch_spi_set_callback(uint8_t id,
                          arch_spi_cb_t cb,
                          uintptr_t ctx)
{
    return arch_spi_impl_set_callback(id, cb, ctx);
}