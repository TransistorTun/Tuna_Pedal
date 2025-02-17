/*
 * Communicate.h
 *
 *  Created on: Dec 1, 2024
 *      Author: thanh
 */

#ifndef COMMUNICATE_COMMUNICATE_H_
#define COMMUNICATE_COMMUNICATE_H_

#include <stdbool.h>
#include <stdint.h>

#define COMMUNICATE_TIMEOUT  50

extern bool is_Confirm;
extern bool is_Sequence_Send;
extern uint8_t Send_Sequence_Index;
extern uint8_t Total_Sequence;
extern uint8_t Confirm_Sequence;
typedef enum _e_communicate_ {
	idle = 0,
	request,
	wait_respone,
	response,
	time_out,
} e_comm_state;

void Communicate_Task();
void Get_All_Send();
void Get_All_Received();
#endif /* COMMUNICATE_COMMUNICATE_H_ */
