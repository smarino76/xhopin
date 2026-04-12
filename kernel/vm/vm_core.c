/**
 * @file vm_core.c
 * @author Santiago Marino
 * @year 2026
 * @brief Implementazione dell'interprete della Virtual Machine.
 * 
 * Contiene il cuore della VM: creazione, distruzione, ciclo fetch?decode?execute,
 * e la gestione delle istruzioni. Supporta tutte le istruzioni definite in opcodes.h.
 * 
 * MODIFICHE RISPETTO ALLA VERSIONE PRECEDENTE:
 * - Implementate le funzioni `vm_ref_inc`, `vm_ref_dec`.
 * - `vm_destroy` ora verifica `ref_count` prima di liberare la memoria.
 * - `vm_create` inizializza `ref_count = 1` e lo stack pointer (sp) alla fine della RAM.
 * - `OP_LOAD` e `OP_STORE` ora usano l'offset immediato (imm) invece del registro rt.
 */

#include "vm_core.h"
#include "opcodes.h"
#include "kernel/memory/mem_alloc.h"
#include <string.h>

/* Dichiarazione esterna del dispatcher degli intent (definito in kernel_intent_dispatcher.c) */
extern uint32_t kernel_intent_dispatcher(VMContext *vm);

/* ----------------------------------------------------------------------------
 * vm_create
 * -------------------------------------------------------------------------- */
VMContext* vm_create(AppHeader *header, uint8_t *code_ptr) {
    /* Alloca la struttura VMContext */
    VMContext *vm = (VMContext *) mem_alloc(sizeof (VMContext));
    if (!vm) return NULL;

    /* Azzera l'intera struttura */
    memset(vm, 0, sizeof (VMContext));

    /* Copia il nome dell'app (massimo 15 caratteri + terminatore) */
    strncpy(vm->name, header->name, sizeof (vm->name) - 1);
    vm->name[sizeof (vm->name) - 1] = '\0';

    /* Inizializza i campi base */
    vm->code_buffer = code_ptr;
    vm->code_size = header->code_size;
    vm->capabilities = header->capabilities;
    vm->pc = header->entry_point;
    vm->state = VM_STATE_READY;
    vm->ref_count = 1; /* Il task che esegue la VM tiene un riferimento */

    /* Allinea la RAM a 4 byte per evitare eccezioni di allineamento su MIPS */
    uint32_t aligned_ram = (header->ram_size + 3) & ~3;
    vm->ram_size = aligned_ram;
    if (aligned_ram > 0) {
        vm->ram_buffer = (uint8_t *) mem_alloc(aligned_ram);
        if (!vm->ram_buffer) {
            mem_free(vm);
            return NULL;
        }
        memset(vm->ram_buffer, 0, aligned_ram);
    }

    /* Inizializza lo stack pointer alla fine della RAM */
    vm->regs.named.sp = aligned_ram - 4;

    return vm;
}

/* ----------------------------------------------------------------------------
 * vm_ref_inc
 * Incrementa il contatore di riferimenti.
 * Usato da servizi asincroni che devono mantenere la VM viva oltre la vita del task.
 * -------------------------------------------------------------------------- */
void vm_ref_inc(VMContext *vm) {
    if (vm) {
        vm->ref_count++;
    }
}

/* ----------------------------------------------------------------------------
 * vm_ref_dec
 * Decrementa il contatore di riferimenti.
 * Se il contatore arriva a zero e la VM è in stato HALTED o ERROR, la distrugge.
 * -------------------------------------------------------------------------- */
void vm_ref_dec(VMContext *vm) {
    if (!vm) return;
    if (vm->ref_count > 0) {
        vm->ref_count--;
    }
    /* Se il contatore arriva a zero e la VM è in stato terminale, la distrugge */
    if (vm->ref_count == 0 && (vm->state == VM_STATE_HALTED || vm->state == VM_STATE_ERROR)) {
        vm_destroy(vm);
    }
}

/* ----------------------------------------------------------------------------
 * vm_destroy
 * Distrugge la VM solo se ref_count == 0, altrimenti la marca come HALTED.
 * -------------------------------------------------------------------------- */
void vm_destroy(VMContext *vm) {
    if (!vm) return;
    /* Se ci sono ancora riferimenti attivi, non distruggere, ma marcare come HALTED */
    if (vm->ref_count > 0) {
        vm->state = VM_STATE_HALTED; /* In attesa che i riferimenti vengano rilasciati */
        return;
    }
    /* Distruzione effettiva */
    if (vm->ram_buffer) mem_free(vm->ram_buffer);
    mem_free(vm);
}

/* ----------------------------------------------------------------------------
 * vm_step - Esecuzione di una singola istruzione
 * -------------------------------------------------------------------------- */
void vm_step(VMContext *vm) {
    /* Se la VM non è in esecuzione, non fare nulla */
    if (vm->state != VM_STATE_RUNNING) return;

    /* Protezione: verifica che il PC non esca dal codice */
    if (vm->pc + 4 > vm->code_size) {
        vm->state = VM_STATE_ERROR;
        return;
    }

    /* FETCH: carica l'istruzione a 32 bit (big-endian) */
    uint32_t instr = *(uint32_t *) (vm->code_buffer + vm->pc);
    uint8_t opcode = (instr >> 24) & 0xFF; /* Primo byte = opcode */
    uint8_t rd = (instr >> 16) & 0xFF; /* Secondo byte = registro destinazione */
    uint8_t rs = (instr >> 8) & 0xFF; /* Terzo byte = primo operando */
    uint8_t rt = instr & 0xFF; /* Quarto byte = secondo operando o immediate */
    int16_t imm = (int16_t) (instr & 0xFFFF); /* Immediate a 16 bit con segno */

    int pc_jumped = 0; /* Flag: true se l'istruzione modifica il PC (salto) */

    /* DECODE-EXECUTE */
    switch (opcode) {
            /* ===== Aritmetica ===== */
        case OP_ADD: vm->regs.gp[rd] = vm->regs.gp[rs] + vm->regs.gp[rt];
            break;
        case OP_SUB: vm->regs.gp[rd] = vm->regs.gp[rs] - vm->regs.gp[rt];
            break;
        case OP_ADDI:
        {
            int8_t imm8 = (int8_t) rt; /* imm a 8 bit con segno */
            vm->regs.gp[rd] = vm->regs.gp[rs] + imm8;
            break;
        }

            /* ===== Logiche ===== */
        case OP_AND: vm->regs.gp[rd] = vm->regs.gp[rs] & vm->regs.gp[rt];
            break;
        case OP_OR: vm->regs.gp[rd] = vm->regs.gp[rs] | vm->regs.gp[rt];
            break;
        case OP_MOV: vm->regs.gp[rd] = vm->regs.gp[rs];
            break;
        case OP_LUI: vm->regs.gp[rd] = (uint32_t) imm << 16;
            break;
        case OP_XOR: vm->regs.gp[rd] = vm->regs.gp[rs] ^ vm->regs.gp[rt];
            break;
        case OP_NOR: vm->regs.gp[rd] = ~(vm->regs.gp[rs] | vm->regs.gp[rt]);
            break;

            /* ===== Shift ===== */
        case OP_SLL:
        {
            uint8_t shamt = instr & 0x1F; /* shift amount nei bit 4..0 */
            vm->regs.gp[rd] = vm->regs.gp[rt] << shamt;
            break;
        }
        case OP_SRL:
        {
            uint8_t shamt = instr & 0x1F;
            vm->regs.gp[rd] = vm->regs.gp[rt] >> shamt;
            break;
        }
        case OP_SRA:
        {
            uint8_t shamt = instr & 0x1F;
            int32_t val = (int32_t) vm->regs.gp[rt];
            vm->regs.gp[rd] = val >> shamt;
            break;
        }

            /* ===== Memoria word (allineata) - CON OFFSET IMMEDIATO ===== */
        case OP_LOAD:
        {
            uint32_t addr = vm->regs.gp[rs] + imm;
            if ((addr & 3) == 0 && (addr + 4 <= vm->ram_size))
                vm->regs.gp[rd] = *(uint32_t*) (vm->ram_buffer + addr);
            else vm->state = VM_STATE_ERROR;
            break;
        }
        case OP_STORE:
        {
            uint32_t addr = vm->regs.gp[rs] + imm;
            if ((addr & 3) == 0 && (addr + 4 <= vm->ram_size))
                *(uint32_t*) (vm->ram_buffer + addr) = vm->regs.gp[rd];
            else vm->state = VM_STATE_ERROR;
            break;
        }

            /* ===== Memoria byte (non allineata) ===== */
        case OP_LOADB:
        {
            uint32_t addr = vm->regs.gp[rs] + vm->regs.gp[rt];
            if (addr < vm->ram_size)
                vm->regs.gp[rd] = vm->ram_buffer[addr];
            else vm->state = VM_STATE_ERROR;
            break;
        }
        case OP_STOREB:
        {
            uint32_t addr = vm->regs.gp[rd] + vm->regs.gp[rs];
            if (addr < vm->ram_size)
                vm->ram_buffer[addr] = vm->regs.gp[rt] & 0xFF;
            else vm->state = VM_STATE_ERROR;
            break;
        }

            /* ===== Confronti e salti ===== */
        case OP_SLT:
            vm->regs.gp[rd] = (vm->regs.gp[rs] < vm->regs.gp[rt]) ? 1 : 0;
            break;
        case OP_BEQ:
            if (vm->regs.gp[rd] == vm->regs.gp[rs]) {
                vm->pc += (imm << 2);
                pc_jumped = 1;
            }
            break;
        case OP_BNE:
            if (vm->regs.gp[rd] != vm->regs.gp[rs]) {
                vm->pc += (imm << 2);
                pc_jumped = 1;
            }
            break;
        case OP_JMP:
            vm->pc = (instr & 0x00FFFFFF);
            pc_jumped = 1;
            break;
        case OP_CALL:
            vm->regs.named.ra = vm->pc + 4;
            vm->pc = (instr & 0x00FFFFFF);
            pc_jumped = 1;
            break;
        case OP_RET:
            vm->pc = vm->regs.named.ra;
            pc_jumped = 1;
            break;

            /* ===== Chiamata al kernel ===== */
        case OP_VCALL:
            vm->regs.named.k0 = kernel_intent_dispatcher(vm);
            break;

            /* ===== Terminazione ===== */
        case OP_HALT:
            vm->state = VM_STATE_HALTED;
            return;

        default:
            vm->state = VM_STATE_ERROR;
            return;
    }

    /* Avanzamento PC se non è stato un salto */
    if (!pc_jumped && vm->state == VM_STATE_RUNNING)
        vm->pc += 4;

    /* Mantiene il registro zero sempre a 0 */
    vm->regs.named.zero = 0;
}

/* ----------------------------------------------------------------------------
 * vm_interpreter_loop
 * Loop principale di esecuzione. Chiamato dal task come funzione entry.
 * -------------------------------------------------------------------------- */
void vm_interpreter_loop(VMContext *vm) {
    if (!vm) return;
    vm->state = VM_STATE_RUNNING;
    while (vm->state == VM_STATE_RUNNING) {
        vm_step(vm);
    }
}