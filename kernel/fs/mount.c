/**
 * @file mount.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementation of the mount table.
 */

#include "kernel/fs/mount.h"
#include <string.h>

static mount_entry_t mounts[MAX_MOUNTS];  /**< Static mount table */
static int mount_count = 0;               /**< Number of active entries */

void mount_init(void) {
    mount_count = 0;
    for (int i = 0; i < MAX_MOUNTS; i++) {
        mounts[i].dev_name[0] = '\0';
        mounts[i].storage = NULL;
        mounts[i].fs = NULL;
    }
}

int mount_register(const char *dev_name, storage_dev_t *dev, fs_t *fs) {
    if (!dev_name || !dev || !fs) return -1;
    if (mount_count >= MAX_MOUNTS) return -1;
    strncpy(mounts[mount_count].dev_name, dev_name, MOUNT_NAME_LEN - 1);
    mounts[mount_count].storage = dev;
    mounts[mount_count].fs = fs;
    mount_count++;
    return 0;
}

fs_t *mount_get_fs(const char *dev_name) {
    for (int i = 0; i < mount_count; i++) {
        if (strcmp(mounts[i].dev_name, dev_name) == 0) {
            return mounts[i].fs;
        }
    }
    return NULL;
}

storage_dev_t *mount_get_storage(const char *dev_name) {
    for (int i = 0; i < mount_count; i++) {
        if (strcmp(mounts[i].dev_name, dev_name) == 0) {
            return mounts[i].storage;
        }
    }
    return NULL;
}