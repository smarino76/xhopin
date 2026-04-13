#include "arch/arch.h"
#include "pic32mx_cpu.h"
#include "arch/include/arch_cpu.h"
#include <stddef.h>
/* =====================================================================================
 * CONTEXT MANAGEMENT
 * ===================================================================================== */
#if defined(__32MX470F512H_HARMONY__)
uint32_t save_t0;
uint32_t save_t1;
uint32_t save_t2;
uint32_t save_v0;
uint32_t save_v1;

void cpu_context_init(
    CpuContext *ctx,
    void (*entry)(void *),
    void *arg,
    void *stack_top,
    void (*exit_handler)(void)
)
{
    ctx->sp = (uint32_t)stack_top;
    ctx->ra = (uint32_t)exit_handler;
    ctx->epc = (uint32_t)entry;
    ctx->a0 = (uint32_t)arg;
    ctx->a1 = ctx->a2 = ctx->a3 = 0;

    __asm__ volatile ("move %0, $gp" : "=r"(ctx->gp));

    uint32_t status;
    __asm__ volatile ("mfc0 %0, $12" : "=r"(status));
    ctx->status = status | 0x1;  /* IE bit */

    ctx->s0 = ctx->s1 = ctx->s2 = ctx->s3 = ctx->s4 = ctx->s5 = ctx->s6 = ctx->s7 = 0;
}
#endif