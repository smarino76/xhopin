#ifndef ARCH_BUS_SPI_H
#define ARCH_BUS_SPI_H

#include "arch.h"
#include "arch_bus.h"

typedef void (*arch_spi_cb_t)(uintptr_t);

typedef struct {
    arch_bus_config_t bs;   // ? comune
    uint32_t frequency;
    uint8_t mode;   // CPOL/CPHA
    uint8_t bits;
} arch_spi_config_t;

// lifecycle
int arch_spi_init(void);
int arch_spi_open(uint8_t id, arch_spi_config_t* cfg);

// operations
int arch_spi_transfer(uint8_t id,
                      void* tx,
                      size_t tx_size,
                      void* rx,
                      size_t rx_size);

// callback
int arch_spi_set_callback(uint8_t id,
                          arch_spi_cb_t cb,
                          uintptr_t ctx);

#endif