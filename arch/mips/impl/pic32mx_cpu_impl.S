    .text
    .set noreorder
    .set nomacro

/* ============================================================
 * cpu_context_save
 * Salva lo stato corrente della CPU nella struttura CpuContext
 * Input: $a0 = puntatore a CpuContext
 * Output: nessuno
 * ============================================================ */
//    .global cpu_context_save
//cpu_context_save:
//    move $t2, $a0                # ctx ? t2 (salva il puntatore)
//
//    # Salva i registri callee-saved e speciali
//   // sw   $ra,  0($t2)            # ra */ // $ra deve puntare a exit handler. a questo punto ra punta a switch_to_next sarebbe un errore salvarlo nel contesto?
//    sw   $sp,  4($t2)            # sp
//    sw   $gp,  8($t2)            # gp
//
//    sw   $s0, 12($t2)            # s0
//    sw   $s1, 16($t2)            # s1
//    sw   $s2, 20($t2)            # s2
//    sw   $s3, 24($t2)            # s3
//    sw   $s4, 28($t2)            # s4
//    sw   $s5, 32($t2)            # s5
//    sw   $s6, 36($t2)            # s6
//    sw   $s7, 40($t2)            # s7
//
//    # Salva i registri argomento (per il passaggio di parametri)
//    sw   $a0, 44($t2)            # a0 (puntatore ctx, sarà sovrascritto al ripristino)
//    sw   $a1, 48($t2)            # a1
//    sw   $a2, 52($t2)            # a2
//    sw   $a3, 56($t2)            # a3
//    
//    # Legge il registro Status (CP0 $12) e lo pulisce
//    mfc0 $t0, $12                # $t0 = Status corrente
//    li   $t1, 0xFFFFF8F9         # maschera per azzerare EXL(bit1), ERL(bit2), IPL(bit10-8)
//    and  $t0, $t0, $t1           # azzera i bit indesiderati
//    ori  $t0, $t0, 0x1           # imposta IE (bit0) = 1 (interrupt abilitati)
//    sw   $t0, 60($t2)            # salva Status pulito
//
//    # Salva EPC (CP0 $14)
//    mfc0 $t0, $14                # $t0 = EPC
//    sw   $t0, 64($t2)            # salva EPC
//    sw	 $fp, 68($t2)		 # fp
//    jr   $ra                     # ritorna al chiamante
//    nop                          # branch delay slot

    
/* ============================================================
 * cpu_context_restore
 * Ripristina TUTTI i 32 registri e salta all'EPC.
 * Input: $a0 = puntatore alla struttura CpuContext
 * ============================================================ */
    .global cpu_context_restore
    .ent cpu_context_restore
cpu_context_restore:
    .set noreorder
    .set noat

    # 1. Usiamo k0 come ancora fissa per il puntatore al contesto
    move    $k0, $a0             

    # 2. Ripristino Registri Speciali CP0 e HI/LO
    lw      $k1, 124($k0)        # Carica HI
    mthi    $k1
    ehb
    nop
    nop
    nop
    
    lw      $k1, 128($k0)        # Carica LO
    mtlo    $k1
    ehb
    nop
    nop
    nop
    
    //mtc0    $zero, $9            # Reset Core Timer Count ($9)
    ehb
    nop
    nop
    nop
    
    lw      $k1, 60($k0)         # Carica Status salvato
    ori     $k1, $k1, 0x0002     # Forza bit EXL = 1
    mtc0    $k1, $12             # Ripristina Status ($12)
    ehb
    nop
    nop
    nop
    
    lw      $k1, 64($k0)         # Carica EPC (Indirizzo di ritorno)
    mtc0    $k1, $14             # Ripristina EPC ($14)
    ehb
    nop
    nop
    nop
    
    # 3. Ripristino Registri Generali 
    lw      $ra,  0($k0)         # ra (31)
    lw      $sp,  4($k0)         # sp (29)
    lw      $gp,  8($k0)         # gp (28)
    lw      $s0, 12($k0)         # s0 (16)
    lw      $s1, 16($k0)         # s1 (17)
    lw      $s2, 20($k0)         # s2 (18)
    lw      $s3, 24($k0)         # s3 (19)
    lw      $s4, 28($k0)         # s4 (20)
    lw      $s5, 32($k0)         # s5 (21)
    lw      $s6, 36($k0)         # s6 (22)
    lw      $s7, 40($k0)         # s7 (23)
    
    lw      $a1, 48($k0)         # a1 (5)
    lw      $a2, 52($k0)         # a2 (6)
    lw      $a3, 56($k0)         # a3 (7)
    
    lw      $fp, 68($k0)         # fp/s8 (30)
    lw      $v0, 72($k0)         # v0 (2)
    lw      $v1, 76($k0)         # v1 (3)
    
    lw      $t0, 84($k0)         # t0 (8)
    lw      $t1, 88($k0)         # t1 (9)
    lw      $t2, 92($k0)         # t2 (10)
    lw      $t3, 96($k0)         # t3 (11)
    lw      $t4, 100($k0)        # t4 (12)
    lw      $t5, 104($k0)        # t5 (13)
    lw      $t6, 108($k0)        # t6 (14)
    lw      $t7, 112($k0)        # t7 (15)
    lw      $t8, 116($k0)        # t8 (24)
    lw      $t9, 120($k0)        # t9 (25)

    # 4. Ultimi due registri critici
    # Carichiamo a0 per ultimo perché k0 deriva da lui
    lw      $a0, 44($k0)         # a0 (4) originale del task
    
    # Recuperiamo l'EPC da k1 per fare il salto finale
   // mfc0    $k1, $14             

    # 5. Salto finale al task
    //jr      $k1                  # Jump all'indirizzo dove il task era sospeso
    sync
    ehb
    eret
    nop                          # Delay slot
    nop
    nop
    

    .end cpu_context_restore

    
    .global cpu_set_sp
cpu_set_sp:
    move $sp, $a0                # sp = nuovo valore
    jr   $ra                     # ritorna
    nop

/* ============================================================
 * cpu_get_sp
 * Restituisce lo stack pointer corrente
 * Input: nessuno
 * Output: $v0 = sp corrente
 * ============================================================ */
    .global cpu_get_sp
cpu_get_sp:
    move $v0, $sp                # v0 = sp
    jr   $ra                     # ritorna
    nop

/* ============================================================
 * cpu_idle
 * Porta la CPU in stato di idle (attesa interrupt)
 * Input: nessuno
 * Output: nessuno (ritorna solo dopo un interrupt)
 * ============================================================ */
    .global cpu_idle
cpu_idle:
    wait                         # istruzione WAIT (PIC32)
    j	cpu_idle                    # ritorna (dopo interrupt)
    nop

/* ============================================================
 * cpu_irq_disable
 * Disabilita gli interrupt globali
 * Input: nessuno
 * Output: nessuno
 * ============================================================ */
    .global cpu_irq_disable
cpu_irq_disable:
    di                           # disable interrupts
    jr   $ra                     # ritorna
    nop

/* ============================================================
 * cpu_irq_enable
 * Abilita gli interrupt globali
 * Input: nessuno
 * Output: nessuno
 * ============================================================ */
    .global cpu_irq_enable
cpu_irq_enable:
    ei                           # enable interrupts
    jr   $ra                     # ritorna
    nop
    
    
    
    /* ============================================================
 * cpu_irq_save
 * Salva lo stato corrente degli interrupt e li disabilita
 * Input: nessuno
 * Output: $v0 = vecchio stato (valore del registro Status)
 * ============================================================ */
    .global cpu_irq_save
cpu_irq_save:
    mfc0 $v0, $12          # legge Status
    di                     # disabilita interrupt
    nop
    nop
    jr   $ra
    nop

/* ============================================================
 * cpu_irq_restore
 * Ripristina lo stato degli interrupt (passato in $a0)
 * Input: $a0 = vecchio stato (valore Status)
 * Output: nessuno
 * ============================================================ */
    .global cpu_irq_restore
cpu_irq_restore:
    mtc0 $a0, $12          # scrive il vecchio Status
    nop
    nop
    jr   $ra
    nop