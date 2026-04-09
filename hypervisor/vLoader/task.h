/*
 
 ? TASK

? estado lógico
? PC global
 
 */





#ifndef TASK_H
#define TASK_H
#include "../../kconfig.h"


#if defined(USE_HYPERVISOR_OS)
#include "task_segment.h"

typedef struct {
    uint32_t magic;        // para validar (ej: 0xDEADBEEF)
    uint32_t version;      // formato
    uint32_t total_size;   // tamaño total del programa
    uint32_t entry_point;  // offset inicial de ejecución
    uint32_t flags;        // opcional (tipo, permisos, etc)
} ProgramHeader;


typedef enum {
    TASK_NEW,
    TASK_READY,
    TASK_RUNNING,
    TASK_WAITING_BLOCK,
    TASK_FINISHED
} TaskState;

typedef struct {
    int task_id;

    ProgramHeader header;

    size_t pc; // program counter global

    TaskState state;

} Task;


#endif