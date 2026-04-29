#include "arch/arch_bus_i2c.h"
#include "arch/arch.h"
/* ============================================================================
 * PIC32MX470F512H Support (Harmony)
 * ============================================================================
 */
#if defined(__32MX470F512H_HARMONY__)

#include "peripheral/i2c/master/plib_i2c1_master.h"
// Aggiungi I2C2 se presente (es. plib_i2c2_master.h)

// INIT
void arch_i2c_impl_init(void)
{
    // Harmony ha già inizializzato tutto
}

// OPEN
int arch_i2c_impl_open(uint8_t id, arch_i2c_config_t* cfg)
{
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
int arch_i2c_impl_read(uint8_t id, uint16_t addr, uint8_t* data, size_t len)
{
    switch (id) {
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
int arch_i2c_impl_write(uint8_t id, uint16_t addr, uint8_t* data, size_t len)
{
    switch (id) {
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
int arch_i2c_impl_write_read(uint8_t id, uint16_t addr,
                             uint8_t* wdata, size_t wlen,
                             uint8_t* rdata, size_t rlen)
{
    switch (id) {
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
int arch_i2c_impl_set_callback(uint8_t id, arch_i2c_cb_t cb, uintptr_t ctx)
{
    switch (id) {
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

#endif /* defined(__32MX470F512H_HARMONY__) */


/* ============================================================================
 * PIC32MZ2048EFM144 Support (Harmony)
 * ============================================================================
 */
#if defined(__PIC32MZ2048EFM144_HARMONY__) || defined(__32MZ2048EFM144_HARMONY__)

#include "peripheral/i2c/master/plib_i2c1_master.h"
// Aggiungi I2C2, I2C3, I2C4, I2C5 se necessario
// #include "peripheral/i2c/master/plib_i2c2_master.h"
// #include "peripheral/i2c/master/plib_i2c3_master.h"
// #include "peripheral/i2c/master/plib_i2c4_master.h"
// #include "peripheral/i2c/master/plib_i2c5_master.h"

// INIT
void arch_i2c_impl_init(void)
{
    // Harmony ha già inizializzato tutto
}

// OPEN
int arch_i2c_impl_open(uint8_t id, arch_i2c_config_t* cfg)
{
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

            if (cfg != NULL) {
                I2C_TRANSFER_SETUP setup;
                setup.clkSpeed = cfg->speed;

                return I2C2_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_I2C_MASK & I2C3_MASK)
        case 2: // I2C3
        {
            if (!HAS_I2C(I2C3_MASK))
                return -1;

            if (cfg != NULL) {
                I2C_TRANSFER_SETUP setup;
                setup.clkSpeed = cfg->speed;

                return I2C3_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_I2C_MASK & I2C4_MASK)
        case 3: // I2C4
        {
            if (!HAS_I2C(I2C4_MASK))
                return -1;

            if (cfg != NULL) {
                I2C_TRANSFER_SETUP setup;
                setup.clkSpeed = cfg->speed;

                return I2C4_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_I2C_MASK & I2C5_MASK)
        case 4: // I2C5
        {
            if (!HAS_I2C(I2C5_MASK))
                return -1;

            if (cfg != NULL) {
                I2C_TRANSFER_SETUP setup;
                setup.clkSpeed = cfg->speed;

                return I2C5_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

        default:
            return -1;
    }
}

// READ
int arch_i2c_impl_read(uint8_t id, uint16_t addr, uint8_t* data, size_t len)
{
    switch (id) {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            return I2C1_Read(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            return I2C2_Read(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C3_MASK)
        case 2:
            return I2C3_Read(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C4_MASK)
        case 3:
            return I2C4_Read(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C5_MASK)
        case 4:
            return I2C5_Read(addr, data, len) ? 0 : -1;
#endif

        default:
            return -1;
    }
}

// WRITE
int arch_i2c_impl_write(uint8_t id, uint16_t addr, uint8_t* data, size_t len)
{
    switch (id) {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            return I2C1_Write(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            return I2C2_Write(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C3_MASK)
        case 2:
            return I2C3_Write(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C4_MASK)
        case 3:
            return I2C4_Write(addr, data, len) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C5_MASK)
        case 4:
            return I2C5_Write(addr, data, len) ? 0 : -1;
#endif

        default:
            return -1;
    }
}

// WRITE + READ
int arch_i2c_impl_write_read(uint8_t id, uint16_t addr,
                             uint8_t* wdata, size_t wlen,
                             uint8_t* rdata, size_t rlen)
{
    switch (id) {
#if (CONFIG_I2C_MASK & I2C1_MASK)
        case 0:
            return I2C1_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C2_MASK)
        case 1:
            return I2C2_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C3_MASK)
        case 2:
            return I2C3_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C4_MASK)
        case 3:
            return I2C4_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

#if (CONFIG_I2C_MASK & I2C5_MASK)
        case 4:
            return I2C5_WriteRead(addr, wdata, wlen, rdata, rlen) ? 0 : -1;
#endif

        default:
            return -1;
    }
}

// CALLBACK
int arch_i2c_impl_set_callback(uint8_t id, arch_i2c_cb_t cb, uintptr_t ctx)
{
    switch (id) {
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

#if (CONFIG_I2C_MASK & I2C3_MASK)
        case 2:
            I2C3_CallbackRegister(cb, ctx);
            return 0;
#endif

#if (CONFIG_I2C_MASK & I2C4_MASK)
        case 3:
            I2C4_CallbackRegister(cb, ctx);
            return 0;
#endif

#if (CONFIG_I2C_MASK & I2C5_MASK)
        case 4:
            I2C5_CallbackRegister(cb, ctx);
            return 0;
#endif

        default:
            return -1;
    }
}

#endif /* defined(__PIC32MZ2048EFM144_HARMONY__) || defined(__32MZ2048EFM144_HARMONY__) */