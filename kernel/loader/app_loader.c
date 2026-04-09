/**
 * @file app_loader.c
 * @author Santiago Marino
 * @year 2026
 * @brief Caricatore di applicazioni VM (kernel residente).
 */

#include "kernel/loader/app_loader.h"
#include "kernel/vm/vm_core.h"
#include "kernel/scheduler/task.h"
#include "kernel/memory/mem_alloc.h"
#include <string.h>

#define APP_MAGIC 0xCAFEFACE

int kernel_load_app(const uint8_t *binary, uint32_t bin_size) {
    if (binary == NULL || bin_size < sizeof(AppHeader))
        return -1;

    AppHeader *hdr = (AppHeader*)binary;

    /* Verifica magic number */
    if (hdr->magic != APP_MAGIC)
        return -2;

    /* Controllo intervallo di versioni (compatibilità sia in avanti che all'indietro) */
    if (hdr->version < VM_ABI_MIN_VERSION || hdr->version > VM_ABI_VERSION)
        return -4;   /* Versione non supportata (troppo vecchia o troppo nuova) */

    /* Verifica checksum (opzionale, commentato per ora) */
    // if (compute_checksum(binary, bin_size) != hdr->checksum) return -3;

    uint8_t *code = (uint8_t*)binary + sizeof(AppHeader);
    uint32_t code_size = hdr->code_size;
    if (code_size > bin_size - sizeof(AppHeader))
        return -5;

    /* Crea la VM */
    VMContext *vm = vm_create(hdr, code);
    if (!vm)
        return -6;

    /* Crea un task che esegue il loop della VM */
    Task *task = task_create((void (*)(void *))vm_interpreter_loop, (void *)vm, 1024);
    if (!task) {
        vm_destroy(vm);
        return -7;
    }

    /* Collega la VM al task (per poterla distruggere in caso di terminazione) */
    task->priv_data = vm;

    return (int)task->id;   /* Restituisce l'ID del task (positivo) */
}