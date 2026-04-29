.include "arch/mips/pic32config.inc"
    
#ifdef PIC32MX
# Definizione registro atomico CLEAR per IFS0 (PIC32MX470)
# IFS0 = 0xBF881030 -> IFS0CLR = 0xBF881034    
.equ IFS0CLR,            0xBF881034  
.equ CTIF_BIT,           0           /* Bit 0 = Core Timer Interrupt Flag */
.equ KERNEL_STACK_SIZE,  256         
.equ STATUS_CLEAR_MASK,  0xFFFFF8F9  /* Maschera per pulire EXL, ERL e IPL */
#endif

#ifdef PIC32MZ
# Definizione registro atomico CLEAR per IFS0 (PIC32MZ)
# IFS0 = 0xBF881030 -> IFS0CLR = 0xBF881044    
.equ IFS0CLR,            0xBF810044  
.equ CTIF_BIT,           0           /* Bit 0 = Core Timer Interrupt Flag */
.equ KERNEL_STACK_SIZE,  256         
.equ STATUS_CLEAR_MASK,  0xFFFFF8F9  /* Maschera per pulire EXL, ERL e IPL */
#endif

    
    
.text
.set noreorder
.set noat

.global core_timer_isr
.ent core_timer_isr

core_timer_isr:
    /* 1. Puntiamo al TCB (Task Control Block) usando k0 */
    la      $k0, current_task
    lw	    $k0, 0($k0)		    # $k0 = current_task (Task*)
#    lw      $k0, 0($k0)		    # $k0 = current_task->context (CpuContext*)
    
    # Se il puntatore è NULL (avvio sistema), saltiamo il salvataggio
    beqz    $k0, setup_kernel_stack
    nop
    nop
    nop

    /* 2. SALVATAGGIO CONTESTO (32 registri + Speciali) */
   # la      $k0, current_task	    # se siamo arrivati qui è perche current_task
   # lw	    $k0, 0($k0)		    # $k0 = current_task (Task*)
    lw      $k0, 0($k0)		    # $k0 = current_task->context (CpuContext*)
    sw      $ra,  0($k0)
    sw      $sp,  4($k0)         # SP originale del task
    sw      $gp,  8($k0)
    sw      $s0, 12($k0)
    sw      $s1, 16($k0)
    sw      $s2, 20($k0)
    sw      $s3, 24($k0)
    sw      $s4, 28($k0)
    sw      $s5, 32($k0)
    sw      $s6, 36($k0)
    sw      $s7, 40($k0)
    sw      $a0, 44($k0)
    sw      $a1, 48($k0)
    sw      $a2, 52($k0)
    sw      $a3, 56($k0)
    sw      $v0, 72($k0)
    sw      $v1, 76($k0)
    sw      $t0, 84($k0)
    sw      $t1, 88($k0)
    sw      $t2, 92($k0)
    sw      $t3, 96($k0)
    sw      $t4, 100($k0)
    sw      $t5, 104($k0)
    sw      $t6, 108($k0)
    sw      $t7, 112($k0)
    sw      $t8, 116($k0)
    sw      $t9, 120($k0)
    sw      $fp, 68($k0)         # Registro s8

    # Registri speciali HI/LO (usiamo k1 come ponte)
    mfhi    $k1
    ehb
    sw      $k1, 124($k0)
    mflo    $k1
    ehb
    nop
    nop
    nop
    sw      $k1, 128($k0)

    # Status: salviamo ESATTAMENTE lo stato corrente (incluso EXL=1)
    mfc0    $k1, $12
    ehb
    nop
    nop
    nop
    sw      $k1, 60($k0)

    # EPC: Indirizzo di rientro del task
    mfc0    $k1, $14
    ehb
    nop
    nop
    nop
    sw      $k1, 64($k0)

setup_kernel_stack:
    /* 3. Passaggio allo stack del Kernel */
    la      $k1, kernel_stack
    addiu   $sp, $k1, KERNEL_STACK_SIZE - 16

    /* 4. Pulisci il flag interrupt (IFS0CLR) */
    li      $k1, (1 << CTIF_BIT)
    la      $k0, IFS0CLR
    sw      $k1, 0($k0)

    /* 5. NUOVO: Incrementa Compare invece di resettare Count
     *    Chiama arch_systick_get_instance() per ottenere il puntatore
     *    Poi chiama arch_systick_tick() per incrementare Compare
     *    Questa è la chiave per la misurazione del CPU usage
     */
    jal     arch_systick_get_instance
    nop
    move    $a0, $v0              # $a0 = puntatore a ArchSysTick
    jal     arch_systick_tick
    nop

    /* 6. Chiamata allo Scheduler C (timer_callback) */
    jal     timer_callback
    ehb
    nop
    nop
    nop

//    /* 7. Salto al ripristino (Tail-Call) */
//    la      $a0, current_task
//    lw      $a0, 0($a0)          # Carica il TCB scelto
//lw      $a0, 0($a0)          # $a0 = current_task->context (CpuContext*)    
//    j       cpu_context_restore  
//    nop

.end core_timer_isr