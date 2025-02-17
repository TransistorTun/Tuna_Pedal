/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <string.h>
#include <stdlib.h>
#include "fsp_frame.h"
#include "fsp_line_task.h"
#include "stdint.h"
#include "Communicate.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

//static void convertTemperature(float temp, uint8_t buf[]);
//static void convertIntegerToBytes(int number, uint8_t arr[]);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
 extern double   compensated_pressure;
 extern double   compensated_temperature;

 float           temp;
 uint32_t        press;
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t hs_relay_pole, ls_relay_pole, relay_state;
void FSP_Line_Process() {
	switch (ps_FSP_RX->CMD) {
	case FSP_CMD_GET_ALL:
		Get_All_Received();
		break;

	case FSP_CMD_SET_ALL:
		break;

	case FSP_CMD_SEQUENCE:

		break;
	case FSP_CMD_KILL:

		break;

	case FSP_CMD_MEASURE_BMP:

		break;

	case FSP_CMD_MEASURE_LSM:

		break;
	case FSP_CMD_CONFIRM:
		Confirm_Sequence = ps_FSP_RX->Payload.sequence_confirm.sequence_index;
		is_Confirm = true;
		break;
	}
}

void fsp_print(uint8_t packet_length) {
	s_FSP_TX_Packet.sod = FSP_PKT_SOD;
	s_FSP_TX_Packet.src_adr = fsp_my_adr;
	s_FSP_TX_Packet.dst_adr = FSP_ADR_GPC;
	s_FSP_TX_Packet.length = packet_length;
	s_FSP_TX_Packet.type = FSP_PKT_TYPE_CMD_W_DATA;
	s_FSP_TX_Packet.eof = FSP_PKT_EOF;
	s_FSP_TX_Packet.crc16 = crc16_CCITT(FSP_CRC16_INITIAL_VALUE,
			&s_FSP_TX_Packet.src_adr, s_FSP_TX_Packet.length + 4);

	uint8_t encoded_frame[100] = { 0 };
	uint8_t frame_len;
	fsp_encode(&s_FSP_TX_Packet, encoded_frame, &frame_len);

	usart2_send_array((const char*)encoded_frame,frame_len);
//	UART_FSP(&GPC_UART, (char*) encoded_frame, frame_len);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
