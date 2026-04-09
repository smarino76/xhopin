/**
 * @file vm_types.h
 * @author Santiago Marino
 * @year 2026
 * @brief Definizioni dei tipi base per la Virtual Machine.
 * 
 * Questo file contiene le strutture dati fondamentali della VM,
 * i tipi per gli intent, le costanti per la gestione delle versioni dell'ABI,
 * e il contesto runtime di una VM.
 * 
 * MODIFICHE RISPETTO ALLA VERSIONE PRECEDENTE:
 * - Aggiunto il campo `ref_count` in `VMContext` per il reference counting,
 *   necessario per prevenire use?after?free quando servizi asincroni (futuri)
 *   detengono riferimenti a VM che potrebbero essere terminate.
 */

#ifndef VM_TYPES_H
#define VM_TYPES_H

#include <stdint.h>
/* ============================================================================
 * VERSIONI DELL'ABI DELLA VM
 * ============================================================================ */

/**
 * @def VM_ABI_VERSION
 * @brief Versione corrente dell'ABI supportata dal kernel.
 */
#define VM_ABI_VERSION     1

/**
 * @def VM_ABI_MIN_VERSION
 * @brief Versione minima dell'ABI ancora supportata.
 */
#define VM_ABI_MIN_VERSION 1

/* ============================================================================
 * CATEGORIE DI INTENT (CAPABILITIES)
 * ============================================================================ */

/**
 * @enum IntentCategory
 * @brief Maschera di bit che definisce i permessi di un'applicazione.
 */
typedef enum {
    CAP_SYSTEM  = (1 << 0),   /**< Servizi di sistema (caricamento app, stop) */
    CAP_LOGGING = (1 << 1),   /**< Console (input/output testuale) */
    CAP_SENSORS = (1 << 2),   /**< Lettura sensori simulati */
    CAP_STORAGE = (1 << 3),   /**< Filesystem (create, read, write, delete) */
    CAP_COMPILER = (1 << 4)   /**< Assemblatore residente (non ancora implementato) */
} IntentCategory;

/**
 * @enum IntentAction
 * @brief Operazione specifica richiesta all'interno di una categoria.
 */
typedef enum {
    ACTION_GET              = 0,  /**< Richiedi un dato/stato */
    ACTION_SEND             = 1,  /**< Invia dati (es. stampa su console) */
    ACTION_LOAD_APP         = 2,  /**< Carica una nuova app (crea VM e task) */
    ACTION_STOP             = 3,  /**< Termina un task */
    ACTION_STORAGE_CREATE   = 4,  /**< Crea un file vuoto */
    ACTION_STORAGE_WRITE    = 5,  /**< Scrive dati in un file */
    ACTION_STORAGE_READ     = 6,  /**< Legge dati da un file */
    ACTION_STORAGE_DELETE   = 7,  /**< Cancella un file */
    ACTION_STORAGE_LIST     = 8,  /**< Elenca file (stub) */
    ACTION_RECEIVE          = 9,  /**< Riceve dati dall'esterno (es. lettura console) */
    ACTION_ASSEMBLE         = 10  /**< Assembla un file sorgente (non implementato) */
} IntentAction;

/**
 * @enum VMState
 * @brief Stati operativi di una macchina virtuale.
 */
typedef enum {
    VM_STATE_READY,     /**< VM creata, pronta per essere eseguita */
    VM_STATE_RUNNING,   /**< VM in esecuzione attiva */
    VM_STATE_HALTED,    /**< VM terminata volontariamente (OP_HALT) o in attesa di distruzione */
    VM_STATE_ERROR      /**< VM bloccata per errore (violazione memoria o istruzione illegale) */
} VMState;

/**
 * @struct VM_Registers
 * @brief Banco di 32 registri a 32 bit (architettura MIPS?like).
 */
typedef struct {
    union {
        uint32_t gp[32];
        struct {
            uint32_t zero, at, v0, v1, a0, a1, a2, a3,
                     t0, t1, t2, t3, t4, t5, t6, t7,
                     s0, s1, s2, s3, s4, s5, s6, s7,
                     t8, t9, k0, k1, gp, sp, fp, ra;
        } named;
    };
} VM_Registers;

/**
 * @struct AppHeader
 * @brief Intestazione di un'applicazione (presente all'inizio del file binario).
 */
typedef struct {
    uint32_t magic;          /**< Identificatore magico: 0xCAFEFACE */
    char     name[16];       /**< Nome dell'applicazione (fino a 15 caratteri + null) */
    uint32_t version;        /**< Versione dell'ABI usata dall'app */
    uint32_t code_size;      /**< Dimensione del bytecode (in byte) */
    uint32_t ram_size;       /**< Dimensione richiesta per la RAM virtuale (sandbox) */
    uint32_t entry_point;    /**< Offset della prima istruzione nel code_buffer */
    uint32_t capabilities;   /**< Maschera di IntentCategory (permessi richiesti) */
    uint32_t checksum;       /**< Checksum su tutto il binario (header+code) per integrità */
} AppHeader;

/**
 * @struct VMContext
 * @brief Struttura che rappresenta una macchina virtuale in esecuzione.
 * 
 * Ogni VM ha il proprio Program Counter, banco registri, buffer di codice
 * (condiviso, read?only), buffer di RAM isolato (sandbox), stato e capabilities.
 * 
 * Il campo `ref_count` viene utilizzato per tracciare quanti riferimenti attivi
 * puntano a questa VM (es. da parte di servizi asincroni). Una VM può essere
 * distrutta solo quando `ref_count == 0` e lo stato è HALTED o ERROR.
 */
typedef struct {
    uint32_t      pc;           /**< Program Counter corrente (offset in byte) */
    VM_Registers  regs;         /**< Banco registri */
    uint8_t      *code_buffer;  /**< Puntatore fisico al bytecode (read?only) */
    uint32_t      code_size;    /**< Dimensione del bytecode */
    uint8_t      *ram_buffer;   /**< Puntatore alla RAM virtuale isolata */
    uint32_t      ram_size;     /**< Dimensione della RAM virtuale (in byte) */
    VMState       state;        /**< Stato corrente della VM */
    uint32_t      capabilities; /**< Permessi effettivi (copiati dal manifest) */
    char          name[16];     /**< Nome dell'app (copia dal manifest) */
    int           ref_count;    /**< Contatore di riferimenti (per servizi asincroni) */
} VMContext;

#endif /* VM_TYPES_H */