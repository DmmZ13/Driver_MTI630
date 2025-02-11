#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "driver_mti630.h"

uint8_t hexMessage[] = {
    0xFA, 0xFF, 0x36, 0x26, 0x20, 0x10, 0x10, 0x3E, 0xFC, 0x23, 
    0x42, 0x3F, 0x00, 0xE0, 0x28, 0xBF, 0x01, 0x8F, 0xA7, 0x3E, 
    0xFE, 0xEE, 0xB1, 0x10, 0x60, 0x04, 0x00, 0x0A, 0xF8, 0x4C, 
    0xE0, 0x20, 0x04, 0x00, 0x00, 0x00, 0x02, 0x10, 0x20, 0x02, 
    0x1B, 0xC5, 0xD4
};

int main() {
    size_t tamanho = sizeof(hexMessage) / sizeof(hexMessage[0]);
    printf("Mensagem formatada (16 bytes por linha):\n");
    for (size_t i = 0; i < tamanho; i++) {
        printf("%02X ", hexMessage[i]);  // Formato hexadecimal com 2 dígitos
        if ((i + 1) % 16 == 0) {
            printf("\n");  // Quebra de linha a cada 16 bytes
        }
    }
    printf("Tamanho do array: %d bytes\n", hexMessage[0]);  // %lu para size_t
    return 0;
}