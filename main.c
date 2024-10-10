#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "driver_mti630.h"

void main() {
    // Mensagem de exemplo em formato de string com espaços
    char hexMessage[] = "FA FF 36 4E 10 60 04 02 53 35 CB 20 10 10 3F 7F FA 66 BB 40 34 B8 3C 49 96 2D 3B 5A 1F 82 80 20 0C 39 FF 91 01 BA EC DD 81 BA 86 53 80 40 20 0C BE 64 DB 99 BC E7 FF 83 41 1D C2 CF C0 20 0C BE 90 48 80 3F 67 32 04 BF 54 89 04 E0 20 04 00 00 00 03 C9";

    // Cria uma estrutura destino que será passada para a função
    DataDriver destino[MAX_IDS];  // Supondo que MAX_IDS define o número de IDs conhecidos

    // Chama a função para armazenar os dados da mensagem
    GuardaMsg(destino, hexMessage);

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
}