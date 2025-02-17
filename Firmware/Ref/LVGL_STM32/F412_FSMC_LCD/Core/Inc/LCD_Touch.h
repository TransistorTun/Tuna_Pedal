/*
 * LCD_Touch.h
 *
 *  Created on: Apr 9, 2024
 *      Author: arunrawat
 */

#ifndef INC_LCD_TOUCH_H_
#define INC_LCD_TOUCH_H_

#include "main.h"

 void     TS_IO_Init(void);
 void     TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
 uint8_t  TS_IO_Read(uint8_t Addr, uint8_t Reg);
 uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
 void     TS_IO_Delay(uint32_t Delay);


 /*********************************************************************************/

 /**
  * @file lv_port_indev_templ.h
  *
  */

 /*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
 #if 1

 #ifndef LV_PORT_INDEV_TEMPL_H
 #define LV_PORT_INDEV_TEMPL_H

 #ifdef __cplusplus
 extern "C" {
 #endif

 /*********************
  *      INCLUDES
  *********************/
 #include "lvgl.h"

 /*********************
  *      DEFINES
  *********************/

 /**********************
  *      TYPEDEFS
  **********************/

 /**********************
  * GLOBAL PROTOTYPES
  **********************/
 void lv_port_indev_init(void);

 /**********************
  *      MACROS
  **********************/

 #ifdef __cplusplus
 } /*extern "C"*/
 #endif

 #endif /*LV_PORT_INDEV_TEMPL_H*/

 #endif /*Disable/Enable content*/

#endif /* INC_LCD_TOUCH_H_ */
