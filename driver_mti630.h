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

/* DEFINIÇÃO DE FUNÇÕES */
uint8_t hexCharToByte(const char *hex);

int findIDIndex(uint16_t id, DataDriver *entries, int numEntries);

float uint32ToFloat(const uint32_t bytes);

void GuardaMsg(DataDriver *Dest, char hexMessage[]);

#endif // DATA_DRIVER_H