/*
 * App.h
 *
 *  Created on: Nov 14, 2024
 *      Author: thanh
 */
#if 1
#ifndef APP_APP_H_
#define APP_APP_H_

#define NUM_OF_POINT 2000
#include <stdbool.h>
#include <stdint.h>
#include "System.h"
typedef enum {
	EditHV = 0,
	EditHOn,
	EditHOf,
	EditHPosCount,
	EditHNegCount,
	EditLV,
	EditLOn,
	EditLoff,
	EditLPosCount,
	EditLNegCount,
	EditH_DPN,
	EditL_DPN,
	EditDHL
} e_edit_index;

void Chart_Init();
void Numpad_Init();
uint16_t Draw_Rect(uint16_t x_coordinates, uint16_t y_coordinates, uint8_t Ton,
		uint8_t Toff, uint8_t delay, uint8_t NegCount, uint8_t PosCount,
		uint8_t isHigh);
void Communicate_Task();
void show_message(const char *message, uint32_t timeout_ms);
bool ValidateSequence(s_sequence *seq);
void simulate_dropdown_change(uint8_t new_value);
void create_sequence_buttons();
int get_next_file_index(const char *dir_path);

#endif /* APP_APP_H_ */
#endif
