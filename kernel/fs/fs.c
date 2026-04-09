#include "fs.h"

// ==============================
// LIFECYCLE
// ==============================

int fs_mount(fs_t* fs, storage_dev_t* dev)
{
    if (!fs || !fs->ops || !fs->ops->mount)
        return -1;

    return fs->ops->mount(fs, dev);
}

int fs_format(fs_t* fs)
{
    if (!fs || !fs->ops || !fs->ops->format)
        return -1;

    return fs->ops->format(fs);
}

// ==============================
// FILE OPS
// ==============================

int fs_create(fs_t* fs, const char* name)
{
    if (!fs || !fs->ops || !fs->ops->create)
        return -1;

    return fs->ops->create(fs, name);
}

int fs_write(fs_t* fs,
             const char* name,
             const uint8_t* data,
             size_t len)
{
    if (!fs || !fs->ops || !fs->ops->write)
        return -1;

    return fs->ops->write(fs, name, data, len);
}

int fs_read(fs_t* fs,
            const char* name,
            uint8_t* data,
            size_t len)
{
    if (!fs || !fs->ops || !fs->ops->read)
        return -1;

    return fs->ops->read(fs, name, data, len);
}

int fs_remove(fs_t* fs, const char* name)
{
    if (!fs || !fs->ops || !fs->ops->remove)
        return -1;

    return fs->ops->remove(fs, name);
}

int fs_rename(fs_t* fs,
              const char* old_name,
              const char* new_name)
{
    if (!fs || !fs->ops || !fs->ops->rename)
        return -1;

    return fs->ops->rename(fs, old_name, new_name);
}