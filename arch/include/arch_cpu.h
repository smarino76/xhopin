#ifndef ARCH_CPU_H
#define ARCH_CPU_H

#include <stdint.h>

/* ===== Tipi ===== */
typedef struct CpuContext {
    uint32_t ra; //offset 0
    uint32_t sp; //offset 4
    uint32_t gp; //offset 8
    uint32_t s0; //offset 12
    uint32_t s1; //offset 16
    uint32_t s2; //offset 20
    uint32_t s3; //offset 24
    uint32_t s4; //offset 28
    uint32_t s5; //offset 32
    uint32_t s6; //offset 36
    uint32_t s7; //offset 40
    uint32_t a0; //offset 44
    uint32_t a1; //offset 48
    uint32_t a2; //offset 52
    uint32_t a3; //offset 56
    uint32_t status; //offset 60 
    uint32_t epc; //offset 64
    uint32_t fp; //offset 68
    uint32_t v0; // offset 72
    uint32_t v1; // offset 76
    uint32_t at; // offset 80
    uint32_t t0; //offset 84
    uint32_t t1; //offset 88
    uint32_t t2; //offset 92
    uint32_t t3; //offset 96
    uint32_t t4; //offset 100
    uint32_t t5; //offset 104
    uint32_t t6; //offset 108
    uint32_t t7; //offset 112
    uint32_t t8; //offset 116
    uint32_t t9; //offset 120
    uint32_t hi; //offset 124
    uint32_t lo; //offset 128
} CpuContext;

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