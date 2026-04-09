#include "drivers/storage/storage.h"
#include "kernel/memory/mem_alloc.h"
#include "storage_ram.h"
#include "arch/arch.h"

#define RAMDISK_BLOCK_SIZE      ARCH_RAMDISK_BLOCK_SIZE
#define RAMDISK_BLOCK_COUNT     ARCH_RAMDISK_BLOCK_COUNT

typedef struct {
    uint8_t *buffer;
} ram_ctx_t;

static void mem_copy(uint8_t *dst, const uint8_t *src, size_t len) {
    for (size_t i = 0; i < len; i++) dst[i] = src[i];
}
static void mem_set(uint8_t *dst, uint8_t value, size_t len) {
    for (size_t i = 0; i < len; i++) dst[i] = value;
}

static int ram_init(storage_dev_t *dev) {
    ram_ctx_t *ctx = mem_alloc(sizeof(ram_ctx_t));
    if (!ctx) return -1;

    ctx->buffer = mem_alloc(RAMDISK_BLOCK_SIZE * RAMDISK_BLOCK_COUNT);
    if (!ctx->buffer) {
        mem_free(ctx);
        return -1;
    }

    mem_set(ctx->buffer, 0xfd, RAMDISK_BLOCK_SIZE * RAMDISK_BLOCK_COUNT);
    dev->dev_base.ctx = ctx;
    return 0;
}

static int ram_deinit(storage_dev_t *dev) {
    ram_ctx_t *ctx = (ram_ctx_t *)dev->dev_base.ctx;
    if (ctx) {
        if (ctx->buffer)
            mem_free(ctx->buffer);
        mem_free(ctx);
        dev->dev_base.ctx = NULL;
    }
    return 0;
}

static int ram_read(storage_dev_t *dev, uint32_t block, uint8_t *data, size_t len) {
    ram_ctx_t *ctx = (ram_ctx_t *)dev->dev_base.ctx;
    if (!ctx || !data) return -1;
    if (block >= RAMDISK_BLOCK_COUNT) return -1;
    if (len > RAMDISK_BLOCK_SIZE) return -1;

    uint8_t *src = &ctx->buffer[block * RAMDISK_BLOCK_SIZE];
    mem_copy(data, src, len);
    return 0;
}

static int ram_write(storage_dev_t *dev, uint32_t block, const uint8_t *data, size_t len) {
    ram_ctx_t *ctx = (ram_ctx_t *)dev->dev_base.ctx;
    if (!ctx || !data) return -1;
    if (block >= RAMDISK_BLOCK_COUNT) return -1;
    if (len > RAMDISK_BLOCK_SIZE) return -1;

    uint8_t *dst = &ctx->buffer[block * RAMDISK_BLOCK_SIZE];
    mem_copy(dst, data, len);
    return 0;
}

static int ram_info(storage_dev_t *dev, storage_info_t *info) {
    if (!info) return -1;
    info->producer    = "Bianca Kernel";
    info->version     = "1.0.0";
    info->block_size  = RAMDISK_BLOCK_SIZE;
    info->block_count = RAMDISK_BLOCK_COUNT;
    return 0;
}

static storage_ops_t ram_ops = {
    .init     = ram_init,
    .deinit   = ram_deinit,
    .read     = ram_read,
    .write    = ram_write,
    .get_info = ram_info
};

static storage_dev_t *ram_dev = NULL;

void storage_ram_init(void) {
    ram_dev = mem_alloc(sizeof(storage_dev_t));
    if (!ram_dev) return;

    ram_dev->dev_base.name = "ramdisk0";
    ram_dev->dev_base.class_dev = DEVICE_CLASS_STORAGE;
    ram_dev->dev_base.class_ops = &ram_ops;
    ram_dev->dev_base.ctx = NULL;
    ram_dev->dev_base.ops = NULL;
    ram_dev->dev_base.allocated = 1;

    storage_register(ram_dev);

    storage_ops_t *ops = storage_get_ops(ram_dev);
    if (ops && ops->init)
        ops->init(ram_dev);
}