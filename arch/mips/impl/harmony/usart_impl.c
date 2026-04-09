#include "arch/include/arch_usart.h"

#if defined(__32MX470F512H_HARMONY__)

/* Include solo le istanze abilitate in CONFIG_UART_MASK */
#if (CONFIG_UART_MASK & UART1_MASK)
    #include "peripheral/uart/plib_uart1.h"
#endif
#if (CONFIG_UART_MASK & UART2_MASK)
    #include "peripheral/uart/plib_uart2.h"
#endif
#if (CONFIG_UART_MASK & UART3_MASK)
    #include "peripheral/uart/plib_uart3.h"
#endif
#if (CONFIG_UART_MASK & UART4_MASK)
    #include "peripheral/uart/plib_uart4.h"
#endif

/* Storage per callback (una per istanza) */
static arch_uart_cb_t uart_callbacks[4];
static uintptr_t uart_contexts[4];

/* Wrapper generico: riceve l'ID dell'istanza come contesto */
static void uart_generic_cb(uintptr_t context)
{
    uint8_t id = (uint8_t)context;
    if (uart_callbacks[id])
        uart_callbacks[id](uart_contexts[id]);
}

/* ================================================================ */
/*  IMPLEMENTAZIONE                                                 */
/* ================================================================ */

void arch_uart_impl_init(void)
{
    /* Harmony già inizializza i moduli all'avvio; eventuali
       inizializzazioni aggiuntive possono essere aggiunte qui. */
}

int arch_uart_impl_open(uint8_t id, arch_uart_config_t *cfg)
{
    if (cfg == NULL)
        return -1;

    UART_SERIAL_SETUP setup;
    setup.baudRate = cfg->baudRate;

    switch (cfg->parity) {
        case 0: setup.parity = UART_PARITY_NONE; break;
        case 1: setup.parity = UART_PARITY_EVEN; break;
        case 2: setup.parity = UART_PARITY_ODD;  break;
        default: return -1;
    }

    if (cfg->dataBits == 8)
        setup.dataWidth = UART_DATA_8_BIT;
    else if (cfg->dataBits == 9)
        setup.dataWidth = UART_DATA_9_BIT;
    else
        return -1;

    if (cfg->stopBits == 1)
        setup.stopBits = UART_STOP_1_BIT;
    else if (cfg->stopBits == 2)
        setup.stopBits = UART_STOP_2_BIT;
    else
        return -1;

    switch (id) {
#if (CONFIG_UART_MASK & UART1_MASK)
        case 0:
            if (!HAS_UART(UART1_MASK)) return -1;
            if (!UART1_SerialSetup(&setup, UART1_FrequencyGet()))
                return -1;
            return 0;
#endif
#if (CONFIG_UART_MASK & UART2_MASK)
        case 1:
            if (!HAS_UART(UART2_MASK)) return -1;
            if (!UART2_SerialSetup(&setup, UART2_FrequencyGet()))
                return -1;
            return 0;
#endif
#if (CONFIG_UART_MASK & UART3_MASK)
        case 2:
            if (!HAS_UART(UART3_MASK)) return -1;
            if (!UART3_SerialSetup(&setup, UART3_FrequencyGet()))
                return -1;
            return 0;
#endif
#if (CONFIG_UART_MASK & UART4_MASK)
        case 3:
            if (!HAS_UART(UART4_MASK)) return -1;
            if (!UART4_SerialSetup(&setup, UART4_FrequencyGet()))
                return -1;
            return 0;
#endif
        default:
            return -1;
    }
}

int arch_uart_impl_read(uint8_t id, void *buffer, size_t size)
{
    switch (id) {
#if (CONFIG_UART_MASK & UART1_MASK)
        case 0:
            return UART1_Read(buffer, size) ? 0 : -1;
#endif
#if (CONFIG_UART_MASK & UART2_MASK)
        case 1:
            return UART2_Read(buffer, size) ? 0 : -1;
#endif
#if (CONFIG_UART_MASK & UART3_MASK)
        case 2:
            return UART3_Read(buffer, size) ? 0 : -1;
#endif
#if (CONFIG_UART_MASK & UART4_MASK)
        case 3:
            return UART4_Read(buffer, size) ? 0 : -1;
#endif
        default:
            return -1;
    }
}

int arch_uart_impl_write(uint8_t id, const void *buffer, size_t size)
{
    switch (id) {
#if (CONFIG_UART_MASK & UART1_MASK)
        case 0:
            return UART1_Write((void*)buffer, size) ? 0 : -1;
#endif
#if (CONFIG_UART_MASK & UART2_MASK)
        case 1:
            return UART2_Write((void*)buffer, size) ? 0 : -1;
#endif
#if (CONFIG_UART_MASK & UART3_MASK)
        case 2:
            return UART3_Write((void*)buffer, size) ? 0 : -1;
#endif
#if (CONFIG_UART_MASK & UART4_MASK)
        case 3:
            return UART4_Write((void*)buffer, size) ? 0 : -1;
#endif
        default:
            return -1;
    }
}

int arch_uart_impl_set_callback(uint8_t id, arch_uart_cb_t cb, uintptr_t ctx)
{
    if (id >= 4) return -1;

    uart_callbacks[id] = cb;
    uart_contexts[id] = ctx;

    switch (id) {
#if (CONFIG_UART_MASK & UART1_MASK)
        case 0:
            UART1_ReadCallbackRegister(uart_generic_cb, (uintptr_t)id);
            return 0;
#endif
#if (CONFIG_UART_MASK & UART2_MASK)
        case 1:
            UART2_ReadCallbackRegister(uart_generic_cb, (uintptr_t)id);
            return 0;
#endif
#if (CONFIG_UART_MASK & UART3_MASK)
        case 2:
            UART3_ReadCallbackRegister(uart_generic_cb, (uintptr_t)id);
            return 0;
#endif
#if (CONFIG_UART_MASK & UART4_MASK)
        case 3:
            UART4_ReadCallbackRegister(uart_generic_cb, (uintptr_t)id);
            return 0;
#endif
        default:
            return -1;
    }
}

#endif /* __32MX470F512H_HARMONY__ */