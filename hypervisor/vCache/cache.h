/*
 
 ? CACHE

? guarda bloques
? reemplaza bloques
 
 
 
 */






#ifndef CACHE_H
#define CACHE_H

#include "../../kconfig.h"


#if defined(USE_HYPERVISOR_OS)

#include "../vLoader/task_segment.h"

typedef struct {
    TaskSegment segments[MAX_CACHE_SEGMENTS];
    int count;
} CodeCache;



#endif

#endif