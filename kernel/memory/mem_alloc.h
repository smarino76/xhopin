
#ifdef __cplusplus
extern "C" {
#endif


#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H
#include <stddef.h>
#include <stdint.h>
#include "arch/arch.h"
//#include <stdint.h>
//#include <stddef.h>

/* configurazione */

#define MEM_HEAP_SIZE   MEM_RAM_SIZE/2
#define MEM_UNIT_SIZE   8

    
#ifndef NULL
#define NULL ((void*)0)
#endif

    
void mem_init(void);

void* mem_alloc(size_t size);

void mem_free(void *ptr);

size_t mem_free_space(void);

#endif
