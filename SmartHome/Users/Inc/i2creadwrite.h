#ifndef _I2C_ReadWrite
#define _I2C_ReadWrite

#include <stdbool.h>
#include "main.h"

#define BH1750_ADDR          (0x23 << 1)
#define BH1750_CMD_POWER_ON  0x01
#define BH1750_CMD_RESET     0x07
#define BH1750_CMD_CONT_H    0x10

#define SHT21_ADDR           (0x40 << 1)
#define SHT21_NO_HOLD_TEMP   0xF3
#define SHT21_NO_HOLD_HUMID  0xF5

uint16_t Read2Bytes(uint8_t address);
void ReadNBytes(uint8_t address, uint8_t *buf, uint16_t N);
float BH1750_ReadLux(void);
float SHT21_ReadTemperature(void);
float SHT21_ReadHumidity(void);
void BH1750_Init(void);
static int I2C_WriteCmd(uint8_t cmd, uint8_t address);

#endif