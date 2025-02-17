/*
 * System.c
 *
 *  Created on: Nov 5, 2024
 *      Author: thanh
 */

#include <stdbool.h>
#include "main.h"
#include "System.h"

volatile uint16_t Run_Time_Tick = 0;
volatile uint32_t Run_Time = 0;
volatile uint16_t POWER_OFF_COUNT = 500;
e_system_state SystemStatus;
uint8_t BackLight = 100;

uint16_t File_Count = 0;

s_sequence Sequence[20];

s_sys_para Running_Paramater = {
		.Status = READYs,
};


