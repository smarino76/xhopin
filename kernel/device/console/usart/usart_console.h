/**
 * @file uart_console.h
 * @author Santiago Marino
 * @year 2026
 * @brief Console basata su UART (seriale).
 */

#ifndef UART_CONSOLE_H
#define UART_CONSOLE_H

/**
 * @brief Inizializza la console UART e la registra nel sistema.
 * 
 * Deve essere chiamata dopo che la UART è stata inizializzata e aperta.
 * Di solito in main() dopo arch_uart_init() e arch_uart_open().
 */
void uart_console_init(void);

#endif