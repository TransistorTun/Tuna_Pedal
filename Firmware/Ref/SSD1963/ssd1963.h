#ifndef DISPLAY_SSD1963
#define DISPLAY_SSD1963

#include "stm32f4xx_hal.h"
#include <stdlib.h>

// Adress for data
#define LCD_DATA 0x60020000
// Adress for command
#define LCD_REG 0x60000000

#define SSD1963_HOR_RES     LV_HOR_RES
#define SSD1963_VER_RES     LV_VER_RES

#define SSD1963_NOP						0x00
#define SSD1963_SOFT_RESET  			0x01
#define SSD1963_GET_POWER_MODE 			0x0A
#define SSD1963_GET_ADDRESS_MODE		0x0B
#define SSD1963_GET_DISPLAY_MODE		0x0D
#define SSD1963_GET_TEAR_EFFECT_STATUS 	0x0E
#define SSD1963_ENTER_SLEEP_MODE		0x10
#define SSD1963_EXIT_SLEEP_MODE			0x11
#define SSD1963_ENTER_PARTIAL_MODE		0x12
#define SSD1963_ENTER_NORMAL_MODE		0x13
#define SSD1963_EXIT_INVERT_MODE		0x20
#define SSD1963_ENTER_INVERT_MODE		0x21
#define SSD1963_SET_GAMMA_CURVE			0x26
#define SSD1963_SET_DISPLAY_OFF			0x28
#define SSD1963_SET_DISPLAY_ON			0x29
#define SSD1963_SET_COLUMN_ADDRESS		0x2A
#define SSD1963_SET_PAGE_ADDRESS		0x2B
#define SSD1963_WRITE_MEMORY_START		0x2C
#define SSD1963_READ_MEMORY_START		0x2E
#define SSD1963_SET_PARTIAL_AREA		0x30
#define SSD1963_SET_SCROLL_AREA			0x33
#define SSD1963_SET_TEAR_OFF			0x34
#define SSD1963_SET_REAR_ON				0x35
#define SSD1963_SET_ADDRESS_MODE		0x36
#define SSD1963_SET_SCROLL_START		0x37
#define SSD1963_EXIT_IDLE_MODE			0x38
#define SSD1963_ENTER_IDLE_MODE			0x39
#define SSD1963_WRITE_MEMORY_CONTINUE	0x3C
#define SSD1963_READ_MEMORY_CONTINUE	0x3E
#define SSD1963_SET_TEAR_SCANLINE		0x44
#define SSD1963_GET_SCANLINE			0x45
#define SSD1963_READ_DDB				0xA1
#define SSD1963_SET_LCD_MODE			0xB0
#define SSD1963_GET_LCD_MODE			0xB1
#define SSD1963_SET_HORI_PERIOD			0xB4
#define SSD1963_GET_HORI_PERIOD			0xB5
#define SSD1963_SET_VERT_PERIOD			0xB6
#define SSD1963_GET_VERT_PERIOD			0xB7
#define SSD1963_SET_GPIO_CONF			0xB8
#define SSD1963_GET_GPIO_CONF			0xB9
#define SSD1963_SET_GPIO_VALUE			0xBA
#define SSD1963_GET_GPIO_STATUS			0xBB
#define SSD1963_SET_POST_PROC			0xBC
#define SSD1963_GET_POST_PROC			0xBD
#define SSD1963_SET_PWM_CONF			0xBE
#define SSD1963_GET_PWM_CONF			0xBF
#define SSD1963_GET_LCD_GEN0			0xC0
#define SSD1963_SET_LCD_GEN0			0xC1
#define SSD1963_GET_LCD_GEN1			0xC2
#define SSD1963_SET_LCD_GEN1			0xC3
#define SSD1963_GET_LCD_GEN2			0xC4
#define SSD1963_SET_LCD_GEN2			0xC5
#define SSD1963_GET_LCD_GEN3			0xC6
#define SSD1963_SET_LCD_GEN3			0xC7
#define SSD1963_SET_GPIO0_ROP			0xC8
#define SSD1963_GET_GPIO0_ROP			0xC9
#define SSD1963_SET_GPIO1_ROP			0xCA
#define SSD1963_GET_GPIO1_ROP			0xCB
#define SSD1963_SET_GPIO2_ROP			0xCC
#define SSD1963_GET_GPIO2_ROP			0xCD
#define SSD1963_SET_GPIO3_ROP			0xCE
#define SSD1963_GET_GPIO3_ROP			0xCF
#define SSD1963_SET_DBC_CONF			0xD0
#define SSD1963_GET_DBC_CONF			0xD1
#define SSD1963_SET_DBC_TH				0xD4
#define SSD1963_GET_DBC_TH				0xD5
#define SSD1963_SET_PLL					0xE0
#define SSD1963_SET_PLL_MN				0xE2
#define SSD1963_GET_PLL_MN				0xE3
#define SSD1963_GET_PLL_STATUS			0xE4
#define SSD1963_SET_DEEP_SLEEP			0xE5
#define SSD1963_SET_LSHIFT_FREQ			0xE6
#define SSD1963_GET_LSHIFT_FREQ			0xE7
#define SSD1963_SET_PIXEL_DATA_INTERFACE 0xF0
#define SSD1963_PDI_8BIT				0
#define SSD1963_PDI_12BIT				1
#define SSD1963_PDI_16BIT				2
#define SSD1963_PDI_16BIT565			3
#define SSD1963_PDI_18BIT				4
#define SSD1963_PDI_24BIT				5
#define SSD1963_PDI_9BIT				6
#define SSD1963_GET_PIXEL_DATA_INTERFACE 0xF1

#define LCD_WIDTH				800ULL
#define LCD_HSYNC_BACK_PORCH	30LL
#define LCD_HSYNC_FRONT_PORCH	0ULL
#define LCD_HSYNC_PULSE			0ULL

#define LCD_HEIGHT				480ULL
#define LCD_VSYNC_BACK_PORCH	10ULL
#define LCD_VSYNC_FRONT_PORCH	0ULL
#define LCD_VSYNC_PULSE			0ULL

#define GREY	  			0x8410
#define YELLOW   			0xFFE0
#define	RED	  				0XF800
#define	GREEN	  			0X07E0
#define	BLUE      		0X001F
#define BLACK     		0X0000
#define WHITE     		0XFFFF
#define DARK_BLUE 		0X0007


typedef struct { // Data stored PER GLYPH
	uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor position to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint8_t   first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;




extern const uint8_t font8x8[][8];

uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);

void Init_SSD1963(void);
void LCD_Write_Cmd(uint16_t Cmd);
void LCD_Write_Data(uint16_t data);
void LCD_SetArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey);
void LCD_FillArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, int16_t color);
void LCD_SetPixel(int16_t x, int16_t y, int16_t color);
void LCD_ClearScreen(int16_t color);

//void LCD_SetPixel(int16_t x, int16_t y, int16_t color);

void LCD_Set_XY(uint16_t x, uint16_t y);
void LCD_Set_Work_Area(uint16_t x, uint16_t y, uint16_t length, uint16_t width);


void LCD_Clear_Screen(uint16_t color);
void LCD_Draw_Char(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, const uint8_t *table, uint8_t ascii, uint8_t size);
void LCD_Draw_String(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, const uint8_t *table, char *string, uint8_t size);

void LCD_Draw_Line (uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t size,uint16_t color);
void LCD_Draw_HLine(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color);
void LCD_Draw_VLine(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color);

void LCD_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t size, uint16_t color);
void LCD_Draw_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color);

void LCD_Draw_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint16_t color);
void LCD_Draw_Fill_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint16_t color);

void LCD_Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color);
void LCD_Draw_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint16_t color);
void LCD_Draw_Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) ;

void LCD_Font(uint16_t x, uint16_t y, char *text, const GFXfont *p_font, uint8_t size, uint32_t color24);

//=============================================================================
// SSD1963 commands
//=============================================================================


#endif
