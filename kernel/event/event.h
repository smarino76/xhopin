#ifndef EVENT_H
#define EVENT_H


/**
 * @brief Tipi di evento riconosciuti dal kernel.
 * 
 * Estendibile: aggiungere nuovi valori prima di EVENT_COUNT.
 */
typedef enum {
    EVENT_DEVICE_ADDED,      /**< Un dispositivo è stato registrato */
    EVENT_DEVICE_REMOVED,    /**< Un dispositivo è stato rimosso */
    EVENT_TASK_TERMINATED,   /**< Un task è terminato (volontariamente o per eccezione) */
    EVENT_COUNT              /**< Numero totale di tipi di evento (usato per array statici) */
} event_type_t;

/**
 * @brief Prototipo di una funzione gestore di eventi.
 * 
 * @param event   Il tipo di evento che si è verificato.
 * @param data    Dati associati all'evento (es. puntatore al dispositivo).
 * @param ctx     Contesto privato passato al momento della registrazione.
 */
typedef void (*event_handler_t)(event_type_t event, void *data, void *ctx);

/**
 * @brief Registra un gestore per un certo tipo di evento.
 * 
 * @param event   Tipo di evento a cui si vuole essere notificati.
 * @param handler Funzione da chiamare quando l'evento si verifica.
 * @param ctx     Contesto privato passato al gestore.
 * @return 0 in caso di successo, -1 altrimenti (es. evento non valido, troppi handler).
 */
int event_register(event_type_t event, event_handler_t handler, void *ctx);

/**
 * @brief Rimuove un gestore precedentemente registrato.
 * 
 * @param event   Tipo di evento.
 * @param handler Funzione gestore.
 * @param ctx     Contesto privato.
 * @return 0 se rimosso con successo, -1 se non trovato.
 */
int event_unregister(event_type_t event, event_handler_t handler, void *ctx);

/**
 * @brief Innesca un evento (chiamata dal kernel).
 * 
 * L'evento viene accodato e sarà elaborato asincronamente dal dispatcher.
 * 
 * @param event   Tipo di evento.
 * @param data    Dati da passare ai gestori (puntatore a struttura persistente).
 */
void event_trigger(event_type_t event, void *data);

/**
 * @brief Avvia il sistema di eventi (crea il task dispatcher).
 * 
 * Deve essere chiamata una volta all'avvio del kernel.
 */
void event_system_init(void);

#endif /* EVENT_H */