#ifndef ARCH_BUS_I2C_H
#define ARCH_BUS_I2C_H

#include "arch.h"
#include "arch_bus.h"

typedef void (*arch_i2c_cb_t)(uintptr_t);

typedef struct {
    arch_bus_config_t bs;   // ? comune
    uint32_t speed;
} arch_i2c_config_t;

// lifecycle
int arch_i2c_init(void);
int arch_i2c_open(uint8_t id, arch_i2c_config_t* cfg);

// operations
int arch_i2c_read(uint8_t id,
                  uint16_t addr,
                  uint8_t* data,
                  size_t len);

int arch_i2c_write(uint8_t id,
                   uint16_t addr,
                   uint8_t* data,
                   size_t len);

int arch_i2c_write_read(uint8_t id,
                        uint16_t addr,
                        uint8_t* wdata,
                        size_t wlen,
                        uint8_t* rdata,
                        size_t rlen);

// callback
int arch_i2c_set_callback(uint8_t id,
                          arch_i2c_cb_t cb,
                          uintptr_t ctx);

#endif