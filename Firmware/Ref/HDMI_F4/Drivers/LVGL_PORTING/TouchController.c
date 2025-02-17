/*
 * TouchController.c
 *
 *  Created on: Oct 23, 2024
 *      Author: thanh
 */

/*********************
 *      INCLUDES
 *********************/
#include "TouchController.h"
#include "lvgl.h"
#include "main.h"
#include "GT911.h"
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
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t *x, lv_coord_t *y);

static void button_init(void);
static void button_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t *indev_touchpad;
lv_indev_t *indev_button;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void) {
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

	/*------------------
	 * Button
	 * -----------------*/

	/*Initialize your button if you have*/
//    button_init();
//
//    /*Register a button input device*/
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_BUTTON;
//    indev_drv.read_cb = button_read;
//    indev_button = lv_indev_drv_register(&indev_drv);
//
//    /*Assign buttons to points on the screen*/
//    static const lv_point_t btn_points[2] = {
//        {10, 10},   /*Button 0 -> x:10; y:10*/
//        {40, 100},  /*Button 1 -> x:40; y:100*/
//    };
//    lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/
static void touchpad_init(void) {
	GT911_Config_t sampleConfig = { .X_Resolution = 800, .Y_Resolution = 480,
			.Number_Of_Touch_Support = 1, .ReverseY = false, .ReverseX = false,
			.SwithX2Y = true, .SoftwareNoiseReduction = true };

	GT911_Init(sampleConfig);
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
	static lv_coord_t last_x = 0;
	static lv_coord_t last_y = 0;

	/*Save the pressed coordinates and the state*/
	if (touchpad_is_pressed()) {
		touchpad_get_xy(&last_x, &last_y);
		data->state = LV_INDEV_STATE_PR;
	} else {
		data->state = LV_INDEV_STATE_REL;
	}

	/*Set the last pressed coordinates*/
	data->point.x = last_x;
	data->point.y = last_y;
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void) {
	/*Your code comes here*/
	uint8_t StatusRegister;
	GT911_GetStatus(&StatusRegister);
	if((StatusRegister & 0x80) != 0) {
		GT911_SetStatus(0);
		if(StatusRegister & 0x07)
			return true;
	}
	return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t *x, lv_coord_t *y) {
	/*Your code comes here*/
	GT911_ReadTouchPoint(x, y);
}

/*------------------
 * Button
 * -----------------*/

/*Initialize your buttons*/
static void button_init(void) {
	/*Your code comes here*/
}

/*Will be called by the library to read the button*/
static void button_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {

	static uint8_t last_btn = 0;

	/*Get the pressed button's ID*/
	int8_t btn_act = button_get_pressed_id();

	if (btn_act >= 0) {
		data->state = LV_INDEV_STATE_PR;
		last_btn = btn_act;
	} else {
		data->state = LV_INDEV_STATE_REL;
	}

	/*Save the last pressed button's ID*/
	data->btn_id = last_btn;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void) {
	uint8_t i;

	/*Check to buttons see which is being pressed (assume there are 2 buttons)*/
	for (i = 0; i < 2; i++) {
		/*Return the pressed button's ID*/
		if (button_is_pressed(i)) {
			return i;
		}
	}

	/*No button pressed*/
	return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id) {

	/*Your code comes here*/

	return false;
}

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
