/**
 * @file task.h
 * @author Santiago Marino
 * @year 2026
 * @brief Definizioni per il task scheduler del kernel.
 * 
 * Questo file definisce la struttura Task e i prototipi delle funzioni
 * per la gestione dei task: creazione, distruzione, scheduler.
 * 
 * Le modifiche recenti includono:
 * - Il prototipo di task_destroy_by_id ora restituisce int (0 successo, -1 errore)
 *   per segnalare il tentativo di auto?terminazione.
 */

#ifndef TASK_H
#define TASK_H

#include "arch/include/arch_cpu.h"
#include <stdint.h>

/* Variabili globali dichiarate altrove */
extern struct Task *current_task;
extern uint8_t kernel_stack[];

/**
 * @struct Task
 * @brief Struttura che rappresenta un task (thread) nello scheduler.
 * 
 * Ogni task ha un contesto CPU (registri salvati), uno stack dedicato,
 * una funzione di ingresso, un argomento, un ID univoco, un puntatore
 * a dati privati (es. VMContext) e un puntatore per la lista circolare.
 */
typedef struct Task {
    CpuContext context;      /**< Contesto CPU salvato (registri) */
    void *stack;             /**< Base dello stack (indirizzo basso) */
    uint32_t stack_size;     /**< Dimensione dello stack in byte */
    void (*entry)(void *);   /**< Funzione di ingresso del task */
    void *arg;               /**< Argomento passato alla funzione entry */
    uint32_t id;             /**< Identificatore univoco del task */
    void *priv_data;         /**< Dati privati (es. VMContext) per associazione */
    struct Task *next;       /**< Puntatore al prossimo task nella lista circolare */
} Task;

/**
 * @brief Crea un nuovo task e lo aggiunge alla coda dei pronti.
 * 
 * Alloca stack e struttura Task, inizializza il contesto CPU con
 * cpu_context_init(), e inserisce il task nella lista round?robin.
 * 
 * @param entry Funzione di ingresso del task (void* -> void)
 * @param arg Argomento da passare a entry
 * @param stack_size Dimensione dello stack (in byte)
 * @return Puntatore al task creato, o NULL in caso di errore (memoria insufficiente)
 */
Task* task_create(void (*entry)(void *), void *arg, uint32_t stack_size);

/**
 * @brief Inizializza lo scheduler e crea il task idle.
 * 
 * Crea il task idle (chiama cpu_idle()), lo inserisce nella lista,
 * e abilita l'interrupt del core timer per avviare il multitasking.
 */
void scheduler_init(void);

/**
 * @brief Distrugge il task corrente.
 * 
 * Rimuove il task corrente dalla lista, libera lo stack e la struttura,
 * e se priv_data non è NULL, chiama vm_destroy().
 * Questa funzione è chiamata automaticamente quando un task termina
 * (tramite task_exit_handler).
 */
void task_destroy_current(void);

/**
 * @brief Distrugge un task dato il suo ID.
 * 
 * Cerca il task nella lista circolare, lo rimuove, libera le risorse,
 * notifica l'evento EVENT_TASK_TERMINATED e, se era il task corrente,
 * resetta current_task a NULL.
 * È thread-safe grazie alla disabilitazione degli interrupt.
 * 
 * @param id ID del task da terminare
 * @return 0 se successo, -1 se il task non esiste o se si tenta di uccidere
 *         il task corrente (auto?terminazione non permessa)
 */
int task_destroy_by_id(uint32_t id);

/**
 * @brief Yield volontario (non utilizzato).
 * 
 * Questo scheduler è solo preemptive (grazie al core timer),
 * quindi task_yield() non fa nulla. Mantenuta per compatibilità.
 */
void task_yield(void);

#endif /* TASK_H */