#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stddef.h>
#include <stdint.h>

/* pool descriptor */

typedef struct mem_pool
{
    void *memory;

    uint32_t *bitmap;

    size_t object_size;

    int capacity;

} mem_pool;

/* API */

int mem_pool_init(
        mem_pool *pool,
        int capacity,
        size_t object_size);

void* mem_pool_alloc(mem_pool *pool);

void mem_pool_free(mem_pool *pool, void *ptr);

int mem_pool_free_count(mem_pool *pool);

#endif
