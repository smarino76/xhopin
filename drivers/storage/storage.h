#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stddef.h>
#include "kernel/device/kdevice.h"

typedef struct storage_dev storage_dev_t;

typedef struct {
    uint32_t block_size;
    uint32_t block_count;
    const char *producer;
    const char *version;
} storage_info_t;

typedef struct {
    int (*init)(storage_dev_t *dev);
    int (*deinit)(storage_dev_t *dev);
    int (*read)(storage_dev_t *dev, uint32_t block, uint8_t *data, size_t len);
    int (*write)(storage_dev_t *dev, uint32_t block, const uint8_t *data, size_t len);
    int (*get_info)(storage_dev_t *dev, storage_info_t *info);
} storage_ops_t;

struct storage_dev {
    device_t dev_base;          /* primo campo per cast sicuro */
};

static inline storage_ops_t *storage_get_ops(storage_dev_t *dev) {
    return (storage_ops_t*)dev->dev_base.class_ops;
}

int storage_register(storage_dev_t *dev);
int storage_unregister(storage_dev_t *dev);
storage_dev_t *storage_get(uint8_t id);

#endif