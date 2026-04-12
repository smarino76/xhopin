/**
 * @file fs.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementation of the abstract filesystem interface.
 * 
 * This file provides wrapper functions that forward calls to the
 * filesystem?specific operation table.
 */

#include "fs.h"

/* -------------------------------------------------------------------------
 * Lifecycle operations
 * ------------------------------------------------------------------------- */

/**
 * @brief Mount a storage device as a filesystem.
 * @param fs  Filesystem instance.
 * @param dev Storage device.
 * @return 0 on success, -1 if invalid or operation missing.
 */
int fs_mount(fs_t *fs, storage_dev_t *dev) {
    if (!fs || !fs->ops || !fs->ops->mount)
        return -1;
    return fs->ops->mount(fs, dev);
}

/**
 * @brief Format the underlying storage device.
 * @param fs Filesystem instance.
 * @return 0 on success, -1 if invalid or operation missing.
 */
int fs_format(fs_t *fs) {
    if (!fs || !fs->ops || !fs->ops->format)
        return -1;
    return fs->ops->format(fs);
}

/* -------------------------------------------------------------------------
 * File operations
 * ------------------------------------------------------------------------- */

/**
 * @brief Create a new empty file.
 * @param fs   Filesystem instance.
 * @param name File name.
 * @return 0 on success, -1 on error.
 */
int fs_create(fs_t *fs, const char *name) {
    if (!fs || !fs->ops || !fs->ops->create)
        return -1;
    return fs->ops->create(fs, name);
}

/**
 * @brief Write data to a file.
 * @param fs   Filesystem instance.
 * @param name File name.
 * @param data Buffer containing data.
 * @param len  Number of bytes to write.
 * @return Number of bytes written, or -1 on error.
 */
int fs_write(fs_t *fs, const char *name, const uint8_t *data, size_t len) {
    if (!fs || !fs->ops || !fs->ops->write)
        return -1;
    return fs->ops->write(fs, name, data, len);
}

/**
 * @brief Read data from a file.
 * @param fs   Filesystem instance.
 * @param name File name.
 * @param data Buffer to receive data.
 * @param len  Number of bytes to read.
 * @return Number of bytes read, or -1 on error.
 */
int fs_read(fs_t *fs, const char *name, uint8_t *data, size_t len) {
    if (!fs || !fs->ops || !fs->ops->read)
        return -1;
    return fs->ops->read(fs, name, data, len);
}

/**
 * @brief Delete a file.
 * @param fs   Filesystem instance.
 * @param name File name.
 * @return 0 on success, -1 on error.
 */
int fs_remove(fs_t *fs, const char *name) {
    if (!fs || !fs->ops || !fs->ops->remove)
        return -1;
    return fs->ops->remove(fs, name);
}

/**
 * @brief Rename a file.
 * @param fs       Filesystem instance.
 * @param old_name Current name.
 * @param new_name New name.
 * @return 0 on success, -1 on error.
 */
int fs_rename(fs_t *fs, const char *old_name, const char *new_name) {
    if (!fs || !fs->ops || !fs->ops->rename)
        return -1;
    return fs->ops->rename(fs, old_name, new_name);
}