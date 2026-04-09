/**
 * @file kernel_services.h
 * @author Santiago Marino
 * @year 2026
 * @brief Dichiarazioni dei servizi kernel accessibili tramite VCALL.
 * 
 * Questo file definisce i prototipi delle funzioni che gestiscono
 * le diverse categorie di intent (SYSTEM, LOGGING, SENSORS, STORAGE)
 * e la funzione di inizializzazione dello storage.
 */

#ifndef KERNEL_SERVICES_H
#define KERNEL_SERVICES_H

#include "vm_types.h"

/**
 * @brief Punto di ingresso per tutte le chiamate VCALL.
 * 
 * Questa funzione è chiamata dall'interprete VM quando incontra OP_VCALL.
 * Riceve il contesto della VM, estrae i parametri dai registri (v0, a0, a1, a2)
 * e smista alla funzione specifica in base alla categoria.
 * 
 * @param vm Puntatore al contesto della VM chiamante
 * @return Risultato dell'operazione (scritto in k0 della VM)
 */
uint32_t kernel_intent_dispatcher(VMContext *vm);

/**
 * @brief Gestisce le chiamate della categoria CAP_SYSTEM.
 * 
 * Azioni supportate: ACTION_GET (versione kernel).
 * 
 * @param action Azione richiesta (ACTION_GET, ecc.)
 * @param param Parametro (non usato per ora)
 * @return Versione del kernel (2026) o 0
 */
uint32_t handle_system_service(uint32_t action, uint32_t param);

/**
 * @brief Gestisce le chiamate della categoria CAP_LOGGING.
 * 
 * Azioni supportate: ACTION_SEND (stampa su UART), ACTION_RECEIVE (lettura UART).
 * 
 * @param action Azione richiesta (ACTION_SEND o ACTION_RECEIVE)
 * @param buffer Per SEND: stringa da stampare; per RECEIVE: buffer dove scrivere
 * @param size Per RECEIVE: dimensione massima del buffer; per SEND: non usato
 * @param unused Non utilizzato (per compatibilità con firma)
 * @return Per SEND: 1 se successo; per RECEIVE: numero di byte letti (senza terminatore)
 */
uint32_t handle_logging_service(uint32_t action, char* buffer, uint32_t size, uint32_t unused);

/**
 * @brief Gestisce le chiamate della categoria CAP_SENSORS.
 * 
 * Azioni supportate: ACTION_GET (lettura sensori simulati).
 * 
 * @param action Azione richiesta (ACTION_GET)
 * @param sensor_id ID del sensore: 1 = temperatura, 2 = batteria
 * @return Valore simulato (25 per temp, 80 per batteria) o 0
 */
uint32_t handle_sensor_service(uint32_t action, uint32_t sensor_id);

/**
 * @brief Gestisce le chiamate della categoria CAP_STORAGE.
 * 
 * Azioni supportate: CREATE, WRITE, READ, DELETE.
 * I parametri sono passati tramite una struttura nella RAM della VM.
 * 
 * @param action Azione richiesta (ACTION_STORAGE_*)
 * @param p1 Offset nella RAM della VM della struttura storage_params_t
 * @param p2 Non usato (riservato)
 * @param ram_base Puntatore alla RAM della VM (per risolvere gli offset)
 * @return Per READ: numero di byte letti; per le altre: 0 se successo, 0xFFFFFFFF se errore
 */
uint32_t handle_storage_service(uint32_t action, uint32_t p1, uint32_t p2, uint8_t* ram_base);

/**
 * @brief Inizializza il sottosistema di storage.
 * 
 * Crea un RAM disk, lo registra nel device manager, monta ramfs2 su di esso
 * e lo registra nella mount table con nome "ram0".
 * Deve essere chiamata una volta all'avvio del kernel, dopo mem_init() e prima di scheduler_init().
 */
void storage_init(void);

#endif /* KERNEL_SERVICES_H */