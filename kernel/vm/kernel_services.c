/**
 * @file kernel_services.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementazione dei servizi kernel.
 * 
 * Contiene le funzioni effettive che realizzano le operazioni richieste
 * dalle VM tramite VCALL: system, logging, sensors, storage.
 */

#include "kernel_services.h"
#include "kernel/fs/fs.h"
#include "kernel/fs/ramfs2/ramfs2.h"
#include "drivers/storage/storage.h"
#include "drivers/storage/ram/storage_ram.h"
#include "kernel/fs/mount.h"
#include "kernel/memory/mem_alloc.h"
#include "arch/include/arch_usart.h"
#include <string.h>

/* ----------------------------------------------------------------------------
 * storage_init
 * ---------------------------------------------------------------------------- */
void storage_init(void) {
    mount_init();                     /* Inizializza la tabella dei mount */
    storage_ram_init();               /* Crea e registra il RAM disk "ramdisk0" */
    storage_dev_t *ram_dev = storage_get(0);  /* Ottieni il primo dispositivo storage (id 0) */
    if (!ram_dev) return;
    fs_t *ram_fs = &ramfs2;           /* Usa il filesystem ramfs2 */
    if (fs_mount(ram_fs, ram_dev) == 0) {
        mount_register("ram0", ram_dev, ram_fs);  /* Monta con nome logico "ram0" */
    }
}

/* ----------------------------------------------------------------------------
 * Helper: get_string_from_vm
 * Copia una stringa dalla RAM della VM in un buffer allocato nell'heap del kernel.
 * ---------------------------------------------------------------------------- */
static char* get_string_from_vm(uint8_t *ram_base, uint32_t offset, uint32_t max_len) {
    char *str = (char*)mem_alloc(max_len + 1);
    if (!str) return NULL;
    strncpy(str, (char*)(ram_base + offset), max_len);
    str[max_len] = '\0';
    return str;
}

/* ----------------------------------------------------------------------------
 * Struttura dei parametri per le operazioni storage (passata dalla VM)
 * ---------------------------------------------------------------------------- */
typedef struct {
    uint32_t dev_name_offset;   /**< Offset (nella RAM della VM) della stringa del nome dispositivo */
    uint32_t file_name_offset;  /**< Offset della stringa del nome file */
    uint32_t data_offset;       /**< Offset del buffer dati (per READ/WRITE) */
    uint32_t size;              /**< Dimensione dati (per WRITE) o max size (per READ) */
} storage_params_t;

/* ----------------------------------------------------------------------------
 * handle_storage_service
 * ---------------------------------------------------------------------------- */
uint32_t handle_storage_service(uint32_t action, uint32_t p1, uint32_t p2, uint8_t *ram_base) {
    /* p1 è l'offset della struttura storage_params_t nella RAM della VM */
    storage_params_t *params = (storage_params_t*)(ram_base + p1);
    char *dev_name = get_string_from_vm(ram_base, params->dev_name_offset, 16);
    if (!dev_name) return 0xFFFFFFFF;

    fs_t *fs = mount_get_fs(dev_name);
    mem_free(dev_name);
    if (!fs) return 0xFFFFFFFF;

    char *file_name = get_string_from_vm(ram_base, params->file_name_offset, 64);
    if (!file_name) return 0xFFFFFFFF;

    int ret = -1;
    switch (action) {
        case ACTION_STORAGE_CREATE:
            ret = fs_create(fs, file_name);
            break;
        case ACTION_STORAGE_WRITE: {
            uint8_t *data = ram_base + params->data_offset;
            ret = fs_write(fs, file_name, data, params->size);
            break;
        }
        case ACTION_STORAGE_READ: {
            uint8_t *buf = ram_base + params->data_offset;
            ret = fs_read(fs, file_name, buf, params->size);
            if (ret >= 0) {
                mem_free(file_name);
                return ret;   /* Restituisce il numero di byte letti */
            }
            break;
        }
        case ACTION_STORAGE_DELETE:
            ret = fs_remove(fs, file_name);
            break;
        default:
            ret = -1;
    }
    mem_free(file_name);
    return (ret == 0) ? 0 : 0xFFFFFFFF;
}

/* ----------------------------------------------------------------------------
 * handle_system_service
 * ---------------------------------------------------------------------------- */
uint32_t handle_system_service(uint32_t action, uint32_t param) {
    if (action == ACTION_GET) {
        return 2026;   /* Versione del kernel */
    }
    return 0;
}

/* ----------------------------------------------------------------------------
 * handle_logging_service
 * ---------------------------------------------------------------------------- */
#include "kernel/device/console/console.h"

uint32_t handle_logging_service(uint32_t action, char* buffer, uint32_t size, uint32_t unused) {
    if (action == ACTION_SEND) {
        console_write(buffer, strlen(buffer));
        return 1;
    } else if (action == ACTION_RECEIVE) {
        return console_read(buffer, size);
    }
    return 0;
}

/* ----------------------------------------------------------------------------
 * handle_sensor_service
 * ---------------------------------------------------------------------------- */
uint32_t handle_sensor_service(uint32_t action, uint32_t sensor_id) {
    if (action == ACTION_GET) {
        if (sensor_id == 1) return 25;   /* Temperatura simulata */
        if (sensor_id == 2) return 80;   /* Batteria simulata (%) */
    }
    return 0;
}