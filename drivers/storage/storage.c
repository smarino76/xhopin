#include "storage.h"
#include "kernel/device/kdevice.h"
#include "kernel/memory/mem_alloc.h"
#include "arch/include/arch_cpu.h"
#include <string.h>

#define MAX_STORAGE_DEVICES  ARCH_MAX_STORAGE_DEVICES

static storage_dev_t *storage_devs[MAX_STORAGE_DEVICES];
static uint8_t storage_count = 0;

int storage_register(storage_dev_t *dev) {
    if (!dev || storage_count >= MAX_STORAGE_DEVICES)
        return -1;

    uint32_t flags = cpu_irq_save();
    storage_devs[storage_count++] = dev;
    device_register(&dev->dev_base);
    cpu_irq_restore(flags);
    return 0;
}

int storage_unregister(storage_dev_t *dev) {
    if (!dev) return -1;

    uint32_t flags = cpu_irq_save();

    int i;
    for (i = 0; i < storage_count; i++) {
        if (storage_devs[i] == dev) break;
    }
    if (i == storage_count) {
        cpu_irq_restore(flags);
        return -1;
    }

    storage_ops_t *ops = storage_get_ops(dev);
    if (ops && ops->deinit)
        ops->deinit(dev);

    for (; i < storage_count - 1; i++)
        storage_devs[i] = storage_devs[i+1];
    storage_count--;

    cpu_irq_restore(flags);

    device_unregister(&dev->dev_base);

    return 0;
}

storage_dev_t *storage_get(uint8_t id) {
    if (id >= storage_count)
        return NULL;
    uint32_t flags = cpu_irq_save();
    storage_dev_t *dev = storage_devs[id];
    cpu_irq_restore(flags);
    return dev;
}