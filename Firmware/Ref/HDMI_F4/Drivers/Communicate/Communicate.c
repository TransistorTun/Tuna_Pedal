/*
 * Communicate.c
 *
 *  Created on: Dec 1, 2024
 *      Author: thanh
 */

#include "Communicate.h"
#include "main.h"
#include "AppDrv.h"
#include "fsp_frame.h"
#include "System.h"
#include "ui.h"

uint8_t Communicate_Timeout = COMMUNICATE_TIMEOUT;
e_comm_state Request_State = idle;
uint8_t Pulsing_state = 0;
bool is_Connect = true;
bool is_Pulse_Running = 0;

bool is_Confirm = false;
bool is_Sequence_Send = false;
uint8_t Send_Sequence_Index = 0;
uint8_t Total_Sequence = 0;
uint8_t Confirm_Sequence = 0;

static void Set_Pulse_Run();
static void Set_Cap_Control();
static void Send_Sequence(uint8_t s_index);

void Communicate_Task() {
	if (is_Connect) {
		switch (Request_State) {
		case idle:
			Get_All_Send();
			Communicate_Timeout = COMMUNICATE_TIMEOUT;
			Request_State = request;
			break;

		case request:
			Request_State = wait_respone;
			break;

		case wait_respone:
			if (Communicate_Timeout > 0)
				Communicate_Timeout--;
			else {
				Request_State = time_out;				//timeout
			}
			break;

		case response:
			if (Running_Paramater.Status == DISCONNECT)
				Running_Paramater.Status = READYs;
			Request_State = idle;
			break;

		case time_out:
			Request_State = idle;
			Running_Paramater.Status = DISCONNECT;
			break;
		}
	}
	switch (Running_Paramater.Status) {
	case POWER_OFF:

		break;

	case READYs:

		break;

	case CHARGINGs:
		if (Running_Paramater.HS_CurrentVolt
				< (Running_Paramater.HS_SetVolt * 99 / 100))
			break;
		if (Running_Paramater.LS_CurrentVolt
				< (Running_Paramater.LS_SetVolt * 99 / 100))
			break;
		Set_Pulse_Run();
		Pulsing_state = 0;
		is_Pulse_Running = false;
		Running_Paramater.Status = RUNs;
		break;
	case WAITs:
		if (Total_Sequence > 0) {
			if (!is_Sequence_Send) {
				Send_Sequence(Send_Sequence_Index++);
				is_Sequence_Send = true;
			} else {
				if (!is_Confirm)
					break;
				else {
					Total_Sequence--;
					is_Sequence_Send = false;
				}
			}
		} else {
			Set_Cap_Control();
			Running_Paramater.Status = CHARGINGs;
		}
		break;
	case RUNs:
		if (!is_Pulse_Running)
			break;
		if (Pulsing_state) {
			Running_Paramater.Status = READYs;
		}
		break;
	case ERRORs:
		break;

	case DISCONNECT:
		break;
	}
}

void Get_All_Send() {
	ps_FSP_TX->CMD = FSP_CMD_GET_ALL;
	fsp_print(1);
}

void Get_All_Received() {
	Request_State = response;
	Running_Paramater.Current =
			(uint16_t) (ps_FSP_RX->Payload.get_all.current_high) << 8;
	Running_Paramater.Current |= ps_FSP_RX->Payload.get_all.current_low;

	Running_Paramater.HS_CurrentVolt =
			(uint16_t) (ps_FSP_RX->Payload.get_all.hv_volt_high) << 8;
	Running_Paramater.HS_CurrentVolt |= ps_FSP_RX->Payload.get_all.hv_volt_low;

	Running_Paramater.LS_CurrentVolt = ps_FSP_RX->Payload.get_all.lv_volt;

	Running_Paramater.Impedance =
			(uint16_t) (ps_FSP_RX->Payload.get_all.impedance_high) << 8;
	Running_Paramater.Impedance |= ps_FSP_RX->Payload.get_all.impedance_low;

	if (Running_Paramater.Status == RUNs) {
		if (!is_Pulse_Running) {
			if ((ps_FSP_RX->Payload.get_all.state & 0x10) == 0x10)
				is_Pulse_Running = true;
		} else {
			if ((ps_FSP_RX->Payload.get_all.state & 0x10) != 0x10)
				Pulsing_state = true;
		}

	}

}
static void Set_Pulse_Run() {
	ps_FSP_TX->CMD = FSP_CMD_SEQUENCE;
	ps_FSP_TX->Payload.set_sequence.state = 0x07;
	fsp_print(2);
}
static void Set_Cap_Control() {
	ps_FSP_TX->CMD = FSP_CMD_SEQUENCE;
	ps_FSP_TX->Payload.set_sequence.state = 0x03;
	fsp_print(2);
}

static void Send_Sequence(uint8_t s_index) {
	s_sequence *sequence = &Sequence[s_index];
	ps_FSP_TX->CMD = FSP_CMD_SET_ALL;

	ps_FSP_TX->Payload.set_all.sequence_index = s_index;
	ps_FSP_TX->Payload.set_all.pole = sequence->Positive_Pole << 4;
	ps_FSP_TX->Payload.set_all.pole |= sequence->Negative_Pole;

	ps_FSP_TX->Payload.set_all.hs_volt_high = Running_Paramater.HS_SetVolt >> 8;
	ps_FSP_TX->Payload.set_all.hs_volt_low = Running_Paramater.HS_SetVolt;

	ps_FSP_TX->Payload.set_all.ls_volt = Running_Paramater.LS_SetVolt;

	ps_FSP_TX->Payload.set_all.hv_OffTime = sequence->HighSide.OffTime;
	ps_FSP_TX->Payload.set_all.hv_OnTime = sequence->HighSide.OnTime;

	ps_FSP_TX->Payload.set_all.lv_OffTime_high = sequence->LowSide.OffTime >> 8;
	ps_FSP_TX->Payload.set_all.lv_OffTime_low = sequence->LowSide.OffTime;

	ps_FSP_TX->Payload.set_all.lv_OnTime_high = sequence->LowSide.OnTime >> 8;
	ps_FSP_TX->Payload.set_all.lv_OnTime_low = sequence->LowSide.OnTime;

	ps_FSP_TX->Payload.set_all.hv_delay = sequence->HighSide.Delay;
	ps_FSP_TX->Payload.set_all.lv_delay = sequence->LowSide.Delay;
	ps_FSP_TX->Payload.set_all.delay_high = 0;
	ps_FSP_TX->Payload.set_all.delay_low = sequence->Delay + 1;

	if(ps_FSP_TX->Payload.set_all.hv_delay == 0)
		ps_FSP_TX->Payload.set_all.hv_delay = 1;
	if(ps_FSP_TX->Payload.set_all.lv_delay == 0)
		ps_FSP_TX->Payload.set_all.lv_delay = 1;
	if(ps_FSP_TX->Payload.set_all.delay_low == 0)
		ps_FSP_TX->Payload.set_all.delay_low = 1;

	ps_FSP_TX->Payload.set_all.hv_pos_count = sequence->HighSide.NumPos;
	ps_FSP_TX->Payload.set_all.hv_neg_count = sequence->HighSide.NumNeg;

	ps_FSP_TX->Payload.set_all.lv_pos_count = sequence->LowSide.NumPos;
	ps_FSP_TX->Payload.set_all.lv_neg_count = sequence->LowSide.NumNeg;
	fsp_print(20);
}
