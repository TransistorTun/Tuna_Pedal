/*
 * LCD_Touch.c
 *
 *  Created on: Apr 9, 2024
 *      Author: arunrawat
 */



#include "LCD_Touch.h"
extern I2C_HandleTypeDef hi2c1;

 void     TS_IO_Init(void)
 {
	 HAL_GPIO_WritePin(CTP_RESET_GPIO_Port, CTP_RESET_Pin, GPIO_PIN_RESET);
	 HAL_Delay(10);
	 HAL_GPIO_WritePin(CTP_RESET_GPIO_Port, CTP_RESET_Pin, GPIO_PIN_SET);
	 HAL_Delay(10);
 }

 void     TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
 {
	 HAL_I2C_Mem_Write(&hi2c1, Addr, Reg, 1, &Value, 1, 100);
 }

 uint8_t  TS_IO_Read(uint8_t Addr, uint8_t Reg)
 {
	 uint8_t data;
	 HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, 1, &data, 1, 100);
	 return data;
 }

 uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
 {
	 return (HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, 1, Buffer, Length, 2000));
 }

 void     TS_IO_Delay(uint32_t Delay)
 {
	 HAL_Delay(Delay);
 }


 /***************************** LVGL START *************************************/

 /*********************
  *      INCLUDES
  *********************/
 #include "lvgl.h"
#include "ft6x06.h"

 /*********************
  *      DEFINES
  *********************/

 /**********************
  *      TYPEDEFS
  **********************/

 /**********************
  *  STATIC PROTOTYPES
  **********************/

 static void touchpad_init(void);
 static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
 static bool touchpad_is_pressed(void);
 static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

 /**********************
  *  STATIC VARIABLES
  **********************/
 lv_indev_t * indev_touchpad;


 /**********************
  *      MACROS
  **********************/

 /**********************
  *   GLOBAL FUNCTIONS
  **********************/

 void lv_port_indev_init(void)
 {
     /**
      * Here you will find example implementation of input devices supported by LittelvGL:
      *  - Touchpad
      *  - Mouse (with cursor support)
      *  - Keypad (supports GUI usage only with key)
      *  - Encoder (supports GUI usage only with: left, right, push)
      *  - Button (external buttons to press points on the screen)
      *
      *  The `..._read()` function are only examples.
      *  You should shape them according to your hardware
      */

     static lv_indev_drv_t indev_drv;

     /*------------------
      * Touchpad
      * -----------------*/

     /*Initialize your touchpad if you have*/
     touchpad_init();

     /*Register a touchpad input device*/
     lv_indev_drv_init(&indev_drv);
     indev_drv.type = LV_INDEV_TYPE_POINTER;
     indev_drv.read_cb = touchpad_read;
     indev_touchpad = lv_indev_drv_register(&indev_drv);
 }

 /**********************
  *   STATIC FUNCTIONS
  **********************/

 /*------------------
  * Touchpad
  * -----------------*/

 /*Initialize your touchpad*/
 static void touchpad_init(void)
 {
     ft6x06_Init(0x70);
     ft6x06_TS_Start(0x70);
 }

 /*Will be called by the library to read the touchpad*/
 static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
 {
     static lv_coord_t last_x = 0;
     static lv_coord_t last_y = 0;

     /*Save the pressed coordinates and the state*/
     if(touchpad_is_pressed()) {
         touchpad_get_xy(&last_x, &last_y);
         data->state = LV_INDEV_STATE_PR;
     }
     else {
         data->state = LV_INDEV_STATE_REL;
     }

     /*Set the last pressed coordinates*/
     data->point.x = last_x;
     data->point.y = last_y;
 }

 /*Return true is the touchpad is pressed*/
 static bool touchpad_is_pressed(void)
 {
     if (ft6x06_TS_DetectTouch(0x70))
     {
    	 return true;
     }

     return false;
 }

 /*Get the x and y coordinates if the touchpad is pressed*/
 static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
 {
	 ft6x06_TS_GetXY(0x70, (uint16_t *)x, (uint16_t *)y);
	 int xx = *x;
	 int yy = *y;
	 *y = 240-xx;
	 *x = yy;
 }
