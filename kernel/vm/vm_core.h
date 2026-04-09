/**
 * @file vm_core.h
 * @author Santiago Marino
 * @year 2026
 * @brief Prototipi delle funzioni principali della VM.
 * 
 * Questo file dichiara le funzioni per creare, distruggere, eseguire un passo,
 * il loop principale di una VM, e le funzioni di reference counting.
 * 
 * MODIFICHE RISPETTO ALLA VERSIONE PRECEDENTE:
 * - Aggiunte le funzioni `vm_ref_inc` e `vm_ref_dec` per gestire i riferimenti
 *   a VM da parte di servizi asincroni (futuri).
 * - `vm_destroy` ora rispetta il contatore di riferimenti.
 */

#ifndef VM_CORE_H
#define VM_CORE_H

#include "vm_types.h"

/**
 * @brief Crea una nuova VM a partire da un header e dal bytecode.
 * 
 * Alloca la struttura VMContext, la RAM virtuale, inizializza i registri,
 * lo stato e imposta `ref_count = 1` (il task che la eseguirà tiene un riferimento).
 * 
 * @param header Puntatore all'AppHeader (deve essere già validato)
 * @param code_ptr Puntatore al bytecode (subito dopo l'header)
 * @return Puntatore alla VM creata, o NULL in caso di errore (memoria insufficiente)
 */
VMContext* vm_create(AppHeader *header, uint8_t *code_ptr);

/**
 * @brief Distrugge una VM, ma solo se il contatore di riferimenti è zero.
 * 
 * Se `ref_count > 0`, la VM viene solo marcata come HALTED e la distruzione
 * viene posticipata fino a quando tutti i riferimenti saranno rilasciati.
 * 
 * @param vm Puntatore alla VM da distruggere (può essere NULL)
 */
void vm_destroy(VMContext *vm);

/**
 * @brief Esegue una singola istruzione della VM.
 * 
 * Legge l'istruzione all'indirizzo PC, la decodifica e la esegue.
 * Aggiorna i registri, la RAM e il PC. In caso di errore, imposta stato ERROR.
 * 
 * @param vm Puntatore alla VM in esecuzione
 */
void vm_step(VMContext *vm);

/**
 * @brief Loop principale di esecuzione della VM.
 * 
 * Imposta lo stato a RUNNING e chiama vm_step() in ciclo finché lo stato
 * rimane RUNNING. Questa funzione è tipicamente il punto di ingresso di un task.
 * 
 * @param vm Puntatore alla VM da eseguire (passato come argomento del task)
 */
void vm_interpreter_loop(VMContext *vm);

/**
 * @brief Incrementa il contatore di riferimenti della VM.
 * 
 * Deve essere chiamato ogni volta che una parte del kernel (es. un servizio asincrono)
 * prende un riferimento alla VM per uso futuro (es. scrivere un risultato dopo
 * che l'operazione asincrona è completata).
 * 
 * @param vm Puntatore alla VM
 */
void vm_ref_inc(VMContext *vm);

/**
 * @brief Decrementa il contatore di riferimenti della VM.
 * 
 * Se il contatore diventa zero e la VM è in stato HALTED o ERROR,
 * la VM viene immediatamente distrutta (libera la RAM e la struttura).
 * 
 * @param vm Puntatore alla VM
 */
void vm_ref_dec(VMContext *vm);

#endif /* VM_CORE_H */