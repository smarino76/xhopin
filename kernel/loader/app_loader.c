/**
 * @file app_loader.c
 * @author Santiago Marino
 * @year 2026
 * @brief Application loader for VM binaries.
 * 
 * Loads a binary (header + bytecode) from memory, validates it,
 * creates a VM context, and creates a kernel task to run it.
 * 
 * The binary is expected to be in little-endian byte order (as generated
 * by the assembler on a little-endian host). All header fields are
 * converted to host endianness using le32_to_host().
 */

#include "kernel/loader/app_loader.h"
#include "kernel/vm/vm_core.h"
#include "kernel/scheduler/task.h"
#include "kernel/memory/mem_alloc.h"
#include <string.h>

#define APP_MAGIC 0xCAFEFACE

/**
 * @brief Convert a 32-bit little-endian value to host endianness.
 * @param x Value in little-endian byte order.
 * @return Value in host byte order.
 * 
 * On a little-endian CPU (like PIC32), this returns the value unchanged.
 * On a big-endian CPU, it swaps the bytes.
 */
static inline uint32_t le32_to_host(uint32_t x) {
    return ((x >> 24) & 0xFF) |
           ((x >> 8)  & 0xFF00) |
           ((x << 8)  & 0xFF0000) |
           ((x << 24) & 0xFF000000);
}

/**
 * @brief Load an application from a binary blob.
 * 
 * @param binary Pointer to the binary data (header + bytecode)
 * @param bin_size Size of the binary data in bytes
 * @return Task ID on success, negative error code on failure
 * 
 * Error codes:
 *   -1: Invalid parameters (NULL or too small)
 *   -2: Invalid magic number
 *   -3: Invalid checksum (not implemented)
 *   -4: Unsupported ABI version
 *   -5: Code size exceeds binary size
 *   -6: VM creation failed
 *   -7: Task creation failed
 */
int kernel_load_app(const uint8_t *binary, uint32_t bin_size) {
    if (binary == NULL || bin_size < sizeof(AppHeader))
        return -1;

    AppHeader *hdr = (AppHeader*)binary;

    /* Convert magic from little-endian to host endianness */
    //uint32_t magic = le32_to_host(hdr->magic);
    uint32_t magic = hdr->magic;
    if (magic != APP_MAGIC)
        return -2;

    /* Convert version fields */
    //uint32_t version = le32_to_host(hdr->version);
    uint32_t version = hdr->version;
    if (version < VM_ABI_MIN_VERSION || version > VM_ABI_VERSION)
        return -4;

    /* Convert code_size, ram_size, entry_point, capabilities */
    //uint32_t code_size = le32_to_host(hdr->code_size);
    uint32_t code_size = hdr->code_size;
    
    //uint32_t ram_size = le32_to_host(hdr->ram_size);
    uint32_t ram_size = hdr->ram_size;
    
    //uint32_t entry_point = le32_to_host(hdr->entry_point);
    uint32_t entry_point = hdr->entry_point;
    
    
    //uint32_t capabilities = le32_to_host(hdr->capabilities);
    uint32_t capabilities = hdr->capabilities;

    /* Optional: checksum validation (commented out) */
    // uint32_t checksum = le32_to_host(hdr->checksum);
    // if (compute_checksum(binary, bin_size) != checksum) return -3;

    /* Verify code_size doesn't exceed binary */
    if (code_size > bin_size - sizeof(AppHeader))
        return -5;

    /* Create a temporary header with converted values */
    AppHeader converted_hdr;
    converted_hdr.magic = magic;
    strncpy(converted_hdr.name, hdr->name, 16);
    converted_hdr.version = version;
    converted_hdr.code_size = code_size;
    converted_hdr.ram_size = ram_size;
    converted_hdr.entry_point = entry_point;
    converted_hdr.capabilities = capabilities;
    converted_hdr.checksum = 0;  // not used

    /* Bytecode starts right after the header */
    uint8_t *code = (uint8_t*)binary + sizeof(AppHeader);

    /* Create the VM */
    VMContext *vm = vm_create(&converted_hdr, code);
    if (!vm)
        return -6;

    /* Create a kernel task to run the VM interpreter loop */
    Task *task = task_create((void (*)(void *))vm_interpreter_loop, (void *)vm, 1024);
    if (!task) {
        vm_destroy(vm);
        return -7;
    }

    /* Store VM pointer in task's private data for cleanup */
    task->priv_data = vm;

    return (int)task->id;
}