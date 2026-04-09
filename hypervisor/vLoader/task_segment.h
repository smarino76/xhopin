#ifndef TASK_SEGMENT_H
#define TASK_SEGMENT_H
#include "../../kconfig.h"


#if defined(USE_HYPERVISOR_OS)

typedef struct TaskSegment TaskSegment;

#define BLOCK_SIZE TASK_SEGMENT_BLOCK_SIZE

struct TaskSegment {
    // Tamano del bloque de codigo
    size_t block_size;

    // Puntero al bloque de datos en la RAM
    void * block_data;

    // dónde empieza este bloque en el programa global
    size_t base_offset;
};


#endif
#endif