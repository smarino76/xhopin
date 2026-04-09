#include "kdevice.h"
#include <string.h>
#include "kernel/memory/mem_alloc.h"
#include "arch/include/arch_cpu.h"
#include "kernel/event/event.h"

#define MAX_DEVICES MAX_KERNEL_DEVICES

static device_t *devices[MAX_DEVICES];
static uint8_t device_count = 0;

int device_register(device_t *dev) {
    if (!dev || device_count >= MAX_DEVICES)
        return -1;

    uint32_t flags = cpu_irq_save();
    devices[device_count++] = dev;
    cpu_irq_restore(flags);
    event_trigger(EVENT_DEVICE_ADDED, dev);
    return 0;
}

int device_unregister(device_t *dev) {
    if (!dev) return -1;

    uint32_t flags = cpu_irq_save();

    int i;
    for (i = 0; i < device_count; i++) {
        if (devices[i] == dev) break;
    }
    if (i == device_count) {
        cpu_irq_restore(flags);
        return -1;
    }

    if (dev->ops && dev->ops->deinit)
        dev->ops->deinit(dev);

    for (; i < device_count - 1; i++)
        devices[i] = devices[i + 1];
    device_count--;

    cpu_irq_restore(flags);
    event_trigger(EVENT_DEVICE_REMOVED, dev);
    if (dev->allocated)
        mem_free(dev);

    return 0;
}

device_t *device_get_by_name(const char *name) {
    if (!name) return NULL;
    uint32_t flags = cpu_irq_save();
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i]->name && strcmp(devices[i]->name, name) == 0) {
            cpu_irq_restore(flags);
            return devices[i];
        }
    }
    cpu_irq_restore(flags);
    return NULL;
}

device_t *device_get_by_class(device_class_t class, uint8_t index) {
    uint8_t found = 0;
    uint32_t flags = cpu_irq_save();
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i]->class_dev == class) {
            if (found == index) {
                cpu_irq_restore(flags);
                return devices[i];
            }
            found++;
        }
    }
    cpu_irq_restore(flags);
    return NULL;
}

uint8_t device_count_by_class(device_class_t class) {
    uint8_t count = 0;
    uint32_t flags = cpu_irq_save();
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i]->class_dev == class)
            count++;
    }
    cpu_irq_restore(flags);
    return count;
}