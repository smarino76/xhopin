/**
 * @file mount.h
 * @author Santiago Marino
 * @year 2026
 * @brief Gestione della tabella dei mount per storage multipli.
 * 
 * Permette di associare un nome logico di dispositivo (es. "ram0")
 * a un dispositivo storage e al filesystem montato su di esso.
 */

#ifndef MOUNT_H
#define MOUNT_H

#include "kernel/fs/fs.h"
#include "drivers/storage/storage.h"

#define MAX_MOUNTS 4        /**< Numero massimo di mount point */
#define MOUNT_NAME_LEN 16   /**< Lunghezza massima del nome del dispositivo */

/**
 * @struct mount_entry_t
 * @brief Singola entry nella tabella dei mount.
 */
typedef struct {
    char dev_name[MOUNT_NAME_LEN];   /**< Nome logico del dispositivo (es. "ram0") */
    storage_dev_t *storage;          /**< Puntatore al dispositivo storage */
    fs_t *fs;                        /**< Puntatore al filesystem montato */
} mount_entry_t;

/**
 * @brief Inizializza la tabella dei mount (tutte le entry vuote).
 */
void mount_init(void);

/**
 * @brief Registra un nuovo mount point.
 * 
 * @param dev_name Nome logico del dispositivo
 * @param dev Puntatore al dispositivo storage
 * @param fs Puntatore al filesystem da montare
 * @return 0 se successo, -1 se tabella piena o parametri nulli
 */
int mount_register(const char *dev_name, storage_dev_t *dev, fs_t *fs);

/**
 * @brief Cerca un filesystem in base al nome del dispositivo.
 * 
 * @param dev_name Nome logico del dispositivo
 * @return Puntatore al filesystem, o NULL se non trovato
 */
fs_t* mount_get_fs(const char *dev_name);

/**
 * @brief Cerca un dispositivo storage in base al nome logico.
 * 
 * @param dev_name Nome logico del dispositivo
 * @return Puntatore al dispositivo storage, o NULL se non trovato
 */
storage_dev_t* mount_get_storage(const char *dev_name);

#endif /* MOUNT_H */