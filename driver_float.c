#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IDS 19  // Defina o número máximo de IDs que você pode ter

// Estrutura para armazenar informações de ID
typedef struct {
    uint16_t id;       // ID do dado (por exemplo, 0x1060)
    uint8_t length;    // Número de bytes de dados associados ao ID
    float data[64];    // Buffer para armazenar os dados em formato float (máx 64 floats)
    char idName[50];   // Nome do ID (descrição)
} DataEntry;

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
int findIDIndex(uint16_t id, DataEntry *entries, int numEntries) {
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

int main() {
    // Mensagem de exemplo em formato de string com espaços
    char hexMessage[] = "FA FF 36 4E 10 60 04 02 53 35 CB 20 10 10 3F 7F FA 66 BB 40 34 B8 3C 49 96 2D 3B 5A 1F 82 80 20 0C 39 FF 91 01 BA EC DD 81 BA 86 53 80 40 20 0C BE 64 DB 99 BC E7 FF 83 41 1D C2 CF C0 20 0C BE 90 48 80 3F 67 32 04 BF 54 89 04 E0 20 04 00 00 00 03 C9";
    int hexMessageLength = strlen(hexMessage);

    // Converta a string hexagonal para um array de bytes, ignorando espaços
    uint8_t message[hexMessageLength / 2];
    int messageLength = 0;

    for (int i = 0; i < hexMessageLength; i += 2) {
        while (hexMessage[i] == ' ') i++; // Ignorar espaços em branco
        if (hexMessage[i] == '\0' || hexMessage[i + 1] == '\0') break; // Verificar fim da string
        message[messageLength++] = hexCharToByte(&hexMessage[i]);
    }

    // IDs conhecidos e tamanhos que você possui
    DataEntry knownIDs[MAX_IDS] = {
        {0x0810, 0, {0}, "Temperature"}, // 0
        {0x1010, 0, {0}, "UtcTime"}, // 1
        {0x1020, 0, {0}, "PacketCounter"}, // 2
        {0x1060, 0, {0}, "SampleTimeFine"}, // 3
        {0x1070, 0, {0}, "SampleTimeCoarse"}, // 4
        {0x2010, 0, {0}, "Quaternion"}, // 5
        {0x2020, 0, {0}, "RotationMatrix"}, // 6
        {0x2030, 0, {0}, "EulerAngles"}, // 7
        {0x3010, 0, {0}, "BaroPressure"}, // 8
        {0x4010, 0, {0}, "Delta V"}, // 9
        {0x4020, 0, {0}, "Acceleration"}, // 10
        {0x4030, 0, {0}, "Free Acceleration"}, // 11
        {0x4040, 0, {0}, "AccelerationHR"}, // 12
        {0x8020, 0, {0}, "Rate of Turn"}, // 13
        {0x8030, 0, {0}, "Delta Q"}, // 14
        {0x8040, 0, {0}, "RateOfTurnHR"}, // 15
        {0xE010, 0, {0}, "StatusByte"}, // 16
        {0xE020, 0, {0}, "StatusWord"}, // 17
        {0xC020, 0, {0}, "MagneticField"}, // 18
        // Adicione mais IDs conforme necessário
    };

    int numKnownIDs = 19;  // Atualize com o número real de IDs

    int pos = 4; // Começar após os primeiros 4 bytes irrelevantes

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


        // Converte cada grupo de 4 bytes para um float e armazena no buffer
        for (int i = 0; i < length / 4; ++i) {
            knownIDs[index].data[i] = uint32ToFloat(((message[pos] << 24) | (message[pos + 1] << 16)| (message[pos+2] << 8) | (message[pos + 3])));
            pos += 4;
        }

        knownIDs[index].length = length / 4; // Agora o length é o número de floats
    }

    // Exibe os dados armazenados
    for (int i = 0; i < numKnownIDs; ++i) {
        printf("ID 0x%04X %s: ", knownIDs[i].id, knownIDs[i].idName);
        for (int j = 0; j < knownIDs[i].length; ++j) {
            printf("%f ", knownIDs[i].data[j]);
        }
        printf("\n");
    }

    return 0;
}