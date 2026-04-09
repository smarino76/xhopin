#include "mem_alloc.h"

/* parametri derivati */

#define NUM_UNITS   (MEM_HEAP_SIZE / MEM_UNIT_SIZE)
#define BITMAP_WORDS ((NUM_UNITS + 31) / 32)

#define MEM_MAGIC 0xBEEF

typedef struct
{
    uint16_t magic;
    uint16_t units;
} mem_header;

/* memoria */

static uint8_t heap[MEM_HEAP_SIZE];
static uint32_t bitmap[BITMAP_WORDS];

/* ---------------------------------- */
/* bit operations                     */
/* ---------------------------------- */

static inline int bit_get(int index)
{
    return (bitmap[index >> 5] >> (index & 31)) & 1;
}

static inline void bit_set(int index)
{
    bitmap[index >> 5] |= (1u << (index & 31));
}

static inline void bit_clear(int index)
{
    bitmap[index >> 5] &= ~(1u << (index & 31));
}

/* ---------------------------------- */
/* init                               */
/* ---------------------------------- */

void mem_init(void)
{
    for(int i = 0; i < BITMAP_WORDS; i++)
        bitmap[i] = 0;
}

/* ---------------------------------- */
/* ricerca blocchi liberi             */
/* ---------------------------------- */

static int find_free_units(int needed)
{
    int count = 0;
    int start = -1;

    for(int w = 0; w < BITMAP_WORDS; w++)
    {
        if(bitmap[w] == 0xFFFFFFFF)
            continue;

        for(int b = 0; b < 32; b++)
        {
            int index = w * 32 + b;

            if(index >= NUM_UNITS)
                return -1;

            if(!bit_get(index))
            {
                if(count == 0)
                    start = index;

                count++;

                if(count == needed)
                    return start;
            }
            else
            {
                count = 0;
            }
        }
    }

    return -1;
}

/* ---------------------------------- */
/* alloc                              */
/* ---------------------------------- */

void* mem_alloc(size_t size)
{
    if(size == 0)
        return NULL;

    size += sizeof(mem_header);

    int units = (size + MEM_UNIT_SIZE - 1) / MEM_UNIT_SIZE;

    int start = find_free_units(units);

    if(start < 0)
        return NULL;

    for(int i = 0; i < units; i++)
        bit_set(start + i);

    mem_header *h = (mem_header*)(heap + start * MEM_UNIT_SIZE);

    h->magic = MEM_MAGIC;
    h->units = units;

    return (void*)(h + 1);
}

/* ---------------------------------- */
/* free                               */
/* ---------------------------------- */

void mem_free(void *ptr)
{
    if(ptr == NULL)
        return;

    mem_header *h = ((mem_header*)ptr) - 1;

    if(h->magic != MEM_MAGIC)
        return;

    int start = ((uint8_t*)h - heap) / MEM_UNIT_SIZE;

    for(int i = 0; i < h->units; i++)
        bit_clear(start + i);

    h->magic = 0;
}

/* ---------------------------------- */
/* spazio libero                      */
/* ---------------------------------- */

size_t mem_free_space(void)
{
    int free_units = 0;

    for(int i = 0; i < NUM_UNITS; i++)
    {
        if(!bit_get(i))
            free_units++;
    }

    return free_units * MEM_UNIT_SIZE;
}
