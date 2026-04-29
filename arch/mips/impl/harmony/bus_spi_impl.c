#include "arch/arch_bus_spi.h"
#include "arch/arch.h"

/* ============================================================================
 * PIC32MX470F512H Support (Harmony)
 * ============================================================================
 */
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
    // Harmony ha già inizializzato tutto
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
#if (CONFIG_SPI_MASK & SPI1_MASK)
        case 0:
            ok = SPI1_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI2_MASK)
        case 1:
            ok = SPI2_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

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

#endif /* defined(__32MX470F512H_HARMONY__) */


/* ============================================================================
 * PIC32MZ2048EFM144 Support (Harmony)
 * ============================================================================
 */
#if defined(__PIC32MZ2048EFM144_HARMONY__) || defined(__32MZ2048EFM144_HARMONY__)

/*
 * ============================================================================
 * NOTE IMPORTANTI PER IL SUPPORTO PIC32MZ2048EFM144
 * ============================================================================
 *
 * 1. INCLUDE COMMENTATI:
 *    Gli include per SPI2-SPI6 sono commentati. Dovrai decommentarli e
 *    aggiungerli al tuo progetto Harmony se utilizzi piu' di SPI1.
 *
 *    Esempio per decommentare:
 *    #include "peripheral/spi/spi_master/plib_spi2_master.h"
 *    #include "peripheral/spi/spi_master/plib_spi3_master.h"
 *    #include "peripheral/spi/spi_master/plib_spi4_master.h"
 *    #include "peripheral/spi/spi_master/plib_spi5_master.h"
 *    #include "peripheral/spi/spi_master/plib_spi6_master.h"
 *
 * 2. CONFIGURAZIONE IN kconfig.h:
 *    Per abilitare SPI2-SPI6, assicurati che CONFIG_SPI_MASK includa
 *    i bit corrispondenti.
 *
 *    Esempio:
 *    #define CONFIG_SPI_MASK (SPI1_MASK | SPI2_MASK | SPI3_MASK)
 *
 * 3. CONSISTENZA DELL'ASTRAZIONE:
 *    Come per I2C, anche per SPI l'interfaccia Harmony e' identica
 *    tra PIC32MX e PIC32MZ. Questo dimostra l'eccellente lavoro di
 *    astrazione di Microchip Harmony e permette al kernel di essere
 *    completamente indipendente dall'hardware sottostante!
 *
 * ============================================================================
 */

#include "peripheral/spi/spi_master/plib_spi1_master.h"
// Decommentare per supporto multi-SPI
// #include "peripheral/spi/spi_master/plib_spi2_master.h"
// #include "peripheral/spi/spi_master/plib_spi3_master.h"
// #include "peripheral/spi/spi_master/plib_spi4_master.h"
// #include "peripheral/spi/spi_master/plib_spi5_master.h"
// #include "peripheral/spi/spi_master/plib_spi6_master.h"

// ==============================
// CALLBACK STORAGE
// Supporto fino a 6 moduli SPI (0-5)
// ==============================
#define SPI_MAX_INSTANCES 6

static arch_spi_cb_t spi_callbacks[SPI_MAX_INSTANCES];
static uintptr_t spi_contexts[SPI_MAX_INSTANCES];

// ==============================
// WRAPPERS
// ==============================

static void spi1_cb(uintptr_t context)
{
    if (spi_callbacks[0])
        spi_callbacks[0](spi_contexts[0]);
}

#if (CONFIG_SPI_MASK & SPI2_MASK)
static void spi2_cb(uintptr_t context)
{
    if (spi_callbacks[1])
        spi_callbacks[1](spi_contexts[1]);
}
#endif

#if (CONFIG_SPI_MASK & SPI3_MASK)
static void spi3_cb(uintptr_t context)
{
    if (spi_callbacks[2])
        spi_callbacks[2](spi_contexts[2]);
}
#endif

#if (CONFIG_SPI_MASK & SPI4_MASK)
static void spi4_cb(uintptr_t context)
{
    if (spi_callbacks[3])
        spi_callbacks[3](spi_contexts[3]);
}
#endif

#if (CONFIG_SPI_MASK & SPI5_MASK)
static void spi5_cb(uintptr_t context)
{
    if (spi_callbacks[4])
        spi_callbacks[4](spi_contexts[4]);
}
#endif

#if (CONFIG_SPI_MASK & SPI6_MASK)
static void spi6_cb(uintptr_t context)
{
    if (spi_callbacks[5])
        spi_callbacks[5](spi_contexts[5]);
}
#endif

// ==============================
// IMPL
// ==============================

void arch_spi_impl_init(void)
{
    // Harmony ha già inizializzato tutto
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

#if (CONFIG_SPI_MASK & SPI3_MASK)
        case 2: // SPI3
        {
            if (!HAS_SPI(SPI3_MASK))
                return -1;

            if (cfg != NULL)
            {
                SPI_TRANSFER_SETUP setup;

                setup.clockFrequency = cfg->frequency;
                setup.clockPhase     = (cfg->mode & 0x01);
                setup.clockPolarity  = (cfg->mode & 0x02) >> 1;
                setup.dataBits       = cfg->bits;

                return SPI3_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_SPI_MASK & SPI4_MASK)
        case 3: // SPI4
        {
            if (!HAS_SPI(SPI4_MASK))
                return -1;

            if (cfg != NULL)
            {
                SPI_TRANSFER_SETUP setup;

                setup.clockFrequency = cfg->frequency;
                setup.clockPhase     = (cfg->mode & 0x01);
                setup.clockPolarity  = (cfg->mode & 0x02) >> 1;
                setup.dataBits       = cfg->bits;

                return SPI4_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_SPI_MASK & SPI5_MASK)
        case 4: // SPI5
        {
            if (!HAS_SPI(SPI5_MASK))
                return -1;

            if (cfg != NULL)
            {
                SPI_TRANSFER_SETUP setup;

                setup.clockFrequency = cfg->frequency;
                setup.clockPhase     = (cfg->mode & 0x01);
                setup.clockPolarity  = (cfg->mode & 0x02) >> 1;
                setup.dataBits       = cfg->bits;

                return SPI5_TransferSetup(&setup, 0) ? 0 : -1;
            }

            return 0;
        }
#endif

#if (CONFIG_SPI_MASK & SPI6_MASK)
        case 5: // SPI6
        {
            if (!HAS_SPI(SPI6_MASK))
                return -1;

            if (cfg != NULL)
            {
                SPI_TRANSFER_SETUP setup;

                setup.clockFrequency = cfg->frequency;
                setup.clockPhase     = (cfg->mode & 0x01);
                setup.clockPolarity  = (cfg->mode & 0x02) >> 1;
                setup.dataBits       = cfg->bits;

                return SPI6_TransferSetup(&setup, 0) ? 0 : -1;
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
#if (CONFIG_SPI_MASK & SPI1_MASK)
        case 0:
            ok = SPI1_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI2_MASK)
        case 1:
            ok = SPI2_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI3_MASK)
        case 2:
            ok = SPI3_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI4_MASK)
        case 3:
            ok = SPI4_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI5_MASK)
        case 4:
            ok = SPI5_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI6_MASK)
        case 5:
            ok = SPI6_WriteRead(tx, tx_size, rx, rx_size);
            break;
#endif

        default:
            return -1;
    }

    return ok ? 0 : -1;
}

int arch_spi_impl_set_callback(uint8_t id,
                               arch_spi_cb_t cb,
                               uintptr_t context)
{
    if (id >= SPI_MAX_INSTANCES)
        return -1;

    spi_callbacks[id] = cb;
    spi_contexts[id] = context;

    switch (id)
    {
#if (CONFIG_SPI_MASK & SPI1_MASK)
        case 0:
            SPI1_CallbackRegister(spi1_cb, 0);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI2_MASK)
        case 1:
            SPI2_CallbackRegister(spi2_cb, 0);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI3_MASK)
        case 2:
            SPI3_CallbackRegister(spi3_cb, 0);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI4_MASK)
        case 3:
            SPI4_CallbackRegister(spi4_cb, 0);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI5_MASK)
        case 4:
            SPI5_CallbackRegister(spi5_cb, 0);
            break;
#endif

#if (CONFIG_SPI_MASK & SPI6_MASK)
        case 5:
            SPI6_CallbackRegister(spi6_cb, 0);
            break;
#endif

        default:
            return -1;
    }

    return 0;
}

#endif /* defined(__PIC32MZ2048EFM144_HARMONY__) || defined(__32MZ2048EFM144_HARMONY__) */
