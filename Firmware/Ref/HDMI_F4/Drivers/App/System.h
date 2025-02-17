/*
 * System.h
 *
 *  Created on: Nov 5, 2024
 *      Author: thanh
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#define DEFAUT_HS_MAX_VOLT 300
#define DEFAUT_LS_MAX_VOLT 50

#define DEFAUT_HS_MAX_ONTIME	20		//ms
#define DEFAUT_LS_MAX_ONTIME	1000	//ms

#define DEFAUT_HS_MAX_OFFTIME	20		//ms
#define DEFAUT_LS_MAX_OFFTIME	1000	//ms

#define DEFAUT_HS_MAX_PCOUNT	20		//POSITIVE PULSE COUNT
#define DEFAUT_HS_MAX_NCOUNT	20		//NEGATIVE PULSE COUNT

#define DEFAUT_LS_MAX_PCOUNT	20		//POSITIVE PULSE COUNT
#define DEFAUT_LS_MAX_NCOUNT	20		//NEGATIVE PULSE COUNT

#define DEFAUT_HS_DELAY_PN		20
#define DEFAUT_LS_DELAY_PN		20
#define DEFAUT_HLS_DELAY		20

typedef enum {OFF = 0, SETTING, RUNNING, FAIL} e_channel_state;

typedef enum {POWER_OFF, READYs, CHARGINGs, WAITs, RUNs, ERRORs, DISCONNECT} e_system_state;

typedef struct {
	uint16_t NumPos;
	uint16_t NumNeg;
	uint16_t OnTime;
	uint16_t OffTime;
	uint16_t Delay;
} s_pulse_status;

typedef struct {
	uint8_t Positive_Pole;
	uint8_t Negative_Pole;
	s_pulse_status HighSide;
	s_pulse_status LowSide;
	uint8_t Delay;
} s_sequence;

typedef struct {
	uint16_t HS_SetVolt;
	uint16_t HS_CurrentVolt;
	uint16_t LS_SetVolt;
	uint16_t LS_CurrentVolt;
	uint16_t Current;
	uint16_t Impedance;
	uint8_t  Sequence_Index;
	uint8_t  Total_Sequence;
	uint16_t Total_High_Pulse;
	uint16_t Total_Low_Pulse;
	e_system_state Status;
} s_sys_para;

extern e_system_state SystemStatus;
extern uint8_t BackLight;
extern s_sequence Sequence[20];
extern s_sys_para Running_Paramater;
extern uint16_t File_Count;
#endif /* INC_SYSTEM_H_ */
