#ifndef APP_LOADER_H
#define APP_LOADER_H

#include <stdint.h>

int kernel_load_app(const uint8_t *binary, uint32_t bin_size);

#endif