#ifndef ARCH_BUS_H
#define ARCH_BUS_H

// modalità hardware
typedef enum {
    ARCH_BUS_MODE_MASTER = 0,
    ARCH_BUS_MODE_SLAVE
} arch_bus_mode_t;

// ruolo logico (estendibile)
typedef enum {
    ARCH_BUS_ROLE_CONTROLLER = 0,
    ARCH_BUS_ROLE_TARGET
} arch_bus_role_t;

// base comune
typedef struct {
    arch_bus_mode_t mode;
    arch_bus_role_t role;
} arch_bus_config_t;

#endif
