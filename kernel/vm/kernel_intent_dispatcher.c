/**
 * @file kernel_intent_dispatcher.c
 * @author Santiago Marino
 * @year 2026
 * @brief Dispatcher degli intent (VCALL).
 * 
 * Riceve la chiamata dalla VM, estrae categoria e azione dai registri,
 * verifica (implicitamente) le capabilities e chiama il servizio appropriato.
 * 
 * Modifiche recenti: gestisce il valore di ritorno di task_destroy_by_id
 * per restituire 0xFFFFFFFF in caso di errore (es. auto?terminazione).
 */

#include "kernel_services.h"
#include "kernel/loader/app_loader.h"
#include "kernel/scheduler/task.h"
#include <stdint.h>

/**
 * @brief Punto di ingresso per tutte le chiamate VCALL.
 * 
 * @param vm Puntatore al contesto della VM chiamante
 * @return Risultato dell'operazione (scritto in k0 della VM)
 */
uint32_t kernel_intent_dispatcher(VMContext *vm) {
    uint32_t action = vm->regs.named.v0;
    uint32_t category = vm->regs.named.a0;
    uint32_t p1 = vm->regs.named.a1;
    uint32_t p2 = vm->regs.named.a2;

    switch (category) {
        case CAP_SYSTEM:
            if (action == ACTION_LOAD_APP) {
                /* p1 = offset del binario nella RAM della VM, p2 = dimensione */
                uint8_t* bin_ptr = vm->ram_buffer + p1;
                return (uint32_t) kernel_load_app(bin_ptr, p2);
            } else if (action == ACTION_STOP) {
                /* p1 = ID del task da terminare */
                int ret = task_destroy_by_id(p1);
                return (ret == 0) ? 0 : 0xFFFFFFFF;
            }
            return handle_system_service(action, p1);

        case CAP_LOGGING:
            if (action == ACTION_SEND) {
                char* msg = (char*) (vm->ram_buffer + p1);
                return handle_logging_service(action, msg, 0, 0);
            } else if (action == ACTION_RECEIVE) {
                uint8_t* buf = vm->ram_buffer + p1;
                return handle_logging_service(action, (char*) buf, p2, 0);
            }
            return 0xFFFFFFFF;

        case CAP_SENSORS:
            return handle_sensor_service(action, p1);

        case CAP_STORAGE:
            return handle_storage_service(action, p1, p2, vm->ram_buffer);


        case CAP_MONITOR:
            if (action == ACTION_GET_CPU_USAGE) {
                /* p1 = task_id */
                uint64_t cycles = task_get_cycles(p1);
                /* Restituisce i cicli (split in due registri? o 32 bit) */
                return (uint32_t) (cycles & 0xFFFFFFFF);
            }
            return 0xFFFFFFFF;

        default:
            return 0xFFFFFFFF; /* Categoria non supportata o non autorizzata */
    }
}