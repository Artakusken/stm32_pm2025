#ifndef STM32_TEMPLATE_SSD1306_H
#define STM32_TEMPLATE_SSD1306_H

#include <stm32f10x.h>
#include <stdint.h>

#include "spi1.h"

#define SSD1306_WIDTH    64
#define SSD1306_HEIGHT   64

#define SSD1306_CS_PIN    (1 << 0)
#define SSD1306_DC_PIN    (1 << 1)
#define SSD1306_RES_PIN   (1 << 2)
#define SSD1306_PORT      GPIOA

// Commands
#define SSD1306_SETCONTRAST          0x81
#define SSD1306_DISPLAYALLON_RESUME  0xA4
#define SSD1306_DISPLAYALLON         0xA5
#define SSD1306_NORMALDISPLAY        0xA6
#define SSD1306_INVERTDISPLAY        0xA7
#define SSD1306_DISPLAYOFF           0xAE
#define SSD1306_DISPLAYON            0xAF
#define SSD1306_SETDISPLAYOFFSET     0xD3
#define SSD1306_SETCOMPINS           0xDA
#define SSD1306_SETVCOMDETECT        0xDB
#define SSD1306_SETDISPLAYCLOCKDIV   0xD5
#define SSD1306_SETPRECHARGE         0xD9
#define SSD1306_SETMULTIPLEX         0xA8
#define SSD1306_SETLOWCOLUMN         0x00
#define SSD1306_SETHIGHCOLUMN        0x10
#define SSD1306_SETSTARTLINE         0x40
#define SSD1306_MEMORYMODE           0x20
#define SSD1306_COLUMNADDR           0x21
#define SSD1306_PAGEADDR             0x22
#define SSD1306_COMSCANINC           0xC0
#define SSD1306_COMSCANDEC           0xC8
#define SSD1306_SEGREMAP             0xA0
#define SSD1306_CHARGEPUMP           0x8D

void SSD1306_Init(void);
void SSD1306_UpdateScreen(void);
void SSD1306_Clear(void);
void SSD1306_DrawPattern(void);

extern uint8_t SSD1306_Buffer[];

#endif //STM32_TEMPLATE_SSD1306_H
