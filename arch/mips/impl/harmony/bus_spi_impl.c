#include "arch/arch_bus_spi.h"

#if defined(__32MX470F512H_HARMONY__)

#include "peripheral/spi/spi_master/plib_spi1_master.h"

// ==============================
// CALLBACK STORAGE
// ==============================

static arch_spi_cb_t spi_callbacks[2];
static uintptr_t spi_contexts[2];

// ==============================
// WRAPPERS
// ==============================

static void spi1_cb(uintptr_t context)
{
    if (spi_callbacks[0])
        spi_callbacks[0](spi_contexts[0]);
}

// ==============================
// IMPL
// ==============================

void arch_spi_impl_init(void)
{
//    if (HAS_SPI(SPI1_MASK))
//    {
//        SPI1_Initialize();
//    }
}

int arch_spi_impl_open(uint8_t id,
                       arch_spi_config_t* cfg)
{
    switch (id)
    {
#if (CONFIG_SPI_MASK & SPI1_MASK)
        case 0: // SPI1
        {
            if (!HAS_SPI(SPI1_MASK))
                return -1;

            if (cfg != NULL)
            {
                SPI_TRANSFER_SETUP setup;

                setup.clockFrequency = cfg->frequency;
                setup.clockPhase     = (cfg->mode & 0x01);
                setup.clockPolarity  = (cfg->mode & 0x02) >> 1;
                setup.dataBits       = cfg->bits;

                return SPI1_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_SPI_MASK & SPI2_MASK)
        case 1: // SPI2
        {
            if (!HAS_SPI(SPI2_MASK))
                return -1;

            if (cfg != NULL)
            {
                SPI_TRANSFER_SETUP setup;

                setup.clockFrequency = cfg->frequency;
                setup.clockPhase     = (cfg->mode & 0x01);
                setup.clockPolarity  = (cfg->mode & 0x02) >> 1;
                setup.dataBits       = cfg->bits;

                return SPI2_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

        default:
            return -1;
    }
}


int arch_spi_impl_transfer(uint8_t id,
                           void* tx,
                           size_t tx_size,
                           void* rx,
                           size_t rx_size)
{
    bool ok = false;

    switch (id)
    {
        case 0:
            ok = SPI1_WriteRead(tx, tx_size, rx, rx_size);
            break;

        default:
            return -1;
    }

    return ok ? 0 : -1;
}

int arch_spi_impl_set_callback(uint8_t id,
                               arch_spi_cb_t cb,
                               uintptr_t context)
{
    spi_callbacks[id] = cb;
    spi_contexts[id] = context;

    switch (id)
    {
        case 0:
            SPI1_CallbackRegister(spi1_cb, 0);
            break;

        default:
            return -1;
    }

    return 0;
}

#endif
