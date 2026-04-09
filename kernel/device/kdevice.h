/**
 * @file kdevice.h
 * @author Santiago Marino
 * @year 2026
 * @brief Gestione dei dispositivi del kernel.
 */

#ifndef KDEVICE_H
#define KDEVICE_H

#include "../../arch/arch.h"

/**
 * @enum device_class_t
 * @brief Classi di dispositivi supportate.
 */
typedef enum {
    DEVICE_CLASS_STORAGE,
    DEVICE_CLASS_INPUT,
    DEVICE_CLASS_OUTPUT,
    DEVICE_CLASS_DISPLAY,
    DEVICE_CLASS_CONSOLE,   /* <-- Nuovo: dispositivo di console (I/O testuale) */
    DEVICE_CLASS_MAX
} device_class_t;

typedef struct kdevice device_t;

typedef struct {
    int (*init)(device_t *dev);
    int (*deinit)(device_t *dev);
    int (*get_info)(device_t *dev, char *buf, size_t len);
} device_ops_t;

struct kdevice {
    const char *name;
    device_class_t class_dev;
    void *class_ops;
    void *ctx;
    device_ops_t *ops;
    uint8_t allocated;
};

int device_register(device_t *dev);
int device_unregister(device_t *dev);
device_t *device_get_by_name(const char *name);
device_t *device_get_by_class(device_class_t class, uint8_t index);
uint8_t device_count_by_class(device_class_t class);

#endif