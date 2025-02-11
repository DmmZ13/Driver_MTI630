#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "driver_mti630.h"



int main() {
    // Mensagem de exemplo em formato de string com espaços
    //char hexMessage[] = "FA FF 36 4E 10 60 04 02 53 35 CB 20 10 10 3F 7F FA 66 BB 40 34 B8 3C 49 96 2D 3B 5A 1F 82 80 20 0C 39 FF 91 01 BA EC DD 81 BA 86 53 80 40 20 0C BE 64 DB 99 BC E7 FF 83 41 1D C2 CF C0 20 0C BE 90 48 80 3F 67 32 04 BF 54 89 04 E0 20 04 00 00 00 03 C9";

    uint8_t hexMessage[] = {0xFA,	0xff,	0x36,	0x26,	0x20,	0x10,	0x10,	0x3E,	0xFC,	0x23,	0x42,	0x3F,	0x00,	0xE0,	0x28,	0xBF,	0x01,	0x8F,	0xA7,	0x3E,	0xFE,	0xEE,	0xB1,	0x10,	0x60,	0x04,	0x00,	0x0A,	0xF8,	0x4C,	0xE0,	0x20,	0x04,	0x00,	0x00,	0x00,	0x02,	0x10,	0x20,	0x02,	0x1B,	0xC5,	0xD4};
    size_t tamanho = sizeof(hexMessage) / sizeof(hexMessage[0]);

    // Cria uma estrutura destino que será passada para a função
    DataDriver destino[MAX_IDS]; 

    // Chama a função para armazenar os dados da mensagem
    GuardaMsg(destino, hexMessage, tamanho);

    // Exibe os dados armazenados
    for (int i = 0; i < MAX_IDS; ++i) {
        printf("ID 0x%04X %s: ", destino[i].id, destino[i].idName);

        if (destino[i].length > 0) {
            // Se o ID armazena floats
            if (i == 0 || i == 5 || i == 6 || i == 7 || i == 9 || i == 10 || 
                i == 11 || i == 12 || i == 13 || i == 14 || i == 15 || i == 18) {
                
                for (int j = 0; j < destino[i].length; ++j) {
                    printf("%f ", destino[i].dataf[j]); // Exibe floats
                }
            } 
            // Se o ID armazena inteiros (uint32_t)
            else {
                for (int j = 0; j < destino[i].length; ++j) {
                    printf("%u ", destino[i].data[j]); // Exibe inteiros
                }
            }
        }
        
        printf("\n");
    }
    return 0;
}