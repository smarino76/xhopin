#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LABELS 512
#define MAX_CODE 65536

typedef struct { char name[64]; uint32_t addr; } label_t;
label_t labels[MAX_LABELS];
int label_count = 0;

typedef struct { char name[64]; uint32_t value; } const_t;
const_t constants[MAX_LABELS];
int const_count = 0;

uint8_t code[MAX_CODE];
uint32_t code_len = 0;
uint32_t ram_size = 0, caps = 0;
char app_name[16] = "shell";

// --- Utility di Parsing ---

int reg_to_num(const char *name) {
    if (!name) return 0;
    char r[32]; sscanf(name, "%[^,]", r);
    const char *regs[] = {"zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7",
                          "s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra"};
    for (int i = 0; i < 32; i++) if (strcmp(r, regs[i]) == 0) return i;
    if (r[0] == 'r' && isdigit(r[1])) return atoi(&r[1]);
    return 0;
}

uint32_t resolve_value(const char *s) {
    char clean[64]; int j = 0;
    // Estrae contenuto di hi() o lo()
    if (strstr(s, "(")) {
        const char *start = strchr(s, '(') + 1;
        const char *end = strchr(s, ')');
        if (start && end) { strncpy(clean, start, end-start); clean[end-start] = '\0'; }
    } else {
        for (int i = 0; s[i] && s[i] != ','; i++) clean[j++] = s[i];
        clean[j] = '\0';
    }

    if (isdigit(clean[0]) || (clean[0] == '-' && isdigit(clean[1]))) return strtoul(clean, NULL, 0);
    for (int i = 0; i < const_count; i++) if (strcmp(constants[i].name, clean) == 0) return constants[i].value;
    for (int i = 0; i < label_count; i++) if (strcmp(labels[i].name, clean) == 0) return labels[i].addr;
    return 0; 
}

// --- Assemblaggio ---

void assemble_line(char *line, uint32_t pc) {
    char op[32], a1[64], a2[64], a3[64];
    int n = sscanf(line, "%s %s %s %s", op, a1, a2, a3);
    if (n < 1 || op[0] == '#' || op[0] == '.') return;

    uint32_t instr = 0;
    uint8_t opcode = 0, rd = 0, rs = 0, rt = 0;
    int16_t imm = 0;

    // Sincronizzato con opcodes.h e vm_core.c
    if (strcmp(op, "ADD") == 0)  { opcode = 0x01; rd = reg_to_num(a1); rs = reg_to_num(a2); rt = reg_to_num(a3); }
    else if (strcmp(op, "SUB") == 0)  { opcode = 0x02; rd = reg_to_num(a1); rs = reg_to_num(a2); rt = reg_to_num(a3); }
    else if (strcmp(op, "ADDI") == 0) { 
        opcode = 0x03; rd = reg_to_num(a1); rs = reg_to_num(a2); 
        uint32_t val = resolve_value(a3);
        if (strstr(a3, "hi(")) val >>= 16; else if (strstr(a3, "lo(")) val &= 0xFFFF;
        imm = (int16_t)val;
    }
    else if (strcmp(op, "LUI") == 0) {
        opcode = 0x13; rd = reg_to_num(a1);
        uint32_t val = resolve_value(a2);
        imm = (int16_t)(val >> 16);
    }
    else if (strcmp(op, "MOV") == 0) { opcode = 0x12; rd = reg_to_num(a1); rs = reg_to_num(a2); }
    else if (strcmp(op, "LOAD") == 0 || strcmp(op, "STORE") == 0) {
        opcode = (op[0] == 'L') ? 0x20 : 0x21;
        rd = reg_to_num(a1);
        // Gestisce sia "LOAD r1, r2, 4" che "LOAD r1, 4(r2)"
        if (strchr(a2, '(')) {
            char *p = strchr(a2, '(');
            imm = atoi(a2);
            rs = reg_to_num(p + 1);
        } else {
            rs = reg_to_num(a2);
            imm = (int16_t)resolve_value(a3);
        }
    }
    else if (strcmp(op, "LOADB") == 0)  { opcode = 0x22; rd = reg_to_num(a1); rs = reg_to_num(a2); imm = resolve_value(a3); }
    else if (strcmp(op, "STOREB") == 0) { opcode = 0x23; rd = reg_to_num(a1); rs = reg_to_num(a2); imm = resolve_value(a3); }
    else if (strcmp(op, "BEQ") == 0 || strcmp(op, "BNE") == 0) {
        opcode = (op[1] == 'E') ? 0x30 : 0x31;
        rd = reg_to_num(a1); rs = reg_to_num(a2);
        imm = (int16_t)((resolve_value(a3) - pc - 4) / 4);
    }
    else if (strcmp(op, "JMP") == 0)  { opcode = 0x33; instr = (opcode << 24) | (resolve_value(a1) & 0xFFFFFF); goto emit; }
    else if (strcmp(op, "CALL") == 0) { opcode = 0x34; instr = (opcode << 24) | (resolve_value(a1) & 0xFFFFFF); goto emit; }
    else if (strcmp(op, "RET") == 0)   { instr = 0x35000000; goto emit; }
    else if (strcmp(op, "VCALL") == 0) { instr = 0x50000000; goto emit; }
    else if (strcmp(op, "HALT") == 0)  { instr = 0xFF000000; goto emit; }

    // Formato standard: [OP:8][RD:8][RS:8][IMM/RT:16]
    instr = (opcode << 24) | (rd << 16) | (rs << 8) | (rt & 0xFF);
    if (opcode == 0x03 || opcode == 0x13 || opcode >= 0x20) {
        instr = (opcode << 24) | (rd << 16) | (rs << 8) | (uint16_t)imm;
    }

emit:
    memcpy(&code[code_len], &instr, 4);
    code_len += 4;
}

int main(int argc, char **argv) {
    if (argc < 3) return 1;
    FILE *f = fopen(argv[1], "r");
    char line[256];
    uint32_t pc = 0;

    // Pass 1: Header e Label
    while (fgets(line, 256, f)) {
        char op[64], a1[64], a2[64];
        char *p = line; while(isspace(*p)) p++;
        if (!*p || *p == '#') continue;
        if (sscanf(p, ".name \"%[^\"]\"", a1) == 1) strncpy(app_name, a1, 15);
        else if (sscanf(p, ".ramsize %s", a1) == 1) ram_size = strtoul(a1, NULL, 0);
        else if (sscanf(p, ".capabilities %s", a1) == 1) caps = strtoul(a1, NULL, 0);
        else if (sscanf(p, ".equ %[^,], %s", a1, a2) == 2) {
            strncpy(constants[const_count].name, a1, 63);
            constants[const_count++].value = strtoul(a2, NULL, 0);
        }
        else if (strchr(p, ':')) {
            char *n = strtok(p, ":");
            while(isspace(*n)) n++;
            labels[label_count].addr = pc;
            strncpy(labels[label_count++].name, n, 63);
        }
        else if (p[0] != '.') pc += 4;
    }

    // Pass 2: Generazione codice
    rewind(f);
    pc = 0;
    while (fgets(line, 256, f)) {
        char *p = line; while(isspace(*p)) p++;
        if (!*p || *p == '#' || strchr(p, ':') || p[0] == '.') continue;
        assemble_line(p, pc);
        pc += 4;
    }

    // Scrittura file BIN
    FILE *out = fopen(argv[2], "wb");
    uint32_t magic = 0xCAFEFACE, ver = 1, dummy = 0;
    fwrite(&magic, 4, 1, out);
    fwrite(app_name, 1, 16, out);
    fwrite(&ver, 4, 1, out);
    fwrite(&code_len, 4, 1, out);
    fwrite(&ram_size, 4, 1, out);
    fwrite(&dummy, 4, 1, out); // text_start
    fwrite(&caps, 4, 1, out);
    fwrite(&dummy, 4, 1, out); // checksum
    fwrite(code, 1, code_len, out);

    fclose(f); fclose(out);
    printf("Successo! %d byte scritti.\n", code_len);
    return 0;
}