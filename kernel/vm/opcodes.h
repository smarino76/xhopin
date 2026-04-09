/**
 * @file opcodes.h
 * @author Santiago Marino
 * @year 2026
 * @brief Definizione dei codici operativi (opcodes) per la Virtual Machine.
 * 
 * Questo file contiene le costanti che rappresentano tutte le istruzioni
 * supportate dalla VM. Ogni istruzione è un byte (0x00-0xFF) e viene
 * utilizzata dall'interprete in vm_core.c per decodificare il bytecode.
 * 
 * Le istruzioni sono organizzate per categorie:
 * - Aritmetiche (ADD, SUB, ADDI)
 * - Logiche (AND, OR, MOV, LUI, XOR, NOR)
 * - Shift (SLL, SRL, SRA)
 * - Memoria (LOAD, STORE, LOADB, STOREB)
 * - Controllo di flusso (BEQ, BNE, SLT, JMP, CALL, RET)
 * - Sistema (VCALL, HALT)
 */

#ifndef OPCODES_H
#define OPCODES_H

/* ============================================================================
 * ISTRUZIONI ARITMETICHE
 * ============================================================================ */

/**
 * @def OP_ADD
 * @brief Addizione: Rd = Rs + Rt
 * Formato: [0x01] [rd] [rs] [rt]
 */
#define OP_ADD    0x01

/**
 * @def OP_SUB
 * @brief Sottrazione: Rd = Rs - Rt
 * Formato: [0x02] [rd] [rs] [rt]
 */
#define OP_SUB    0x02

/**
 * @def OP_ADDI
 * @brief Addizione con immediato: Rd = Rs + imm (16-bit signed)
 * Formato: [0x03] [rd] [rs] [imm_low] [imm_high]
 */
#define OP_ADDI   0x03

/* ============================================================================
 * ISTRUZIONI LOGICHE
 * ============================================================================ */

/**
 * @def OP_AND
 * @brief AND bitwise: Rd = Rs & Rt
 * Formato: [0x10] [rd] [rs] [rt]
 */
#define OP_AND    0x10

/**
 * @def OP_OR
 * @brief OR bitwise: Rd = Rs | Rt
 * Formato: [0x11] [rd] [rs] [rt]
 */
#define OP_OR     0x11

/**
 * @def OP_MOV
 * @brief Copia registro: Rd = Rs
 * Formato: [0x12] [rd] [rs] [0]
 */
#define OP_MOV    0x12

/**
 * @def OP_LUI
 * @brief Load Upper Immediate: Rd = imm << 16
 * Formato: [0x13] [rd] [0] [imm_high] [imm_low]
 */
#define OP_LUI    0x13

/**
 * @def OP_XOR
 * @brief XOR bitwise: Rd = Rs ^ Rt
 * Formato: [0x14] [rd] [rs] [rt]
 */
#define OP_XOR    0x14

/**
 * @def OP_NOR
 * @brief NOR bitwise: Rd = ~(Rs | Rt)
 * Formato: [0x15] [rd] [rs] [rt]
 */
#define OP_NOR    0x15

/* ============================================================================
 * ISTRUZIONI DI SHIFT
 * ============================================================================ */

/**
 * @def OP_SLL
 * @brief Shift Left Logical: Rd = Rt << shamt (zero fill)
 * Formato: [0x40] [rd] [0] [rt] [shamt]
 * shamt è un campo a 5 bit (0-31) nei bit 4..0 dell'ultimo byte.
 */
#define OP_SLL    0x40

/**
 * @def OP_SRL
 * @brief Shift Right Logical: Rd = Rt >> shamt (zero fill)
 * Formato: [0x41] [rd] [0] [rt] [shamt]
 */
#define OP_SRL    0x41

/**
 * @def OP_SRA
 * @brief Shift Right Arithmetic: Rd = Rt >> shamt (sign extend)
 * Formato: [0x42] [rd] [0] [rt] [shamt]
 */
#define OP_SRA    0x42

/* ============================================================================
 * ISTRUZIONI DI MEMORIA
 * ============================================================================ */

/**
 * @def OP_LOAD
 * @brief Carica una word (32 bit) dalla RAM: Rd = RAM[Rs + Rt]
 * Richiede allineamento a 4 byte.
 * Formato: [0x20] [rd] [rs] [rt]
 */
#define OP_LOAD   0x20

/**
 * @def OP_STORE
 * @brief Memorizza una word (32 bit) in RAM: RAM[Rd + Rs] = Rt
 * Richiede allineamento a 4 byte.
 * Formato: [0x21] [rd] [rs] [rt]
 */
#define OP_STORE  0x21

/**
 * @def OP_LOADB
 * @brief Carica un byte (zero-extended) dalla RAM: Rd = RAM[Rs + Rt]
 * Formato: [0x22] [rd] [rs] [rt]
 */
#define OP_LOADB  0x22

/**
 * @def OP_STOREB
 * @brief Memorizza un byte in RAM: RAM[Rd + Rs] = Rt & 0xFF
 * Formato: [0x23] [rd] [rs] [rt]
 */
#define OP_STOREB 0x23

/* ============================================================================
 * ISTRUZIONI DI CONTROLLO DI FLUSSO
 * ============================================================================ */

/**
 * @def OP_BEQ
 * @brief Branch if equal: if(Rd == Rs) PC += (imm << 2)
 * L'immediato è signed a 16 bit, interpretato come numero di istruzioni.
 * Formato: [0x30] [rd] [rs] [imm_low] [imm_high]
 */
#define OP_BEQ    0x30

/**
 * @def OP_BNE
 * @brief Branch if not equal: if(Rd != Rs) PC += (imm << 2)
 * Formato: [0x31] [rd] [rs] [imm_low] [imm_high]
 */
#define OP_BNE    0x31

/**
 * @def OP_SLT
 * @brief Set on less than: Rd = (Rs < Rt) ? 1 : 0
 * Formato: [0x32] [rd] [rs] [rt]
 */
#define OP_SLT    0x32

/**
 * @def OP_JMP
 * @brief Jump assoluto: PC = target (24 bit)
 * Formato: [0x33] [target_23:16] [target_15:8] [target_7:0]
 */
#define OP_JMP    0x33

/**
 * @def OP_CALL
 * @brief Chiamata a sottoprogramma: RA = PC + 4, PC = target (24 bit)
 * Formato: [0x34] [target_23:16] [target_15:8] [target_7:0]
 */
#define OP_CALL   0x34

/**
 * @def OP_RET
 * @brief Ritorno da sottoprogramma: PC = RA
 * Formato: [0x35] [0] [0] [0]
 */
#define OP_RET    0x35

/* ============================================================================
 * ISTRUZIONI DI SISTEMA
 * ============================================================================ */

/**
 * @def OP_VCALL
 * @brief Chiamata al kernel (system call)
 * I parametri sono passati nei registri a0, a1, a2, v0.
 * Formato: [0x50] [0] [0] [0]
 */
#define OP_VCALL  0x50

/**
 * @def OP_HALT
 * @brief Termina l'esecuzione della VM
 * Formato: [0xFF] [0] [0] [0]
 */
#define OP_HALT   0xFF

#endif /* OPCODES_H */