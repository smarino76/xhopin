#include "kernel/fs/fs.h"
#include "kernel/memory/mem_alloc.h"
#include "drivers/storage/storage.h"

// FILE
typedef struct {
    char name[32];
    uint32_t size;
    uint32_t* blocks;
    uint32_t block_count;
} file_t;

// FILE NODE
typedef struct file_node {
    file_t file;
    struct file_node* next;
} file_node_t;

// CONTEXT
typedef struct {
    file_node_t* head;
    uint8_t* block_map;
    uint32_t total_blocks;
} ramfs_ctx_t;

// UTILS
static int str_eq(const char* a, const char* b) {
    int i = 0;
    while (1) {
        if (a[i] != b[i]) return 0;
        if (a[i] == '\0') return 1;
        i++;
    }
}

static file_node_t* find_file(ramfs_ctx_t* ctx, const char* name) {
    file_node_t* cur = ctx->head;
    while (cur) {
        if (str_eq(cur->file.name, name)) return cur;
        cur = cur->next;
    }
    return 0;
}

// BLOCK ALLOCATOR
static int alloc_block(ramfs_ctx_t* ctx) {
    for (uint32_t i = 0; i < ctx->total_blocks; i++) {
        if (ctx->block_map[i] == 0) {
            ctx->block_map[i] = 1;
            return i;
        }
    }
    return -1;
}

// OPS
static int ramfs_mount(fs_t* fs, storage_dev_t* dev) {
    if (!fs || !dev) return -1;
    ramfs_ctx_t* ctx = mem_alloc(sizeof(ramfs_ctx_t));
    if (!ctx) return -1;

    storage_ops_t* ops = storage_get_ops(dev);
    storage_info_t info;
    ops->get_info(dev, &info);
    ctx->total_blocks = info.block_count;
    ctx->block_map = mem_alloc(ctx->total_blocks);
    if (!ctx->block_map) return -1;
    for (uint32_t i = 0; i < ctx->total_blocks; i++) ctx->block_map[i] = 0;
    ctx->head = 0;
    fs->ctx = ctx;
    fs->storage = dev;
    return 0;
}

static int ramfs_create(fs_t* fs, const char* name) {
    ramfs_ctx_t* ctx = fs->ctx;
    if (find_file(ctx, name)) return -1;
    file_node_t* node = mem_alloc(sizeof(file_node_t));
    if (!node) return -1;
    for (int i = 0; i < 32; i++) {
        node->file.name[i] = name[i];
        if (name[i] == '\0') break;
    }
    node->file.size = 0;
    node->file.blocks = 0;
    node->file.block_count = 0;
    node->next = ctx->head;
    ctx->head = node;
    return 0;
}

static int ramfs_write(fs_t* fs, const char* name, const uint8_t* data, size_t len) {
    ramfs_ctx_t* ctx = fs->ctx;
    storage_dev_t* dev = fs->storage;
    file_node_t* node = find_file(ctx, name);
    if (!node) return -1;

    storage_ops_t* ops = storage_get_ops(dev);
    storage_info_t info;
    ops->get_info(dev, &info);
    uint32_t block_size = info.block_size;
    uint32_t needed = (len + block_size - 1) / block_size;

    node->file.blocks = mem_alloc(sizeof(uint32_t) * needed);
    if (!node->file.blocks) return -1;
    node->file.block_count = needed;

    for (uint32_t i = 0; i < needed; i++) {
        int blk = alloc_block(ctx);
        if (blk < 0) return -1;
        node->file.blocks[i] = blk;
        ops->write(dev, blk, (uint8_t*)&data[i * block_size], block_size);
    }
    node->file.size = len;
    return 0;
}

static int ramfs_read(fs_t* fs, const char* name, uint8_t* data, size_t len) {
    ramfs_ctx_t* ctx = fs->ctx;
    storage_dev_t* dev = fs->storage;
    file_node_t* node = find_file(ctx, name);
    if (!node) return -1;

    storage_ops_t* ops = storage_get_ops(dev);
    storage_info_t info;
    ops->get_info(dev, &info);
    uint32_t block_size = info.block_size;

    for (uint32_t i = 0; i < node->file.block_count; i++) {
        ops->read(dev, node->file.blocks[i], &data[i * block_size], block_size);
    }
    return 0;
}

static int ramfs_remove(fs_t* fs, const char* name) {
    ramfs_ctx_t* ctx = fs->ctx;
    file_node_t* prev = 0;
    file_node_t* cur = ctx->head;
    while (cur) {
        if (str_eq(cur->file.name, name)) {
            for (uint32_t i = 0; i < cur->file.block_count; i++) {
                ctx->block_map[cur->file.blocks[i]] = 0;
            }
            if (prev) prev->next = cur->next;
            else ctx->head = cur->next;
            mem_free(cur->file.blocks);
            mem_free(cur);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return -1;
}

static fs_ops_t ramfs_ops = {
    .mount  = ramfs_mount,
    .create = ramfs_create,
    .read   = ramfs_read,
    .write  = ramfs_write,
    .remove = ramfs_remove
};

fs_t ramfs2 = {
    .name = "ramfs_v2",
    .ctx = 0,
    .storage = 0,
    .ops = &ramfs_ops
};