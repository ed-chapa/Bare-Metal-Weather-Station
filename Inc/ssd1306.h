#ifndef SSD1306_H
#define SSD1306_H

#include "i2c.h"

#define OLED_ADDRESS 0x3C

extern uint8_t ssd1306_init_sequence[];
extern const uint8_t font5x8[][5];
extern const uint8_t fire_icon[4][16];
extern const uint8_t snow_icon[4][16];
extern const uint8_t thermometer_icon[4][16];
extern const uint8_t icon[2][7];

void SSD1306_Init();
void SSD1306_SendCommand(uint8_t cmd);
void SSD1306_ClearDisplay(void);
void SSD1306_DrawText(const char *text, uint8_t startX, uint8_t startPage);
void SSD1306_DrawIcon(const uint8_t icon[4][16], uint8_t startX, uint8_t startPage);

#endif
