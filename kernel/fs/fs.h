#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stddef.h>
#include "../../drivers/storage/storage.h"

typedef struct fs fs_t;
//typedef struct file file_t;



// ==============================
// FS OPERATIONS
// ==============================

typedef struct {
    int (*mount)(fs_t* fs, storage_dev_t* dev);
    int (*format)(fs_t* fs);

    int (*create)(fs_t* fs, const char* name);
    int (*read)(fs_t* fs, const char* name, uint8_t* data, size_t len);
    int (*write)(fs_t* fs, const char* name, const uint8_t* data, size_t len);

    int (*remove)(fs_t* fs, const char* name);
    int (*rename)(fs_t* fs, const char* old_name, const char* new_name);
} fs_ops_t;

// ==============================
// FS INSTANCE
// ==============================

struct fs {
    const char* name;
    void* ctx;
    storage_dev_t* storage;
    fs_ops_t* ops;
};

#endif
