/**
 * @file ramfs.c
 * @author Santiago Marino
 * @year 2026
 * @brief RAM filesystem implementation (version 1 ? contiguous blocks).
 * 
 * This filesystem stores files in a linear array of blocks.
 * Each file occupies a contiguous range of blocks starting at `start_block`.
 * The number of free blocks is tracked by `next_free_block`.
 */

#include "kernel/fs/fs.h"
#include "kernel/memory/mem_alloc.h"
#include "drivers/storage/storage.h"   /* for storage_get_ops */

/* -------------------------------------------------------------------------
 * File structure (private to this filesystem)
 * ------------------------------------------------------------------------- */

/**
 * @struct file_t
 * @brief Descriptor of a single file.
 */
typedef struct {
    char name[32];          /**< File name (null?terminated) */
    uint32_t size;          /**< File size in bytes */
    uint32_t start_block;   /**< First block number */
} file_t;

/**
 * @struct file_node
 * @brief Linked list node for a file.
 */
typedef struct file_node {
    file_t file;                /**< File descriptor */
    struct file_node *next;     /**< Next file in the list */
} file_node_t;

/**
 * @struct ramfs_ctx_t
 * @brief Private context for ramfs.
 */
typedef struct {
    file_node_t *head;          /**< Head of the file list */
    uint32_t next_free_block;   /**< Next unused block number */
} ramfs_ctx_t;

/* -------------------------------------------------------------------------
 * Helper functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Compare two strings.
 * @param a First string.
 * @param b Second string.
 * @return 1 if equal, 0 otherwise.
 */
static int str_eq(const char *a, const char *b) {
    int i = 0;
    while (1) {
        if (a[i] != b[i]) return 0;
        if (a[i] == '\0') return 1;
        i++;
    }
}

/**
 * @brief Copy a string into a fixed?size buffer.
 * @param dst Destination buffer.
 * @param src Source string.
 * @param max Buffer size (including null terminator).
 */
static void str_copy(char *dst, const char *src, int max) {
    int i = 0;
    for (; i < max; i++) {
        dst[i] = src[i];
        if (src[i] == '\0') break;
    }
    for (; i < max; i++) dst[i] = 0;
}

/**
 * @brief Find a file by name.
 * @param ctx  Filesystem context.
 * @param name File name.
 * @return Pointer to the file node, or NULL if not found.
 */
static file_node_t *find_file(ramfs_ctx_t *ctx, const char *name) {
    file_node_t *cur = ctx->head;
    while (cur) {
        if (str_eq(cur->file.name, name)) return cur;
        cur = cur->next;
    }
    return 0;
}

/* -------------------------------------------------------------------------
 * Filesystem operations
 * ------------------------------------------------------------------------- */

static int ramfs_mount(fs_t *fs, storage_dev_t *dev) {
    if (!fs || !dev) return -1;
    ramfs_ctx_t *ctx = mem_alloc(sizeof(ramfs_ctx_t));
    if (!ctx) return -1;
    ctx->head = 0;
    ctx->next_free_block = 0;
    fs->ctx = ctx;
    fs->storage = dev;
    return 0;
}

static int ramfs_format(fs_t *fs) {
    ramfs_ctx_t *ctx = fs->ctx;
    ctx->head = 0;
    ctx->next_free_block = 0;
    return 0;
}

static int ramfs_create(fs_t *fs, const char *name) {
    ramfs_ctx_t *ctx = fs->ctx;
    if (find_file(ctx, name)) return -1;
    file_node_t *node = mem_alloc(sizeof(file_node_t));
    if (!node) return -1;
    str_copy(node->file.name, name, 32);
    node->file.size = 0;
    node->file.start_block = ctx->next_free_block;
    node->next = ctx->head;
    ctx->head = node;
    return 0;
}

static int ramfs_write(fs_t *fs, const char *name, const uint8_t *data, size_t len) {
    ramfs_ctx_t *ctx = fs->ctx;
    storage_dev_t *dev = fs->storage;
    file_node_t *node = find_file(ctx, name);
    if (!node) return -1;

    storage_ops_t *ops = storage_get_ops(dev);
    storage_info_t info;
    ops->get_info(dev, &info);
    uint32_t block_size = info.block_size;
    uint32_t blocks = (len + block_size - 1) / block_size;

    for (uint32_t i = 0; i < blocks; i++) {
        uint32_t offset = i * block_size;
        uint32_t remaining = len - offset;
        uint32_t write_size = (remaining < block_size) ? remaining : block_size;
        ops->write(dev, node->file.start_block + i, (uint8_t *)&data[offset], write_size);
    }
    node->file.size = len;
    ctx->next_free_block += blocks;
    return 0;
}

static int ramfs_read(fs_t *fs, const char *name, uint8_t *data, size_t len) {
    ramfs_ctx_t *ctx = fs->ctx;
    storage_dev_t *dev = fs->storage;
    file_node_t *node = find_file(ctx, name);
    if (!node) return -1;

    storage_ops_t *ops = storage_get_ops(dev);
    storage_info_t info;
    ops->get_info(dev, &info);
    uint32_t block_size = info.block_size;
    size_t to_read = (len < node->file.size) ? len : node->file.size;
    uint32_t blocks = (to_read + block_size - 1) / block_size;

    for (uint32_t i = 0; i < blocks; i++) {
        uint32_t offset = i * block_size;
        uint32_t remaining = to_read - offset;
        uint32_t read_size = (remaining < block_size) ? remaining : block_size;
        ops->read(dev, node->file.start_block + i, &data[offset], read_size);
    }
    return 0;
}

static int ramfs_remove(fs_t *fs, const char *name) {
    ramfs_ctx_t *ctx = fs->ctx;
    file_node_t *prev = 0;
    file_node_t *cur = ctx->head;
    while (cur) {
        if (str_eq(cur->file.name, name)) {
            if (prev) prev->next = cur->next;
            else ctx->head = cur->next;
            mem_free(cur);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return -1;
}

static int ramfs_rename(fs_t *fs, const char *old_name, const char *new_name) {
    ramfs_ctx_t *ctx = fs->ctx;
    file_node_t *node = find_file(ctx, old_name);
    if (!node) return -1;
    str_copy(node->file.name, new_name, 32);
    return 0;
}

/* -------------------------------------------------------------------------
 * Operation table and instance
 * ------------------------------------------------------------------------- */

static fs_ops_t ramfs_ops = {
    .mount = ramfs_mount,
    .format = ramfs_format,
    .create = ramfs_create,
    .read = ramfs_read,
    .write = ramfs_write,
    .remove = ramfs_remove,
    .rename = ramfs_rename
};

fs_t ramfs = {
    .name = "ramfs",
    .ctx = 0,
    .storage = 0,
    .ops = &ramfs_ops
};