#ifndef _I2C_ReadWrite
#define _I2C_ReadWrite

#include <stdbool.h>
#include "main.h"

#define BH1750_ADDR         (0x23 << 1)  // ????? (<<1 ??? 8-??????? ???????)
#define BH1750_CMD_POWER_ON 0x01
#define BH1750_CMD_RESET    0x07
#define BH1750_CMD_CONT_H   0x10

float BH1750_ReadLux(void);
void BH1750_WriteCmd(uint8_t cmd);
void BH1750_Init(void);
static int I2C_WriteCmd(uint8_t cmd);
#endif