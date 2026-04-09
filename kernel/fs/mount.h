/**
 * @file mount.h
 * @author Santiago Marino
 * @year 2026
 * @brief Mount table management for multiple storage devices.
 * 
 * This file defines the mount table, which associates a logical device
 * name (e.g., "ram0") with a storage device and the filesystem mounted
 * on it. It allows the kernel to support multiple storage devices and
 * filesystem types.
 */

#ifndef MOUNT_H
#define MOUNT_H

#include "kernel/fs/fs.h"
#include "drivers/storage/storage.h"

#define MAX_MOUNTS 4        /**< Maximum number of mount points */
#define MOUNT_NAME_LEN 16   /**< Maximum length of a logical device name */

/**
 * @struct mount_entry_t
 * @brief Single entry in the mount table.
 */
typedef struct {
    char dev_name[MOUNT_NAME_LEN];   /**< Logical device name (e.g., "ram0") */
    storage_dev_t *storage;          /**< Underlying storage device */
    fs_t *fs;                        /**< Filesystem mounted on this device */
} mount_entry_t;

/**
 * @brief Initialise the mount table (all entries empty).
 */
void mount_init(void);

/**
 * @brief Register a new mount point.
 * @param dev_name Logical device name.
 * @param dev      Storage device.
 * @param fs       Filesystem mounted on that device.
 * @return 0 on success, -1 if the table is full or any parameter is NULL.
 */
int mount_register(const char *dev_name, storage_dev_t *dev, fs_t *fs);

/**
 * @brief Retrieve the filesystem associated with a logical device name.
 * @param dev_name Logical device name.
 * @return Pointer to the filesystem, or NULL if not found.
 */
fs_t *mount_get_fs(const char *dev_name);

/**
 * @brief Retrieve the storage device associated with a logical device name.
 * @param dev_name Logical device name.
 * @return Pointer to the storage device, or NULL if not found.
 */
storage_dev_t *mount_get_storage(const char *dev_name);

#endif /* MOUNT_H */