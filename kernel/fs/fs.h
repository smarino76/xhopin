/**
 * @file fs.h
 * @author Santiago Marino
 * @year 2026
 * @brief Abstract filesystem interface.
 * 
 * This file defines the operations that every filesystem must implement,
 * and the structure that represents a mounted filesystem instance.
 * Filesystem?specific data (e.g., file nodes, block maps) are stored in
 * the private `ctx` field.
 */

#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stddef.h>
#include "../../drivers/storage/storage.h"

typedef struct fs fs_t;   /**< Opaque filesystem type */

/**
 * @struct fs_ops_t
 * @brief Operations that a filesystem must implement.
 */
typedef struct {
    /**
     * @brief Mount a storage device as a filesystem.
     * @param fs  Filesystem instance.
     * @param dev Storage device to mount.
     * @return 0 on success, -1 on error.
     */
    int (*mount)(fs_t *fs, storage_dev_t *dev);

    /**
     * @brief Format the underlying storage device (erase all data).
     * @param fs Filesystem instance.
     * @return 0 on success, -1 on error.
     */
    int (*format)(fs_t *fs);

    /**
     * @brief Create a new empty file.
     * @param fs   Filesystem instance.
     * @param name File name.
     * @return 0 on success, -1 on error.
     */
    int (*create)(fs_t *fs, const char *name);

    /**
     * @brief Read data from a file.
     * @param fs   Filesystem instance.
     * @param name File name.
     * @param data Buffer to receive data.
     * @param len  Number of bytes to read.
     * @return Number of bytes read, or -1 on error.
     */
    int (*read)(fs_t *fs, const char *name, uint8_t *data, size_t len);

    /**
     * @brief Write data to a file (overwrites existing content).
     * @param fs   Filesystem instance.
     * @param name File name.
     * @param data Buffer containing data.
     * @param len  Number of bytes to write.
     * @return Number of bytes written, or -1 on error.
     */
    int (*write)(fs_t *fs, const char *name, const uint8_t *data, size_t len);

    /**
     * @brief Delete a file.
     * @param fs   Filesystem instance.
     * @param name File name.
     * @return 0 on success, -1 on error.
     */
    int (*remove)(fs_t *fs, const char *name);

    /**
     * @brief Rename a file.
     * @param fs       Filesystem instance.
     * @param old_name Current name.
     * @param new_name New name.
     * @return 0 on success, -1 on error.
     */
    int (*rename)(fs_t *fs, const char *old_name, const char *new_name);
} fs_ops_t;

/**
 * @struct fs
 * @brief Filesystem instance.
 * 
 * Each mounted filesystem has a name, a pointer to its operations,
 * a pointer to the underlying storage device, and a private context
 * area (`ctx`) for filesystem?specific data (e.g., block allocator state).
 */
struct fs {
    const char *name;          /**< Filesystem name (e.g., "ramfs") */
    void *ctx;                 /**< Private data for the filesystem */
    storage_dev_t *storage;    /**< Underlying storage device */
    fs_ops_t *ops;             /**< Operation table */
};

#endif /* FS_H */