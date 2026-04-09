#include "arch_bus_i2c.h"

extern void arch_i2c_impl_init(void);
extern int arch_i2c_impl_open(uint8_t, arch_i2c_config_t*);
extern int arch_i2c_impl_read(uint8_t, uint16_t, uint8_t*, size_t);
extern int arch_i2c_impl_write(uint8_t, uint16_t, uint8_t*, size_t);
extern int arch_i2c_impl_write_read(uint8_t, uint16_t, uint8_t*, size_t, uint8_t*, size_t);
extern int arch_i2c_impl_set_callback(uint8_t, arch_i2c_cb_t, uintptr_t);

// INIT
int arch_i2c_init(void)
{
    if (!arch_has_i2c())
        return -1;

    arch_i2c_impl_init();
    return 0;
}

// OPEN
int arch_i2c_open(uint8_t id, arch_i2c_config_t* cfg)
{
    if (!arch_has_i2c())
        return -1;

    if (cfg && cfg->bs.mode != ARCH_BUS_MODE_MASTER)
        return -1;

    return arch_i2c_impl_open(id, cfg);
}

// OPERATIONS
int arch_i2c_read(uint8_t id, uint16_t addr, uint8_t* data, size_t len)
{
    return arch_i2c_impl_read(id, addr, data, len);
}

int arch_i2c_write(uint8_t id, uint16_t addr, uint8_t* data, size_t len)
{
    return arch_i2c_impl_write(id, addr, data, len);
}

int arch_i2c_write_read(uint8_t id, uint16_t addr,
                        uint8_t* wdata, size_t wlen,
                        uint8_t* rdata, size_t rlen)
{
    return arch_i2c_impl_write_read(id, addr, wdata, wlen, rdata, rlen);
}

// CALLBACK
int arch_i2c_set_callback(uint8_t id, arch_i2c_cb_t cb, uintptr_t ctx)
{
    return arch_i2c_impl_set_callback(id, cb, ctx);
}