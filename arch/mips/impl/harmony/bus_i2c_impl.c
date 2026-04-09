#include "arch/arch_bus_i2c.h"

#if defined(__32MX470F512H_HARMONY__)

#include "peripheral/i2c/master/plib_i2c1_master.h"
// aggiungi i2c2 se presente

// INIT
void arch_i2c_impl_init(void)
{
    // Harmony ha già inizializzato tutto
}

// OPEN

int arch_i2c_impl_open(uint8_t id,
        arch_i2c_config_t* cfg) {
    switch (id) {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0: // I2C1
        {
            if (!HAS_I2C(I2C1_MASK))
                return -1;

            if (cfg != NULL) {
                if (cfg->bs.mode != ARCH_BUS_MODE_MASTER)
                    return -1;

                I2C_TRANSFER_SETUP setup;
                setup.clkSpeed = cfg->speed;

                return I2C1_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1: // I2C2
        {
            if (!HAS_I2C(I2C2_MASK))
                return -1;

            if (cfg != NULL)
            {
                I2C_TRANSFER_SETUP setup;
                setup.clkSpeed = cfg->speed;

                return I2C2_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

        default:
            return -1;
    }
}

// READ
int arch_i2c_impl_read(uint8_t id,
                       uint16_t addr,
                       uint8_t* data,
                       size_t len)
{
    switch (id)
    {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            return I2C1_Read(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            return I2C2_Read(addr, data, len) ? 0 : -1;
#endif

        default:
            return -1;
    }
}

// WRITE
int arch_i2c_impl_write(uint8_t id,
                        uint16_t addr,
                        uint8_t* data,
                        size_t len)
{
    switch (id)
    {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            return I2C1_Write(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            return I2C2_Write(addr, data, len) ? 0 : -1;
#endif

        default:
            return -1;
    }
}

// WRITE + READ
int arch_i2c_impl_write_read(uint8_t id,
                             uint16_t addr,
                             uint8_t* wdata,
                             size_t wlen,
                             uint8_t* rdata,
                             size_t rlen)
{
    switch (id)
    {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            return I2C1_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            return I2C2_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

        default:
            return -1;
    }
}

// CALLBACK
int arch_i2c_impl_set_callback(uint8_t id,
                               arch_i2c_cb_t cb,
                               uintptr_t ctx)
{
    switch (id)
    {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            I2C1_CallbackRegister(cb, ctx);
            return 0;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            I2C2_CallbackRegister(cb, ctx);
            return 0;
#endif

        default:
            return -1;
    }
}

#endif