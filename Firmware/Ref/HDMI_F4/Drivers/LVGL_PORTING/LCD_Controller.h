/*
 * LCD_Controller.h
 *
 *  Created on: Oct 15, 2024
 *      Author: thanh
 */

#ifndef INC_LCD_CONTROLLER_H_
#define INC_LCD_CONTROLLER_H_

/**
 * @file lv_port_disp_templ.h
 *
 */

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
/* Initialize low level display driver */
void lv_port_disp_init(void);

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void);

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void);

/**********************
 *      MACROS
 **********************/



#endif /* INC_LCD_CONTROLLER_H_ */
