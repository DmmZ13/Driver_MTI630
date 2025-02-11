#ifndef DATA_DRIVER_H
#define DATA_DRIVER_H

#include <stdint.h>
#include <string.h>

// Defina o número máximo de IDs que você pode ter
#define MAX_IDS 19  

// Defina o número de bytes iniciais irrelevantes
#define BYTES_IRRELEVANTES 4

// Estrutura para armazenar informações de ID
typedef struct {
    uint16_t id;       // ID do dado (por exemplo, 0x1060)
    uint8_t length;    // Número de bytes de dados armazenados em formato byte ou número de dados armazenados em formato float
    uint32_t data[256]; // Buffer para armazenar os dados em formato bytes
    float dataf[64];    // Buffer para armazenar os dados em formato float (máx 64 floats)
    char idName[50];   // Nome do ID (descrição)
} DataDriver;

extern DataDriver knownIDs[MAX_IDS];

/* DEFINIÇÃO DE FUNÇÕES */
uint8_t hexCharToByte(const char *hex);

int findIDIndex(uint16_t id, DataDriver *entries, int numEntries);

float uint32ToFloat(const uint32_t bytes);

void GuardaMsg(DataDriver *Dest, char hexMessage[]);

#endif // DATA_DRIVER_H