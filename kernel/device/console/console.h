/**
 * @file console.h
 * @author Santiago Marino
 * @year 2026
 * @brief Astrazione della console di sistema (input/output testuale).
 * 
 * La console è un dispositivo che permette di scrivere e leggere stringhe.
 * Può essere implementata su UART, display, USB, ecc. Il kernel mantiene
 * una console attiva (default UART). Le applicazioni usano VCALL CAP_LOGGING
 * che viene reindirizzata alla console attiva.
 */

#ifndef CONSOLE_H
#define CONSOLE_H


#include "../../../kernel/device/kdevice.h"

/**
 * @struct console_dev
 * @brief Struttura che rappresenta un dispositivo console.
 * 
 * Estende device_t con un puntatore alle operazioni specifiche.
 */
typedef struct console_dev console_dev_t;

/**
 * @struct console_ops_t
 * @brief Operazioni che ogni console deve implementare.
 */
typedef struct {
    /**
     * @brief Scrive una stringa sulla console.
     * @param dev Puntatore al dispositivo console
     * @param buf Buffer da scrivere (non necessariamente null-terminato)
     * @param len Numero di byte da scrivere
     * @return Numero di byte effettivamente scritti, o -1 in caso di errore
     */
    int (*write)(console_dev_t *dev, const char *buf, size_t len);
    
    /**
     * @brief Legge una stringa dalla console.
     * @param dev Puntatore al dispositivo console
     * @param buf Buffer dove memorizzare i dati
     * @param max_len Dimensione massima del buffer
     * @return Numero di byte letti (escluso terminatore nullo), o -1 in caso di errore
     */
    int (*read)(console_dev_t *dev, char *buf, size_t max_len);
} console_ops_t;

/**
 * @struct console_dev
 * @brief Struttura concreta di un dispositivo console.
 */
struct console_dev {
    device_t dev_base;          /**< Base device (ereditarietà) */
    console_ops_t *ops;     /**< Operazioni specifiche */
    void *priv;             /**< Dati privati del driver */
};

/**
 * @brief Inizializza il sottosistema console.
 * 
 * Deve essere chiamata prima di qualsiasi altra funzione console.
 */
void console_init(void);

/**
 * @brief Registra un nuovo dispositivo console.
 * 
 * Il dispositivo viene aggiunto al device manager e, se è il primo,
 * viene automaticamente impostato come console attiva.
 * 
 * @param dev Dispositivo console da registrare
 * @return 0 in caso di successo, -1 altrimenti
 */
int console_register(console_dev_t *dev);

/**
 * @brief Cambia la console attiva.
 * 
 * @param name Nome del dispositivo console (es. "uart0", "vt100")
 * @return 0 se trovato e impostato, -1 altrimenti
 */
int console_set_active(const char *name);

/**
 * @brief Restituisce il dispositivo console attualmente attivo.
 * 
 * @return Puntatore alla console attiva, o NULL se nessuna console è registrata
 */
console_dev_t *console_get_active(void);

/**
 * @brief Scrive sulla console attiva.
 * 
 * @param buf Buffer da scrivere
 * @param len Lunghezza del buffer
 * @return Numero di byte scritti, o -1 se nessuna console attiva
 */
int console_write(const char *buf, size_t len);

/**
 * @brief Legge dalla console attiva.
 * 
 * @param buf Buffer per i dati
 * @param max_len Dimensione massima del buffer
 * @return Numero di byte letti, o -1 se nessuna console attiva
 */
int console_read(char *buf, size_t max_len);

#endif