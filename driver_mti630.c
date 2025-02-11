#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "driver_mti630.h"

// Inicializa o array com os IDs conhecidos
DataDriver knownIDs[MAX_IDS] = {
    {0x0810, 0, {0}, {0}, "Temperature"},       // index: 0     dado: Real (R) -> Float32 se y = 0
    {0x1010, 0, {0}, {0}, "UtcTime"},           // index: 1     dado: U4, U2, U1, U1, U1, U1, U1
    {0x1020, 0, {0}, {0}, "PacketCounter"},     // index: 2     dado: U2
    {0x1060, 0, {0}, {0}, "SampleTimeFine"},    // index: 3     dado: U4
    {0x1070, 0, {0}, {0}, "SampleTimeCoarse"},  // index: 4     dado: U4
    {0x2010, 0, {0}, {0}, "Quaternion"},        // index: 5     dado: R R R R
    {0x2020, 0, {0}, {0}, "RotationMatrix"},    // index: 6     dado: R R R R R R R R R
    {0x2030, 0, {0}, {0}, "EulerAngles"},       // index: 7     dado: R R R
    {0x3010, 0, {0}, {0}, "BaroPressure"},      // index: 8     dado: U4
    {0x4010, 0, {0}, {0}, "Delta V"},           // index: 9     dado: R R R
    {0x4020, 0, {0}, {0}, "Acceleration"},      // index: 10    dado: R R R      
    {0x4030, 0, {0}, {0}, "Free Acceleration"}, // index: 11    dado: R R R
    {0x4040, 0, {0}, {0}, "AccelerationHR"},    // index: 12    dado: R R R
    {0x8020, 0, {0}, {0}, "Rate of Turn"},      // index: 13    dado: R R R
    {0x8030, 0, {0}, {0}, "Delta Q"},           // index: 14    dado: R R R R
    {0x8040, 0, {0}, {0}, "RateOfTurnHR"},      // index: 15    dado: R R R
    {0xE010, 0, {0}, {0}, "StatusByte"},        // index: 16    dado: U1
    {0xE020, 0, {0}, {0}, "StatusWord"},        // index: 17    dado: U4
    {0xC020, 0, {0}, {0}, "MagneticField"},     // index: 18    dado: R R R
    // Adicione mais IDs conforme necessário
};

typedef union {
    uint32_t bytes;  // Armazena como inteiro de 32 bits
    float real;        // Acessa como float
} ConversorFloat;

// Função para encontrar um ID na lista de IDs conhecidos
int findIDIndex(uint16_t id, DataDriver *entries, int numEntries) {
    for (int i = 0; i < numEntries; ++i) {
        if (entries[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Função para guardar a mensagem que vem em string nos respectivos vetores
void GuardaMsg(DataDriver *Dest, uint8_t hexMessage[], size_t tamanho) {

    printf("Mensagem formatada (16 bytes por linha):\n");
    for (size_t i = 0; i < tamanho; i++) {
        printf("%02X ", hexMessage[i]);  // Formato hexadecimal com 2 dígitos
        if ((i + 1) % 16 == 0) {
            printf("\n");  // Quebra de linha a cada 16 bytes
        }
    }
    printf("\n");

    // Copia os IDs conhecidos para Dest
    memcpy(Dest, knownIDs, sizeof(knownIDs));

    int numKnownIDs = MAX_IDS;  // Número de IDs conhecidos
    int pos = BYTES_IRRELEVANTES; // Começar após os primeiros bytes irrelevantes

    while (pos < tamanho - 1) {
        // Pega o ID (2 bytes)
        uint16_t id = (hexMessage[pos] << 8) | hexMessage[pos + 1];
        pos += 2;

        // Encontra o índice do ID
        int index = findIDIndex(id, knownIDs, numKnownIDs);
        if (index == -1) {
            printf("ID desconhecido: 0x%04X\n", id);
            break; // Se encontrar um ID desconhecido, interrompe
        }

        // Pega o comprimento dos dados associado ao ID
        uint8_t length = hexMessage[pos];
        pos++;

        // Verifica o tipo de dado e processa de acordo
        switch (index) {
            case 0: case 5: case 6: case 7: case 9: case 10: case 11:
            case 12: case 13: case 14: case 15: case 18: {
                ConversorFloat valorFloat;
                for (int i = 0; i < length / 4; ++i) {
                    valorFloat.bytes = ((hexMessage[pos] << 24) | (hexMessage[pos + 1] << 16) | (hexMessage[pos+2] << 8) | hexMessage[pos + 3]);
                    Dest[index].dataf[i] = valorFloat.real;
                    pos += 4;
                }
                Dest[index].length = length / 4; // O comprimento é o número de floats
                break;
            }

            case 3: case 4: case 8: case 17: {
                Dest[index].data[0] = ((hexMessage[pos] << 24) | (hexMessage[pos + 1] << 16) | (hexMessage[pos+2] << 8) | hexMessage[pos + 3]);
                pos += 4;
                Dest[index].length = length / 4; // O comprimento é o número de inteiros
                break;
            }

            case 2: {
                Dest[index].data[0] = ((hexMessage[pos] << 8) | hexMessage[pos + 1]);
                pos += 2;
                Dest[index].length = length / 2; // O comprimento é o número de inteiros
                break;
            }

            case 1: { 
                Dest[index].data[0] = ((hexMessage[pos] << 24) | (hexMessage[pos + 1] << 16) | (hexMessage[pos+2] << 8) | hexMessage[pos + 3]);
                pos += 4;
                Dest[index].data[1] = ((hexMessage[pos] << 8) | hexMessage[pos + 1]);
                pos += 2;
                Dest[index].data[2] = hexMessage[pos++];
                Dest[index].data[3] = hexMessage[pos++];
                Dest[index].data[4] = hexMessage[pos++];
                Dest[index].data[5] = hexMessage[pos++];
                Dest[index].data[6] = hexMessage[pos++];
                Dest[index].length = 7; // Comprimento é 7 para esse caso
                break;
            }

            case 16: {
                Dest[index].data[0] = hexMessage[pos];
                Dest[index].length = length;
                pos += length;
                break;
            }
        }     
    }
}