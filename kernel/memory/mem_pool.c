#include "mem_pool.h"
#include "mem_alloc.h"

/* ---------------------------------- */
/* bit operations                     */
/* ---------------------------------- */

static inline int bit_get(uint32_t *bitmap, int index)
{
    return (bitmap[index >> 5] >> (index & 31)) & 1;
}

static inline void bit_set(uint32_t *bitmap, int index)
{
    bitmap[index >> 5] |= (1u << (index & 31));
}

static inline void bit_clear(uint32_t *bitmap, int index)
{
    bitmap[index >> 5] &= ~(1u << (index & 31));
}

/* ---------------------------------- */
/* init pool                          */
/* ---------------------------------- */

int mem_pool_init(
        mem_pool *pool,
        int capacity,
        size_t object_size)
{
    if(!pool || capacity <= 0)
        return -1;

    pool->object_size = object_size;
    pool->capacity = capacity;

    size_t mem_size = capacity * object_size;

    pool->memory = mem_alloc(mem_size);

    if(!pool->memory)
        return -1;

    int bitmap_words = (capacity + 31) / 32;

    pool->bitmap = mem_alloc(bitmap_words * sizeof(uint32_t));

    if(!pool->bitmap)
        return -1;

    for(int i = 0; i < bitmap_words; i++)
        pool->bitmap[i] = 0;

    return 0;
}

/* ---------------------------------- */
/* alloc object                       */
/* ---------------------------------- */

void* mem_pool_alloc(mem_pool *pool)
{
    if(!pool)
        return NULL;

    for(int i = 0; i < pool->capacity; i++)
    {
        if(!bit_get(pool->bitmap, i))
        {
            bit_set(pool->bitmap, i);

            uint8_t *p = (uint8_t*)pool->memory;

            return p + (i * pool->object_size);
        }
    }

    return NULL;
}

/* ---------------------------------- */
/* free object                        */
/* ---------------------------------- */

void mem_pool_free(mem_pool *pool, void *ptr)
{
    if(!pool || !ptr)
        return;

    uint8_t *base = (uint8_t*)pool->memory;

    int index = ((uint8_t*)ptr - base) / pool->object_size;

    if(index < 0 || index >= pool->capacity)
        return;

    bit_clear(pool->bitmap, index);
}

/* ---------------------------------- */
/* free objects count                 */
/* ---------------------------------- */

int mem_pool_free_count(mem_pool *pool)
{
    if(!pool)
        return 0;

    int free = 0;

    for(int i = 0; i < pool->capacity; i++)
    {
        if(!bit_get(pool->bitmap, i))
            free++;
    }

    return free;
}
