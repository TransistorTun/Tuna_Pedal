#include "display_ssd1963.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>

/* Memory-to-memory DMA Handler */
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;

uint16_t RGB(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

///////////////////

void Init_SSD1963(void) {

	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);


	Lcd_Write_Cmd(0xE2); //Set PLL with OSC = 8MHz (hardware),	Multiplier N = 40, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 320MHz
	Lcd_Write_Data(0x27);		//����1
	Lcd_Write_Data(0x02);		//����2 Divider M = 2, PLL = 300/(M+1) = 100MHz
	Lcd_Write_Data(0x04);		//����3 Validate M and N values

	HAL_Delay(5);
	Lcd_Write_Cmd(0xE0);		// Start PLL command
	Lcd_Write_Data(0x01);		// enable PLL
	HAL_Delay(10);

	Lcd_Write_Cmd(0xE0);		// Start PLL command again
	Lcd_Write_Data(0x03);		// now, use PLL output as system clock
	HAL_Delay(12);
	Lcd_Write_Cmd(0x01);
	HAL_Delay(10);
	Lcd_Write_Cmd(0xE6);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0xFF);
	Lcd_Write_Data(0xFF);

	Lcd_Write_Cmd(0xB0);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);

	Lcd_Write_Data((TFT_WIDTH - 1) >> 8);
	Lcd_Write_Data(TFT_WIDTH - 1);
	Lcd_Write_Data((TFT_HEIGHT - 1) >> 8);
	Lcd_Write_Data(TFT_HEIGHT - 1);
	Lcd_Write_Data(0x00);		//RGB����

	Lcd_Write_Cmd(0xB4);
	Lcd_Write_Data((TFT_HT - 1) >> 8);
	Lcd_Write_Data(TFT_HT - 1);
	Lcd_Write_Data(TFT_HPS >> 8);
	Lcd_Write_Data(TFT_HPS);
	Lcd_Write_Data(TFT_HSYNC_PULSE);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Cmd(0xB6);		//Set vertical period
	Lcd_Write_Data((TFT_VT - 1) >> 8);
	Lcd_Write_Data(TFT_VT - 1);
	Lcd_Write_Data(TFT_VPS >> 8);
	Lcd_Write_Data(TFT_VPS);
	Lcd_Write_Data(TFT_VSYNC_PULSE);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);

	Lcd_Write_Cmd(0xF0);
	Lcd_Write_Data(0x03);

	Lcd_Write_Cmd(0x29);
	Lcd_Write_Cmd(0xD0);
	Lcd_Write_Data(0x00);

	Lcd_Write_Cmd(0x36);
	Lcd_Write_Data(0x08);

//	Lcd_Write_Cmd(0xBE);
//	Lcd_Write_Data(0x05);
//	Lcd_Write_Data(0xFE);
//	Lcd_Write_Data(0x01);
//	Lcd_Write_Data(0x00);
//	Lcd_Write_Data(0x00);
//	Lcd_Write_Data(0x00);

	Lcd_Write_Cmd(0xB8);
	Lcd_Write_Data(0x07);
	Lcd_Write_Data(0x01);
	Lcd_Write_Cmd(0xBA);
	Lcd_Write_Data(0X01);


}

void TFT_Set_X(uint16_t start_x, uint16_t end_x) {
	Lcd_Write_Cmd(SSD1963_SET_COLUMN_ADDRESS);
	Lcd_Write_Data(start_x >> 8);
	Lcd_Write_Data(start_x & 0x00ff);

	Lcd_Write_Data(end_x >> 8);
	Lcd_Write_Data(end_x & 0x00ff);
}

void TFT_Set_Y(uint16_t start_y, uint16_t end_y) {
	Lcd_Write_Cmd(SSD1963_SET_PAGE_ADDRESS);
	Lcd_Write_Data(start_y >> 8);
	Lcd_Write_Data(start_y & 0x00ff);

	Lcd_Write_Data(end_y >> 8);
	Lcd_Write_Data(end_y & 0x00ff);
}

void TFT_Set_XY(uint16_t x, uint16_t y) {
	TFT_Set_X(x, x);
	TFT_Set_Y(y, y);
}

void TFT_Set_Work_Area(uint16_t x, uint16_t y, uint16_t height, uint16_t width) {
	TFT_Set_X(x, x + height - 1);
	TFT_Set_Y(y, y + width - 1);
	Lcd_Write_Cmd(SSD1963_WRITE_MEMORY_START);
}

void TFT_Set_Window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	TFT_Set_X(x1, x2);
	TFT_Set_Y(y1, y2);
	Lcd_Write_Cmd(SSD1963_WRITE_MEMORY_START);
}

void TFT_Clear_Screen(uint16_t color) {
	unsigned int x, y;
	TFT_Set_Work_Area(0, 0, TFT_WIDTH, TFT_HEIGHT);
	Lcd_Write_Cmd(SSD1963_WRITE_MEMORY_START);
	for (x = 0; x < TFT_WIDTH; x++) {
		for (y = 0; y < TFT_HEIGHT; y++) {
			*(uint16_t*) (LCD_DATA) = color;
		}
	}
}

void TFT_Draw_Char(uint16_t x, uint16_t y, uint16_t color, uint16_t phone,
		const uint8_t *table, uint8_t ascii, uint8_t size) {
	uint8_t i, f = 0;

	for (i = 0; i < 8; i++) {
		for (f = 0; f < 8; f++) {
			if ((*(table + 8 * (ascii - 0x20) + i) >> (7 - f)) & 0x01) {
				TFT_Draw_Fill_Rectangle(x + f * size, y + i * size, size, size,
						color);
			} else {
				TFT_Draw_Fill_Rectangle(x + f * size, y + i * size, size, size,
						phone);
			}
		}
	}
}

void TFT_Draw_String(uint16_t x, uint16_t y, uint16_t color, uint16_t phone,
		const uint8_t *table, char *string, uint8_t size) {
	while (*string) {
		if ((x + 8) > (TFT_WIDTH - 1)) {
			x = 1;
			y = y + 8 * size;
		}
		TFT_Draw_Char(x, y, color, phone, table, *string, size);
		x += 8 * size;
		*string++;
	}
}

void TFT_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint8_t size, uint16_t color) {
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	int error2 = 0;

	for (;;) {
		TFT_Draw_Fill_Rectangle(x1, y1, size, size, color);

		if (x1 == x2 && y1 == y2)
			break;

		error2 = error * 2;

		if (error2 > -deltaY) {
			error -= deltaY;
			x1 += signX;
		}

		if (error2 < deltaX) {
			error += deltaX;
			y1 += signY;
		}
	}
}

void TFT_Draw_HLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size,
		uint16_t color) {
	uint16_t i = 0;
	TFT_Set_Work_Area(x, y, length, size);
	for (i = 0; i < (length * size); i++)
		Lcd_Write_Data(color);
}

void TFT_Draw_VLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size,
		uint16_t color) {
	uint16_t i = 0;
	TFT_Set_Work_Area(x, y, size, length);
	for (i = 0; i < (length * size); i++)
		Lcd_Write_Data(color);
}

void TFT_Draw_H_Dot_Line(uint16_t x, uint16_t y, uint16_t length, uint16_t size,
		uint16_t color) {

	length += x - 1;
	for (x = x; x < length; x += (size + 3)) {
		TFT_Set_X(x, x);
		TFT_Set_Y(y, y + size - 1);
		Lcd_Write_Cmd(SSD1963_WRITE_MEMORY_START);
		Lcd_Write_Data(color);
	}
}

void TFT_Draw_V_Dot_Line(uint16_t x, uint16_t y, uint16_t length, uint16_t size,
		uint16_t color) {

	length += y - 1;
	for (y = y; y < length; y += (size + 5)) {
		TFT_Set_Y(y, y);
		TFT_Set_X(x, x + size - 1);
		Lcd_Write_Cmd(SSD1963_WRITE_MEMORY_START);
		Lcd_Write_Data(color);
	}
}

void TFT_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width,
		uint8_t size, uint16_t color) {
	TFT_Draw_HLine(x, y, length, size, color);
	TFT_Draw_HLine(x, y + width, length, size, color);
	TFT_Draw_VLine(x, y, width, size, color);
	TFT_Draw_VLine(x + length - size, y, width, size, color);
}

void TFT_Draw_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length,
		uint16_t width, uint16_t color) {
	uint32_t i = 0;

	TFT_Set_Work_Area(x, y, length, width);
	for (i = 0; i < length * width; i++) {
		Lcd_Write_Data(color);
	}
}

void TFT_Draw_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint16_t x3, uint16_t y3, uint8_t size, uint16_t color) {

	TFT_Draw_Line(x1, y1, x2, y2, color, size);
	TFT_Draw_Line(x2, y2, x3, y3, color, size);
	TFT_Draw_Line(x3, y3, x1, y1, color, size);
}

void TFT_Draw_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill,
		uint8_t size, uint16_t color) {
	int a_, b_, P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_) {
		if (fill == 1) {
			TFT_Draw_Fill_Rectangle(x - a_, y - b_, 2 * a_ + 1, 2 * b_ + 1,
					color);
			TFT_Draw_Fill_Rectangle(x - b_, y - a_, 2 * b_ + 1, 2 * a_ + 1,
					color);
		} else {
			TFT_Draw_Fill_Rectangle(a_ + x, b_ + y, size, size, color);
			TFT_Draw_Fill_Rectangle(b_ + x, a_ + y, size, size, color);
			TFT_Draw_Fill_Rectangle(x - a_, b_ + y, size, size, color);
			TFT_Draw_Fill_Rectangle(x - b_, a_ + y, size, size, color);
			TFT_Draw_Fill_Rectangle(b_ + x, y - a_, size, size, color);
			TFT_Draw_Fill_Rectangle(a_ + x, y - b_, size, size, color);
			TFT_Draw_Fill_Rectangle(x - a_, y - b_, size, size, color);
			TFT_Draw_Fill_Rectangle(x - b_, y - a_, size, size, color);
		}
		if (P < 0) {
			P = (P + 3) + (2 * a_);
			a_++;
		} else {
			P = (P + 5) + (2 * (a_ - b_));
			a_++;
			b_--;
		}
	}
}

void TFT_Draw_Circle_Helper(int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, uint8_t size, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			TFT_Draw_Fill_Rectangle(x0 + x, y0 + y, size, size, color);
			TFT_Draw_Fill_Rectangle(x0 + y, y0 + x, size, size, color);
		}
		if (cornername & 0x2) {
			TFT_Draw_Fill_Rectangle(x0 + x, y0 - y, size, size, color);
			TFT_Draw_Fill_Rectangle(x0 + y, y0 - x, size, size, color);
		}
		if (cornername & 0x8) {
			TFT_Draw_Fill_Rectangle(x0 - y, y0 + x, size, size, color);
			TFT_Draw_Fill_Rectangle(x0 - x, y0 + y, size, size, color);
		}
		if (cornername & 0x1) {
			TFT_Draw_Fill_Rectangle(x0 - y, y0 - x, size, size, color);
			TFT_Draw_Fill_Rectangle(x0 - x, y0 - y, size, size, color);
		}
	}
}

void TFT_Draw_Round_Rect(uint16_t x, uint16_t y, uint16_t length,
		uint16_t width, uint16_t r, uint8_t size, uint16_t color) {

	TFT_Draw_HLine(x + r, y, length - 2 * r, size, color); // Top
	TFT_Draw_HLine(x + r, y + width - 1, length - 2 * r, size, color); // Bottom
	TFT_Draw_VLine(x, y + r, width - 2 * r, size, color); // Left
	TFT_Draw_VLine(x + length - 1, y + r, width - 2 * r, size, color); // Right

	TFT_Draw_Circle_Helper(x + r, y + r, r, 1, size, color);
	TFT_Draw_Circle_Helper(x + length - r - 1, y + r, r, 2, size, color);
	TFT_Draw_Circle_Helper(x + length - r - 1, y + width - r - 1, r, 4, size,
			color);
	TFT_Draw_Circle_Helper(x + r, y + width - r - 1, r, 8, size, color);
}

void TFT_Draw_Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, int16_t delta, uint16_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			TFT_Draw_VLine(x0 + x, y0 - y, 2 * y + 1 + delta, 1, color);
			TFT_Draw_VLine(x0 + y, y0 - x, 2 * x + 1 + delta, 1, color);
		}
		if (cornername & 0x2) {
			TFT_Draw_VLine(x0 - x, y0 - y, 2 * y + 1 + delta, 1, color);
			TFT_Draw_VLine(x0 - y, y0 - x, 2 * x + 1 + delta, 1, color);
		}
	}
}

void TFT_Draw_Fill_Round_Rect(uint16_t x, uint16_t y, uint16_t length,
		uint16_t width, uint16_t r, uint16_t color) {
	TFT_Draw_Fill_Rectangle(x + r, y, length - 2 * r, width, color);

	TFT_Draw_Fill_Circle_Helper(x + length - r - 1, y + r, r, 1,
			width - 2 * r - 1, color);
	TFT_Draw_Fill_Circle_Helper(x + r, y + r, r, 2, width - 2 * r - 1, color);
}

void TFT_Draw_BMP(uint16_t x, uint16_t y, uint16_t pwidth, uint16_t pheight,
		const uint16_t *p) {
	uint32_t i;
	uint32_t i_max = pwidth * pheight;
	TFT_Set_Work_Area(x, y, pwidth, pheight);
	HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)p, LCD_DATA, i_max);				//DMA can not show full pixel (480x272) is more than 16bit data length
//	HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 1000);

//	for (i = 0; i < i_max; i++)
//		*(uint16_t*) (LCD_DATA) = *(p + i);
//    TFT_Set_Work_Area(0,0,480,272);
}

static void LCD_Char(int16_t x, int16_t y, const GFXglyph *glyph,
		const GFXfont *font, uint8_t size, uint32_t color24) {
	uint8_t *bitmap = font->bitmap;
	uint16_t bo = glyph->bitmapOffset;
	uint8_t bits = 0, bit = 0;
	uint16_t set_pixels = 0;
	uint8_t cur_x, cur_y;
	for (cur_y = 0; cur_y < glyph->height; cur_y++) {
		for (cur_x = 0; cur_x < glyph->width; cur_x++) {
			if (bit == 0) {
				bits = (*(const unsigned char*) (&bitmap[bo++]));
				bit = 0x80;
			}
			if (bits & bit) {
				set_pixels++;
			} else if (set_pixels > 0) {
				TFT_Draw_Fill_Rectangle(
						x + (glyph->xOffset + cur_x - set_pixels) * size,
						y + (glyph->yOffset + cur_y) * size, size * set_pixels,
						size, color24);
				set_pixels = 0;
			}
			bit >>= 1;
		}
		if (set_pixels > 0) {
			TFT_Draw_Fill_Rectangle(
					x + (glyph->xOffset + cur_x - set_pixels) * size,
					y + (glyph->yOffset + cur_y) * size, size * set_pixels,
					size, color24);
			set_pixels = 0;
		}
	}
}

void LCD_Font(uint16_t x, uint16_t y, char *text, const GFXfont *p_font,
		uint8_t size, uint32_t color24) {
	int16_t cursor_x = x;
	int16_t cursor_y = y;
	GFXfont font;
	memcpy((&font), (p_font), (sizeof(GFXfont)));
	for (uint16_t text_pos = 0; text_pos < strlen(text); text_pos++) {
		char c = text[text_pos];
		if (c == '\n') {
			cursor_x = x;
			cursor_y += font.yAdvance * size;
		} else if (c >= font.first && c <= font.last && c != '\r') {
			GFXglyph glyph;
			memcpy((&glyph), (&font.glyph[c - font.first]), (sizeof(GFXglyph)));
			LCD_Char(cursor_x, cursor_y, &glyph, &font, size, color24);
			cursor_x += glyph.xAdvance * size;
		}
	}
}

void SSD1963_SetBacklight(uint8_t BL) {
	Lcd_Write_Cmd(SSD1963_SET_PWM_CONF); //set PWM for Backlight. Manual p.53, 6 parameters to be set
	Lcd_Write_Data(0x0002); // PWM Freq =100MHz/(256*(PWMF[7:0]+1))/256  PWMF[7:0]=4 PWM Freq=305Hz
	Lcd_Write_Data(BL); 								// PWM duty cycle
	Lcd_Write_Data(0x0001); 			// PWM controlled by host, PWM enabled
	Lcd_Write_Data(0x0040); 					// brightness level 0x00 - 0xFF
	Lcd_Write_Data(0x0000); 		// minimum brightness level =  0x00 - 0xFF
	Lcd_Write_Data(0x0000);					// brightness prescalar 0x0 - 0xF
}
