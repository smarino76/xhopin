/**
 * @file bin2c.c
 * @author Santiago Marino
 * @year 2026
 * @brief Convert a binary file to a C array.
 * 
 * Compilation: gcc -o bin2c bin2c.c
 * Usage: ./bin2c input.bin array_name > output.h
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <binary file> <array name>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buf = malloc(size);
    fread(buf, 1, size, f);
    fclose(f);

    printf("const unsigned char %s[] = {\n", argv[2]);
    for (long i = 0; i < size; i++) {
        printf("0x%02x,", buf[i]);
        if ((i + 1) % 12 == 0) printf("\n");
    }
    printf("\n};\n");
    printf("const unsigned int %s_len = %ld;\n", argv[2], size);
    free(buf);
    return 0;
}