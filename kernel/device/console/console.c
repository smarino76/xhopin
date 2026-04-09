/**
 * @file console.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementazione del sottosistema console.
 */

#include "console.h"
#include <string.h>

static console_dev_t *active_console = NULL;   /**< Console attualmente in uso */

void console_init(void) {
    active_console = NULL;
}

int console_register(console_dev_t *dev) {
    if (!dev || !dev->ops || !dev->ops->write || !dev->ops->read)
        return -1;
    
    // Registra il dispositivo nel device manager
    if (device_register(&dev->dev_base) != 0)
        return -1;
    
    // Se è la prima console, la rende attiva
    if (active_console == NULL)
        active_console = dev;
    
    return 0;
}

int console_set_active(const char *name) {
    if (!name) return -1;
    device_t *dev = device_get_by_name(name);
    if (!dev || dev->class_dev != DEVICE_CLASS_CONSOLE)
        return -1;
    
    active_console = (console_dev_t*)dev;
    return 0;
}

console_dev_t *console_get_active(void) {
    return active_console;
}

int console_write(const char *buf, size_t len) {
    if (!active_console || !active_console->ops || !active_console->ops->write)
        return -1;
    return active_console->ops->write(active_console, buf, len);
}

int console_read(char *buf, size_t max_len) {
    if (!active_console || !active_console->ops || !active_console->ops->read)
        return -1;
    return active_console->ops->read(active_console, buf, max_len);
}