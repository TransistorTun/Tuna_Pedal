#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <SSD1963/ssd1963.h>


/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "../../Drivers/lv_drv_conf.h"
#include <stdbool.h>
#include <stdint.h>

/*********************
 *      DEFINES
 *********************/
#define SSD1963_CMD_MODE     0
#define SSD1963_DATA_MODE    1

#define MY_DISP_HOR_RES    800
#define MY_DISP_VER_RES    480

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static inline void SSD1963_data_mode(void);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/
static bool cmd_mode = true;

/**********************
 *      MACROS
 **********************/

/**********************
 *  STATIC FUNCTION
 **********************/
/**
 * Data mode
 */
static inline void ssd1963_data_mode(void)
{
    if(cmd_mode != false) {
        LV_DRV_DISP_CMD_DATA(SSD1963_DATA_MODE);
        cmd_mode = false;
    }
}

//////////////////////////////////////////////////////////////////////////

uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void LCD_Write_CMD(uint16_t CMD)
{
	*(uint16_t *)(LCD_REG) = CMD;
}
////////////////////////
void LCD_Write_Data(uint16_t data)
{
	*(uint16_t *)(LCD_DATA) = data;
}

void LCD_Write_COM(uint16_t data)
{
	*(uint16_t *)(LCD_REG) = data;
}
///////////////////
void LCD_SetArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey)
{
	LCD_Write_CMD(SSD1963_SET_COLUMN_ADDRESS);
	LCD_Write_Data((sx >> 8) & 0xFF);
	LCD_Write_Data((sx >> 0) & 0xFF);
	LCD_Write_Data((ex >> 8) & 0xFF);
	LCD_Write_Data((ex >> 0) & 0xFF);

	LCD_Write_CMD(SSD1963_SET_PAGE_ADDRESS);
	LCD_Write_Data((sy >> 8) & 0xFF);
	LCD_Write_Data((sy >> 0) & 0xFF);
	LCD_Write_Data((ey >> 8) & 0xFF);
	LCD_Write_Data((ey >> 0) & 0xFF);
}

//=============================================================================
// Fill area of specified color
//=============================================================================
void LCD_FillArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, int16_t color)
{
	uint16_t i;
	LCD_SetArea(sx, ex, sy, ey);
	LCD_Write_CMD(SSD1963_WRITE_MEMORY_START);
	for (i = 0; i < ((ex - sx + 1) * (ey - sy + 1)); i++)
	{
		LCD_Write_Data(color);
	}
}

//=============================================================================
// Fills whole screen specified color
//=============================================================================
void LCD_ClearScreen(int16_t color)
{
	unsigned int x, y;
	LCD_SetArea(0, LCD_WIDTH - 1, 0, LCD_HEIGHT - 1);
	LCD_Write_CMD(0x2c);
	for (x = 0; x < LCD_WIDTH; x++)
	{
		for (y = 0; y < LCD_HEIGHT; y++)
		{
			LCD_Write_Data(color);
		}
	}
}

void LCD_SetPixel(int16_t x, int16_t y, int16_t color)
{
	LCD_SetArea(x, x, y, y);
	LCD_Write_CMD(0x2c);
	LCD_Write_Data(color);
	LCD_Write_CMD(0x0);
}

void Init_SSD1963(void)
{
//	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
//
//	LCD_Write_COM(0xE2);  // PLL multiplier, set PLL clock to 120M
//	LCD_Write_Data(0x23); // N=0x36 for 6.5M, 0x23 for 10M crystal
//	LCD_Write_Data(0x02);
//	LCD_Write_Data(0x04);
//	LCD_Write_COM(0xE0); // PLL enable
//	LCD_Write_Data(0x01);
//	HAL_Delay(10);
//	LCD_Write_COM(0xE0);
//	LCD_Write_Data(0x03);
//	HAL_Delay(10);
//	LCD_Write_COM(0x01); // software reset
//	HAL_Delay(100);
//	LCD_Write_COM(0xE6); // PLL setting for PCLK, depends on resolution
//	// LCD_Write_Data(0x04);
//	// LCD_Write_Data(0x93);
//	// LCD_Write_Data(0xE0);
//		LCD_Write_Data(0x01);
//		LCD_Write_Data(0x33);
//		LCD_Write_Data(0x33);
//
//	LCD_Write_COM(0xB0);  // LCD SPECIFICATION
//	// LCD_Write_Data(0x00); // 0x24
//		LCD_Write_Data(0x20);
//	LCD_Write_Data(0x00);
//	// LCD_Write_Data(0x03); // Set HDP	799
//	// LCD_Write_Data(0x1F);
//	// LCD_Write_Data(0x01); // Set VDP	479
//	// LCD_Write_Data(0xDF);
//		LCD_Write_Data(((SSD1963_HOR_RES - 1) >> 8) & 0X00FF); // Set HDP	799
//		LCD_Write_Data((SSD1963_HOR_RES - 1) & 0X00FF);
//		LCD_Write_Data(((SSD1963_VER_RES - 1) >> 8) & 0X00FF); // Set VDP	479
//		LCD_Write_Data((SSD1963_VER_RES - 1) & 0X00FF);
//	LCD_Write_Data(0x00);
//	HAL_Delay(10);
//
//	// LCD_Write_COM(0xB4);  // HSYNC
//	// LCD_Write_Data(0x03); // Set HT	928
//	// LCD_Write_Data(0xA0);
//	// LCD_Write_Data(0x00); // Set HPS	46
//	// LCD_Write_Data(0x2E);
//	// LCD_Write_Data(0x30); // Set HPW	48
//	// LCD_Write_Data(0x00); // Set LPS	15
//	// LCD_Write_Data(0x0F);
//	// LCD_Write_Data(0x00);
//
//	LCD_Write_COM(0xB4);  // HSYNC
//	LCD_Write_Data((SSD1963_HT >> 8) & 0X00FF); // Set HT	928
//	LCD_Write_Data(SSD1963_HT & 0X00FF);
//	LCD_Write_Data((SSD1963_HPS >> 8) & 0X00FF); // Set HPS	46
//	LCD_Write_Data(SSD1963_HPS & 0X00FF);
//	LCD_Write_Data(SSD1963_HPW); // Set HPW	48
//	LCD_Write_Data((SSD1963_LPS >> 8) & 0X00FF); // Set LPS	15
//	LCD_Write_Data(SSD1963_LPS & 0X00FF);
//	LCD_Write_Data(0x00);
//
//	// LCD_Write_COM(0xB6);  // VSYNC
//	// LCD_Write_Data(0x02); // Set VT	525
//	// LCD_Write_Data(0x0D);
//	// LCD_Write_Data(0x00); // Set VPS	16
//	// LCD_Write_Data(0x10);
//	// LCD_Write_Data(0x10); // Set VPW	16
//	// LCD_Write_Data(0x00); // Set FPS	8
//	// LCD_Write_Data(0x08);
//
//	LCD_Write_COM(0xB6);  // VSYNC
//	LCD_Write_Data((SSD1963_VT >> 8) & 0X00FF); // Set HT	928
//	LCD_Write_Data(SSD1963_VT & 0X00FF);
//	LCD_Write_Data((SSD1963_VPS >> 8) & 0X00FF); // Set HPS	46
//	LCD_Write_Data(SSD1963_VPS & 0X00FF);
//	LCD_Write_Data(SSD1963_VPW); // Set HPW	48
//	LCD_Write_Data((SSD1963_FPS >> 8) & 0X00FF); // Set LPS	15
//	LCD_Write_Data(SSD1963_FPS & 0X00FF);
//
//	LCD_Write_COM(0x00B8);
//    LCD_Write_Data(0x000f);    //GPIO is controlled by host GPIO[3:0]=output   GPIO[0]=1  LCD ON  GPIO[0]=1  LCD OFF
//    LCD_Write_Data(0x0001);    //GPIO0 normal
//
//    LCD_Write_COM(0x00BA);
//    LCD_Write_Data(0x0001);    //GPIO[0] out 1 --- LCD display on/off control PIN
//
//    // LCD_Write_COM(0x0036);    //rotation
//    // LCD_Write_Data(0x0008);   //RGB=BGR
//
//    LCD_Write_COM(0x003A);    //Set the current pixel format for RGB image data
//    LCD_Write_Data(0x0050);   //16-bit/pixel
//
//    LCD_Write_COM(0x00F0);    //Pixel Data Interface Format
//    LCD_Write_Data(0x0003);   //16-bit(565 format) data
//
//    LCD_Write_COM(0x00BC);
//    LCD_Write_Data(0x0040);   //contrast value
//    LCD_Write_Data(0x0080);   //brightness value
//    LCD_Write_Data(0x0040);   //saturation value
//    LCD_Write_Data(0x0001);   //Post Processor Enable
//
//	// LCD_Write_COM(0xBA);
//	// LCD_Write_Data(0x05); // GPIO[3:0] out 1
//
//	// LCD_Write_COM(0xB8);
//	// LCD_Write_Data(0x07); // GPIO3=input, GPIO[2:0]=output
//	// LCD_Write_Data(0x01); // GPIO0 normal
//
//	// LCD_Write_COM(0xba);		//rotation
//	// LCD_Write_DATA(0x01);		// -- Set to 0x22 to rotate 180 degrees 0x21
//
//	// LCD_Write_COM(0xF0); // pixel Data interface
//	// LCD_Write_Data(0x03);
//
//	HAL_Delay(10);
//
//	LCD_Write_COM(0x29); // display on
//
//	LCD_Write_COM(0xBE); // set PWM for B/L
//	LCD_Write_Data(0x06);
//	// LCD_Write_Data(0xF0);
//	LCD_Write_Data(0x80);
//	LCD_Write_Data(0x01);
//	LCD_Write_Data(0xF0);
//	LCD_Write_Data(0x00);
//	LCD_Write_Data(0x00);
//
//	LCD_Write_COM(0xD0);
//	LCD_Write_Data(0x0D);
//
//	LCD_Write_COM(0x2C);
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);

			LCD_Write_COM(0xE2);  // PLL multiplier, set PLL clock to 120M
			LCD_Write_Data(0x23); // N=0x36 for 6.5M, 0x23 for 10M crystal
			LCD_Write_Data(0x02);
			LCD_Write_Data(0x04);
			LCD_Write_COM(0xE0); // PLL enable
			LCD_Write_Data(0x01);
			HAL_Delay(10);
			LCD_Write_COM(0xE0);
			LCD_Write_Data(0x03);
			HAL_Delay(10);
			LCD_Write_COM(0x01); // software reset
			HAL_Delay(100);
			LCD_Write_COM(0xE6); // PLL setting for PCLK, depends on resolution
			// LCD_Write_Data(0x04);
			// LCD_Write_Data(0x93);
			// LCD_Write_Data(0xE0);
				LCD_Write_Data(0x01);
				LCD_Write_Data(0x33);
				LCD_Write_Data(0x33);

			LCD_Write_COM(0xB0);  // LCD SPECIFICATION
			// LCD_Write_Data(0x00); // 0x24
				LCD_Write_Data(0x20);
			LCD_Write_Data(0x00);
			 LCD_Write_Data(0x03); // Set HDP	799
			 LCD_Write_Data(0x1F);
			 LCD_Write_Data(0x01); // Set VDP	479
			 LCD_Write_Data(0xDF);
	//			LCD_Write_Data(((SSD1963_HOR_RES - 1) >> 8) & 0X00FF); // Set HDP	799
	//			LCD_Write_Data((SSD1963_HOR_RES - 1) & 0X00FF);
	//			LCD_Write_Data(((SSD1963_VER_RES - 1) >> 8) & 0X00FF); // Set VDP	479
	//			LCD_Write_Data((SSD1963_VER_RES - 1) & 0X00FF);
			LCD_Write_Data(0x00);
			HAL_Delay(10);

			 LCD_Write_COM(0xB4);  // HSYNC
			 LCD_Write_Data(0x03); // Set HT	928
			 LCD_Write_Data(0xA0);
			 LCD_Write_Data(0x00); // Set HPS	46
			 LCD_Write_Data(0x2E);
			 LCD_Write_Data(0x30); // Set HPW	48
			 LCD_Write_Data(0x00); // Set LPS	15
			 LCD_Write_Data(0x0F);
			 LCD_Write_Data(0x00);

	//		LCD_Write_COM(0xB4);  // HSYNC
	//		LCD_Write_Data((SSD1963_HT >> 8) & 0X00FF); // Set HT	928
	//		LCD_Write_Data(SSD1963_HT & 0X00FF);
	//		LCD_Write_Data((SSD1963_HPS >> 8) & 0X00FF); // Set HPS	46
	//		LCD_Write_Data(SSD1963_HPS & 0X00FF);
	//		LCD_Write_Data(SSD1963_HPW); // Set HPW	48
	//		LCD_Write_Data((SSD1963_LPS >> 8) & 0X00FF); // Set LPS	15
	//		LCD_Write_Data(SSD1963_LPS & 0X00FF);
	//		LCD_Write_Data(0x00);

			 LCD_Write_COM(0xB6);  // VSYNC
			 LCD_Write_Data(0x02); // Set VT	525
			 LCD_Write_Data(0x0D);
			 LCD_Write_Data(0x00); // Set VPS	16
			 LCD_Write_Data(0x10);
			 LCD_Write_Data(0x10); // Set VPW	16
			 LCD_Write_Data(0x00); // Set FPS	8
			 LCD_Write_Data(0x08);

	//		LCD_Write_COM(0xB6);  // VSYNC
	//		LCD_Write_Data((SSD1963_VT >> 8) & 0X00FF); // Set HT	928
	//		LCD_Write_Data(SSD1963_VT & 0X00FF);
	//		LCD_Write_Data((SSD1963_VPS >> 8) & 0X00FF); // Set HPS	46
	//		LCD_Write_Data(SSD1963_VPS & 0X00FF);
	//		LCD_Write_Data(SSD1963_VPW); // Set HPW	48
	//		LCD_Write_Data((SSD1963_FPS >> 8) & 0X00FF); // Set LPS	15
	//		LCD_Write_Data(SSD1963_FPS & 0X00FF);

			LCD_Write_COM(0x00B8);
		    LCD_Write_Data(0x000f);    //GPIO is controlled by host GPIO[3:0]=output   GPIO[0]=1  LCD ON  GPIO[0]=1  LCD OFF
		    LCD_Write_Data(0x0001);    //GPIO0 normal

		    LCD_Write_COM(0x00BA);
		    LCD_Write_Data(0x0001);    //GPIO[0] out 1 --- LCD display on/off control PIN

		    // LCD_Write_COM(0x0036);    //rotation
		    // LCD_Write_Data(0x0008);   //RGB=BGR

		    LCD_Write_COM(0x003A);    //Set the current pixel format for RGB image data
		    LCD_Write_Data(0x0050);   //16-bit/pixel

		    LCD_Write_COM(0x00F0);    //Pixel Data Interface Format
		    LCD_Write_Data(0x0003);   //16-bit(565 format) data

		    LCD_Write_COM(0x00BC);
		    LCD_Write_Data(0x0040);   //contrast value
		    LCD_Write_Data(0x0080);   //brightness value
		    LCD_Write_Data(0x0040);   //saturation value
		    LCD_Write_Data(0x0001);   //Post Processor Enable

			// LCD_Write_COM(0xBA);
			// LCD_Write_Data(0x05); // GPIO[3:0] out 1

			// LCD_Write_COM(0xB8);
			// LCD_Write_Data(0x07); // GPIO3=input, GPIO[2:0]=output
			// LCD_Write_Data(0x01); // GPIO0 normal

			// LCD_Write_COM(0xba);		//rotation
			// LCD_Write_DATA(0x01);		// -- Set to 0x22 to rotate 180 degrees 0x21

			// LCD_Write_COM(0xF0); // pixel Data interface
			// LCD_Write_Data(0x03);

			HAL_Delay(10);

			LCD_Write_COM(0x29); // display on

			LCD_Write_COM(0xBE); // set PWM for B/L
			LCD_Write_Data(0x06);
			// LCD_Write_Data(0xF0);
			LCD_Write_Data(0x80);
			LCD_Write_Data(0x01);
			LCD_Write_Data(0xF0);
			LCD_Write_Data(0x00);
			LCD_Write_Data(0x00);

			LCD_Write_COM(0xD0);
			LCD_Write_Data(0x0D);

			LCD_Write_COM(0x2C);
}
//===========================================================================
//===========================================================================
//===========================================================================
// static __inline  - not work before define of function
void LCD_Send_CMD(uint8_t index)
{
	*(uint8_t *)(LCD_REG) = index;
}

void LCD_Set_X(uint16_t start_x, uint16_t end_x)
{
	LCD_Send_CMD(0x002A);
	LCD_Write_Data(start_x >> 8);
	LCD_Write_Data(start_x & 0x00ff);

	LCD_Write_Data(end_x >> 8);
	LCD_Write_Data(end_x & 0x00ff);
}

void LCD_Set_Y(uint16_t start_y, uint16_t end_y)
{
	LCD_Send_CMD(0x002B);
	LCD_Write_Data(start_y >> 8);
	LCD_Write_Data(start_y & 0x00ff);

	LCD_Write_Data(end_y >> 8);
	LCD_Write_Data(end_y & 0x00ff);
}

void LCD_Set_XY(uint16_t x, uint16_t y)
{
	LCD_Set_X(x, x);
	LCD_Set_Y(y, y);
}

void LCD_Set_Work_Area(uint16_t x, uint16_t y, uint16_t length, uint16_t width)
{
	LCD_Set_X(x, x + length - 1);
	LCD_Set_Y(y, y + width - 1);
	LCD_Send_CMD(0x2C);
}

void LCD_Clear_Screen(uint16_t color)
{
	uint32_t i = 0;
	LCD_Set_Work_Area(0, 0, 800, 480);
	for (i = 0; i < 384000; i++)
	{
		LCD_Write_Data(color); 
	}
}

void LCD_Draw_Char(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, const uint8_t *table, uint8_t ascii, uint8_t size)
{
	uint8_t i, f = 0;

	for (i = 0; i < 8; i++)
	{
		for (f = 0; f < 8; f++)
		{
			if ((*(table + 8 * (ascii - 0x20) + i) >> (7 - f)) & 0x01)
			{
				LCD_Draw_Fill_Rectangle(x + f * size, y + i * size, size, size, color);
			}
			else
			{
				LCD_Draw_Fill_Rectangle(x + f * size, y + i * size, size, size, phone);
			}
		}
	}
}

void LCD_Draw_String(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, const uint8_t *table, char *string, uint8_t size)
{
	while (*string)
	{
		if ((x + 8) > (LCD_WIDTH - 1))
		{
			x = 1;
			y = y + 8 * size;
		}
		LCD_Draw_Char(x, y, color, phone, table, *string, size); 
		x += 8 * size;											 
		*string++;												 
	}
}

void LCD_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	int error2 = 0;

	for (;;)
	{
		LCD_Draw_Fill_Rectangle(x1, y1, size, size, color);

		if (x1 == x2 && y1 == y2)
			break;

		error2 = error * 2;

		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}

		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void LCD_Draw_HLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color)
{
	uint16_t i = 0;

	LCD_Set_Work_Area(x, y, length, size);
	for (i = 0; i < (length * size); i++)
		LCD_Write_Data(color);
}

void LCD_Draw_VLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color)
{
	uint16_t i = 0;

	LCD_Set_Work_Area(x, y, size, length);
	for (i = 0; i < (length * size); i++)
		LCD_Write_Data(color);
}

void LCD_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t size, uint16_t color)
{
	LCD_Draw_HLine(x, y, length, size, color);
	LCD_Draw_HLine(x, y + width, length, size, color);
	LCD_Draw_VLine(x, y, width, size, color);
	LCD_Draw_VLine(x + length - size, y, width, size, color);
}

void LCD_Draw_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color)
{
	uint32_t i = 0;

	LCD_Set_Work_Area(x, y, length, width);
	for (i = 0; i < length * width; i++)
	{
		LCD_Write_Data(color); 
	}
}

void LCD_Draw_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color)
{

	LCD_Draw_Line(x1, y1, x2, y2, color, size);
	LCD_Draw_Line(x2, y2, x3, y3, color, size);
	LCD_Draw_Line(x3, y3, x1, y1, color, size);
}

void LCD_Draw_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint16_t color)
{
	int a_, b_, P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_)
	{
		if (fill == 1)
		{
			LCD_Draw_Fill_Rectangle(x - a_, y - b_, 2 * a_ + 1, 2 * b_ + 1, color);
			LCD_Draw_Fill_Rectangle(x - b_, y - a_, 2 * b_ + 1, 2 * a_ + 1, color);
		}
		else
		{
			LCD_Draw_Fill_Rectangle(a_ + x, b_ + y, size, size, color);
			LCD_Draw_Fill_Rectangle(b_ + x, a_ + y, size, size, color);
			LCD_Draw_Fill_Rectangle(x - a_, b_ + y, size, size, color);
			LCD_Draw_Fill_Rectangle(x - b_, a_ + y, size, size, color);
			LCD_Draw_Fill_Rectangle(b_ + x, y - a_, size, size, color);
			LCD_Draw_Fill_Rectangle(a_ + x, y - b_, size, size, color);
			LCD_Draw_Fill_Rectangle(x - a_, y - b_, size, size, color);
			LCD_Draw_Fill_Rectangle(x - b_, y - a_, size, size, color);
		}
		if (P < 0)
		{
			P = (P + 3) + (2 * a_);
			a_++;
		}
		else
		{
			P = (P + 5) + (2 * (a_ - b_));
			a_++;
			b_--;
		}
	}
}

void LCD_Draw_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4)
		{
			LCD_Draw_Fill_Rectangle(x0 + x, y0 + y, size, size, color);
			LCD_Draw_Fill_Rectangle(x0 + y, y0 + x, size, size, color);
		}
		if (cornername & 0x2)
		{
			LCD_Draw_Fill_Rectangle(x0 + x, y0 - y, size, size, color);
			LCD_Draw_Fill_Rectangle(x0 + y, y0 - x, size, size, color);
		}
		if (cornername & 0x8)
		{
			LCD_Draw_Fill_Rectangle(x0 - y, y0 + x, size, size, color);
			LCD_Draw_Fill_Rectangle(x0 - x, y0 + y, size, size, color);
		}
		if (cornername & 0x1)
		{
			LCD_Draw_Fill_Rectangle(x0 - y, y0 - x, size, size, color);
			LCD_Draw_Fill_Rectangle(x0 - x, y0 - y, size, size, color);
		}
	}
}

void LCD_Draw_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint16_t color)
{

	LCD_Draw_HLine(x + r, y, length - 2 * r, size, color);			   // Top
	LCD_Draw_HLine(x + r, y + width - 1, length - 2 * r, size, color); // Bottom
	LCD_Draw_VLine(x, y + r, width - 2 * r, size, color);			   // Left
	LCD_Draw_VLine(x + length - 1, y + r, width - 2 * r, size, color); // Right

	LCD_Draw_Circle_Helper(x + r, y + r, r, 1, size, color);
	LCD_Draw_Circle_Helper(x + length - r - 1, y + r, r, 2, size, color);
	LCD_Draw_Circle_Helper(x + length - r - 1, y + width - r - 1, r, 4, size, color);
	LCD_Draw_Circle_Helper(x + r, y + width - r - 1, r, 8, size, color);
}

void LCD_Draw_Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1)
		{
			LCD_Draw_VLine(x0 + x, y0 - y, 2 * y + 1 + delta, 1, color);
			LCD_Draw_VLine(x0 + y, y0 - x, 2 * x + 1 + delta, 1, color);
		}
		if (cornername & 0x2)
		{
			LCD_Draw_VLine(x0 - x, y0 - y, 2 * y + 1 + delta, 1, color);
			LCD_Draw_VLine(x0 - y, y0 - x, 2 * x + 1 + delta, 1, color);
		}
	}
}

void LCD_Draw_Fill_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint16_t color)
{
	LCD_Draw_Fill_Rectangle(x + r, y, length - 2 * r, width, color);

	LCD_Draw_Fill_Circle_Helper(x + length - r - 1, y + r, r, 1, width - 2 * r - 1, color);
	LCD_Draw_Fill_Circle_Helper(x + r, y + r, r, 2, width - 2 * r - 1, color);
}

static void LCD_Char(int16_t x, int16_t y, const GFXglyph *glyph, const GFXfont *font, uint8_t size, uint32_t color24)
{
	uint8_t *bitmap = font->bitmap;
	uint16_t bo = glyph->bitmapOffset;
	uint8_t bits = 0, bit = 0;
	uint16_t set_pixels = 0;
	uint8_t cur_x, cur_y;
	for (cur_y = 0; cur_y < glyph->height; cur_y++)
	{
		for (cur_x = 0; cur_x < glyph->width; cur_x++)
		{
			if (bit == 0)
			{
				bits = (*(const unsigned char *)(&bitmap[bo++]));
				bit = 0x80;
			}
			if (bits & bit)
			{
				set_pixels++;
			}
			else if (set_pixels > 0)
			{
				LCD_Draw_Fill_Rectangle(x + (glyph->xOffset + cur_x - set_pixels) * size, y + (glyph->yOffset + cur_y) * size, size * set_pixels, size, color24);
				set_pixels = 0;
			}
			bit >>= 1;
		}
		if (set_pixels > 0)
		{
			LCD_Draw_Fill_Rectangle(x + (glyph->xOffset + cur_x - set_pixels) * size, y + (glyph->yOffset + cur_y) * size, size * set_pixels, size, color24);
			set_pixels = 0;
		}
	}
}
void LCD_Font(uint16_t x, uint16_t y, char *text, const GFXfont *p_font, uint8_t size, uint32_t color24)
{
	int16_t cursor_x = x;
	int16_t cursor_y = y;
	GFXfont font;
	memcpy((&font), (p_font), (sizeof(GFXfont)));
	for (uint16_t text_pos = 0; text_pos < strlen(text); text_pos++)
	{
		char c = text[text_pos];
		if (c == '\n')
		{
			cursor_x = x;
			cursor_y += font.yAdvance * size;
		}
		else if (c >= font.first && c <= font.last && c != '\r')
		{
			GFXglyph glyph;
			memcpy((&glyph), (&font.glyph[c - font.first]), (sizeof(GFXglyph)));
			LCD_Char(cursor_x, cursor_y, &glyph, &font, size, color24);
			cursor_x += glyph.xAdvance * size;
		}
	}
}

/*********************
 *      LVGL START   *
 *********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    // /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    // /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
    //                       MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    Init_SSD1963();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	/*Return if the area is out the screen*/
    if(area->x2 < 0) return;
    if(area->y2 < 0) return;
    if(area->x1 > MY_DISP_HOR_RES - 1) return;
    if(area->y1 > MY_DISP_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > SSD1963_HOR_RES - 1 ? SSD1963_HOR_RES - 1 : area->x2;
    int32_t act_y2 = area->y2 > SSD1963_VER_RES - 1 ? SSD1963_VER_RES - 1 : area->y2;

	//Set the rectangular area
    LCD_Write_CMD(0x002A);
    LCD_Write_Data(act_x1 >> 8);
    LCD_Write_Data(0x00FF & act_x1);
    LCD_Write_Data(act_x2 >> 8);
    LCD_Write_Data(0x00FF & act_x2);

    LCD_Write_CMD(0x002B);
    LCD_Write_Data(act_y1 >> 8);
    LCD_Write_Data(0x00FF & act_y1);
    LCD_Write_Data(act_y2 >> 8);
    LCD_Write_Data(0x00FF & act_y2);

    LCD_Write_CMD(0x2c);
    int16_t i;
    uint16_t full_w = area->x2 - area->x1 + 1;

    ssd1963_data_mode();
	LV_DRV_DISP_PAR_CS(0);
	uint16_t act_w = act_x2 - act_x1 + 1;
	for(i = act_y1; i <= act_y2; i++) {
		LV_DRV_DISP_PAR_WR_ARRAY((uint16_t *)color_p, act_w);
		color_p += full_w;
	}
	LV_DRV_DISP_PAR_CS(1);

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);

}
