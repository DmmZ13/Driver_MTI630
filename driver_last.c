#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

// Função para converter um par de caracteres hexadecimais em um byte
uint8_t hexCharToByte(const char *hex) {
    uint8_t byte = 0;
    for (int i = 0; i < 2; ++i) {
        byte <<= 4;
        if (hex[i] >= '0' && hex[i] <= '9') {
            byte |= hex[i] - '0';
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            byte |= hex[i] - 'A' + 10;
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            byte |= hex[i] - 'a' + 10;
        }
    }
    return byte;
}

// Função para encontrar um ID na lista de IDs conhecidos
int findIDIndex(uint16_t id, DataDriver *entries, int numEntries) {
    for (int i = 0; i < numEntries; ++i) {
        if (entries[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Função para converter 4 bytes para float
float uint32ToFloat(const uint32_t bytes) {
    float value;
    memcpy(&value, &bytes, sizeof(float));  // Copia 4 bytes para o valor float
    return value;
}

// Função para guardar a mensagem que vem em string nos respectivos vetores
void GuardaMsg(DataDriver *Dest, char hexMessage[]) {
    // Copia os IDs conhecidos para Dest
    memcpy(Dest, knownIDs, sizeof(knownIDs));

    int hexMessageLength = strlen(hexMessage);

    // Converta a string hexagonal para um array de bytes, ignorando espaços
    uint8_t message[hexMessageLength / 2];
    int messageLength = 0;

    for (int i = 0; i < hexMessageLength; i += 2) {
        while (hexMessage[i] == ' ') i++; // Ignorar espaços em branco
        if (hexMessage[i] == '\0' || hexMessage[i + 1] == '\0') break; // Verificar fim da string
        message[messageLength++] = hexCharToByte(&hexMessage[i]);
    }

    int numKnownIDs = MAX_IDS;  // Número de IDs conhecidos
    int pos = BYTES_IRRELEVANTES; // Começar após os primeiros bytes irrelevantes

    while (pos < messageLength - 1) {
        // Pega o ID (2 bytes)
        uint16_t id = (message[pos] << 8) | message[pos + 1];
        pos += 2;

        // Encontra o índice do ID
        int index = findIDIndex(id, knownIDs, numKnownIDs);
        if (index == -1) {
            printf("ID desconhecido: 0x%04X\n", id);
            break; // Se encontrar um ID desconhecido, interrompe
        }

        // Pega o comprimento dos dados associado ao ID
        uint8_t length = message[pos];
        pos++;

        // Verifica o tipo de dado e processa de acordo
        switch (index) {
            case 0: case 5: case 6: case 7: case 9: case 10: case 11:
            case 12: case 13: case 14: case 15: case 18: {
                for (int i = 0; i < length / 4; ++i) {
                    Dest[index].dataf[i] = uint32ToFloat(((message[pos] << 24) | (message[pos + 1] << 16) | (message[pos+2] << 8) | message[pos + 3]));
                    pos += 4;
                }
                Dest[index].length = length / 4; // O comprimento é o número de floats
                break;
            }

            case 3: case 4: case 8: case 17: {
                Dest[index].data[0] = ((message[pos] << 24) | (message[pos + 1] << 16) | (message[pos+2] << 8) | message[pos + 3]);
                pos += 4;
                Dest[index].length = length / 4; // O comprimento é o número de inteiros
                break;
            }

            case 2: {
                Dest[index].data[0] = ((message[pos] << 8) | message[pos + 1]);
                pos += 2;
                Dest[index].length = length / 2; // O comprimento é o número de inteiros
                break;
            }

            case 1: { 
                Dest[index].data[0] = ((message[pos] << 24) | (message[pos + 1] << 16) | (message[pos+2] << 8) | message[pos + 3]);
                pos += 4;
                Dest[index].data[1] = ((message[pos] << 8) | message[pos + 1]);
                pos += 2;
                Dest[index].data[2] = message[pos++];
                Dest[index].data[3] = message[pos++];
                Dest[index].data[4] = message[pos++];
                Dest[index].data[5] = message[pos++];
                Dest[index].data[6] = message[pos++];
                Dest[index].length = 7; // Comprimento é 7 para esse caso
                break;
            }

            case 16: {
                Dest[index].data[0] = message[pos];
                Dest[index].length = length;
                pos += length;
                break;
            }
        }     
    }
}

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