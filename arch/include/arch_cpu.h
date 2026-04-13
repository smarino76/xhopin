#ifndef ARCH_CPU_H
#define ARCH_CPU_H

#include <stdint.h>

/* ===== Tipi ===== */
typedef struct CpuContext CpuContext;

/* ===== Context ===== */
void cpu_context_save(CpuContext *ctx);
void cpu_context_restore(const CpuContext *ctx);

/* ===== Task context init ===== */
void cpu_context_init(
    CpuContext *ctx,
    void (*entry)(void *),
    void *arg,
    void *stack_top,
    void (*exit_handler)(void)
);

/* ===== Stack ===== */
void cpu_set_sp(void *sp);
void* cpu_get_sp(void);

/* ===== CPU control ===== */
void cpu_idle(void);
void cpu_irq_disable(void);
void cpu_irq_enable(void);

/* ===== Atomic operations ===== */
uint32_t cpu_irq_save(void);      /* salva lo stato e disabilita interrupt */
void cpu_irq_restore(uint32_t flags); /* ripristina lo stato */

#endif /* ARCH_CPU_H */