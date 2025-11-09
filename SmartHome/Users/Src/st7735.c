#include "st7735.h"
#include "font5x7.h"

// --- auxiliary functions SPI ---
static void ST7735_Select(void)
{
    LL_GPIO_ResetOutputPin(ST7735_CS_GPIO_Port, ST7735_CS_Pin);
}
 
static void ST7735_Unselect(void)
{
    LL_GPIO_SetOutputPin(ST7735_CS_GPIO_Port, ST7735_CS_Pin);
}

static void ST7735_Reset(void)
{
    LL_GPIO_ResetOutputPin(ST7735_RST_GPIO_Port, ST7735_RST_Pin);
    LL_mDelay(10);
    LL_GPIO_SetOutputPin(ST7735_RST_GPIO_Port, ST7735_RST_Pin);
    LL_mDelay(10);
}

static void ST7735_WriteCommand(uint8_t cmd)
{
    LL_GPIO_ResetOutputPin(ST7735_DC_GPIO_Port, ST7735_DC_Pin); // DC = 0 - comand
    ST7735_Select();

    while (!LL_SPI_IsActiveFlag_TXE(SPI1));
    LL_SPI_TransmitData8(SPI1, cmd);
    while (LL_SPI_IsActiveFlag_BSY(SPI1));

    ST7735_Unselect();
}

static void ST7735_WriteData(uint8_t data)
{
    LL_GPIO_SetOutputPin(ST7735_DC_GPIO_Port, ST7735_DC_Pin); // DC = 1 - data
    ST7735_Select();

    while (!LL_SPI_IsActiveFlag_TXE(SPI1));
    LL_SPI_TransmitData8(SPI1, data);
    while (LL_SPI_IsActiveFlag_BSY(SPI1));

    ST7735_Unselect();
}

// --- initialization ---
void ST7735_Init(void)
{
    ST7735_Reset();

    ST7735_WriteCommand(0x01); // Software reset
    LL_mDelay(150);

    ST7735_WriteCommand(0x11); // Sleep out
    LL_mDelay(150);

    ST7735_WriteCommand(0x3A); // Interface pixel format
    ST7735_WriteData(0x05);    // 16 bit/pixel

    ST7735_WriteCommand(0x36); // Memory data access control
    ST7735_WriteData(0x60);    // orientation
	
	ST7735_WriteCommand(0x21);

    ST7735_WriteCommand(0x29); // Display ON
}

// --- area of drawing ---
static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    ST7735_WriteCommand(0x2A); // Column addr set
    ST7735_WriteData(0x00);
    ST7735_WriteData(x0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(x1);

    ST7735_WriteCommand(0x2B); // Row addr set
    ST7735_WriteData(0x00);
    ST7735_WriteData(y0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(y1);

    ST7735_WriteCommand(0x2C); // Write to RAM
}

// --- draw 1 pixel ---
void ST7735_DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    ST7735_SetAddressWindow(x, y, x, y);
    ST7735_Select();
    LL_GPIO_SetOutputPin(ST7735_DC_GPIO_Port, ST7735_DC_Pin);

    while (!LL_SPI_IsActiveFlag_TXE(SPI1));
		
    LL_SPI_TransmitData8(SPI1, color >> 8); // 1 byte
    while (!LL_SPI_IsActiveFlag_TXE(SPI1));
    LL_SPI_TransmitData8(SPI1, color & 0xFF); // 0 byte
    while (LL_SPI_IsActiveFlag_BSY(SPI1));

    ST7735_Unselect();
}

// --- fill screen ---
void ST7735_FillScreen(uint16_t color)
{
    ST7735_SetAddressWindow(0, 0, ST7735_WIDTH, ST7735_HEIGHT);
    ST7735_Select();
    LL_GPIO_SetOutputPin(ST7735_DC_GPIO_Port, ST7735_DC_Pin);

    for (uint32_t i = 0; i < ST7735_WIDTH * ST7735_HEIGHT; i++)
    {
        while (!LL_SPI_IsActiveFlag_TXE(SPI1));
        LL_SPI_TransmitData8(SPI1, color >> 8);
        while (!LL_SPI_IsActiveFlag_TXE(SPI1));
        LL_SPI_TransmitData8(SPI1, color & 0xFF);
    }

    while (LL_SPI_IsActiveFlag_BSY(SPI1));
    ST7735_Unselect();
}

// --- draw symbol ---
void ST7735_DrawChar(uint8_t x, uint8_t y, char c, uint16_t color)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t line = Font5x7[(c - 0x20) * 5 + i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (line & 0x01)
                ST7735_DrawPixel(x + i, y + j, color);   // symbol
            else
                ST7735_DrawPixel(x + i, y + j, 0);      // background
            line >>= 1;
        }
    }
}

// --- draw string ---
void ST7735_DrawString(uint8_t x, uint8_t y, const char *str, uint16_t color)
{
    while (*str)
    {
        ST7735_DrawChar(x, y, *str, color);
        x += 6; // 5 pixel symbol + 1 pixel space
        str++;
    }
}
