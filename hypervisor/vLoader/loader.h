/*
 
 
 ? LOADER

? parsea header
? recibe datos
? mete bloques en cache
 
 
 
 */








#ifndef LOADER_H
#define LOADER_H
#include "../../kconfig.h"

#if defined(USE_HYPERVISOR_OS)

#include "task.h"

typedef LoaderContext LoaderContext;

typedef struct {
    uint8_t *buffer;
    size_t buffer_size;
    size_t write_pos;
    size_t read_pos;

    ProgramHeader temp_header;

    enum {
        LOADER_WAIT_HEADER,
        LOADER_LOADING
    } state;

} LoaderContext;

void loader_init(LoaderContext *ctx, uint8_t *buffer, size_t size);

bool loader_feed(LoaderContext *ctx, uint8_t *data, size_t size);

// procesa buffer (header + bloques)
bool loader_process(LoaderContext *ctx, Task *task, CodeCache *cache);

// crea task a partir del header
Task* loader_create_task(ProgramHeader *header);

// carga bloque en cache
bool loader_load_segment(CodeCache *cache, uint8_t *data, size_t size, size_t base_offset);

// limpieza
void loader_destroy_task(Task *task);



#endif
#endif