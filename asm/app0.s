# app0.s - Interactive shell for Micro-App OS (Semplificato)
.name "shell"
.version 1
.ramsize 8192
.capabilities 0x0F

# ============================================================================
# Constants (solo numeri, niente label)
# ============================================================================
.equ CAP_SYSTEM,     1
.equ CAP_LOGGING,    2
.equ CAP_SENSORS,    4
.equ CAP_STORAGE,    8

.equ ACTION_GET,          0
.equ ACTION_SEND,         1
.equ ACTION_LOAD_APP,     2
.equ ACTION_STOP,         3
.equ ACTION_STORAGE_CREATE, 4
.equ ACTION_STORAGE_WRITE,  5
.equ ACTION_STORAGE_READ,   6
.equ ACTION_STORAGE_DELETE, 7
.equ ACTION_STORAGE_LIST,   8
.equ ACTION_RECEIVE,        9

# ============================================================================
# Data section
# ============================================================================
.data
.align 4

cmd_buf:        .space 64
tmp_buf:        .space 16
file_buf:       .space 64
code_buf:       .space 4096
num_buf:        .space 12
storage_params: .space 16

dev_name_ram0:  .asciiz "ram0"
temp_filename:  .asciiz "temp.bin"

banner:         .asciiz "\nMicro-App OS Shell v1.0\n"
prompt:         .asciiz "> "
msg_unknown:    .asciiz "Unknown command\n"
msg_loaded:     .asciiz "Loaded app ID: "
msg_load_fail:  .asciiz "Load failed\n"
msg_stopped:    .asciiz "App stopped\n"
msg_download_prompt: .asciiz "Enter size and binary\n"
msg_download_ok: .asciiz "Download OK\n"
msg_download_fail: .asciiz "Download failed\n"
msg_list_stub:  .asciiz "List not implemented\n"

str_load:       .asciiz "load"
str_stop:       .asciiz "stop"
str_download:   .asciiz "download"
str_list:       .asciiz "list"
str_help:       .asciiz "help"

help_text:      .asciiz "Commands: load, stop, download, list, help\n"

# ============================================================================
# Code section
# ============================================================================
.text
_start:
    CALL print_banner

main_loop:
    # Print prompt "> "
    ADDI a0, zero, CAP_LOGGING
    ADDI v0, zero, ACTION_SEND
    LUI a1, prompt
    ADDI a1, a1, prompt
    VCALL

    # Read command
    LUI a1, cmd_buf
    ADDI a1, a1, cmd_buf
    ADDI a2, zero, 64
    ADDI a0, zero, CAP_LOGGING
    ADDI v0, zero, ACTION_RECEIVE
    VCALL

    # Parse - per ora solo comando "help"
    LUI t0, str_help
    ADDI t0, t0, str_help
    LUI s1, cmd_buf
    ADDI s1, s1, cmd_buf
    CALL strcmp
    BEQ v0, zero, cmd_help

    # Unknown command
    LUI a1, msg_unknown
    ADDI a1, a1, msg_unknown
    CALL print_string
    JMP main_loop

# ----------------------------------------------------------------------------
cmd_help:
    ADDI sp, sp, -32
    STORE sp, zero, ra
    LUI a1, help_text
    ADDI a1, a1, help_text
    CALL print_string
    LOAD ra, sp, zero
    ADDI sp, sp, 32
    JMP main_loop

# ============================================================================
# Helper functions
# ============================================================================

print_string:
    ADDI a0, zero, CAP_LOGGING
    ADDI v0, zero, ACTION_SEND
    VCALL
    RET

print_banner:
    ADDI sp, sp, -4
    STORE sp, zero, ra
    LUI a1, banner
    ADDI a1, a1, banner
    CALL print_string
    LOAD ra, sp, zero
    ADDI sp, sp, 4
    RET

strcmp:
    MOV t1, s1
    MOV t2, t0
_cmp:
    LOADB t3, t1, zero
    LOADB t4, t2, zero
    BNE t3, t4, _diff
    BEQ t3, zero, _eq
    ADDI t1, t1, 1
    ADDI t2, t2, 1
    JMP _cmp
_eq:
    ADDI v0, zero, 0
    RET
_diff:
    SUB v0, t3, t4
    RET