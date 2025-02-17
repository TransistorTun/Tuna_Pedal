/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "fsp_line_task.h"
#include "fsp_frame.h"
#include "fsp.h"
#include "RingBuffer.h"
#include "UART.h"
#include <string.h>
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define FSP_BUF_LEN 64
typedef struct _fsp_line_typedef {
	uint16_t buffer_size;
	char *p_buffer;

	volatile uint16_t write_index;
} fsp_line_typedef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static const char *ErrorCode[7] = { "FSP_PKT_RECEIVED_OK\n",
		"FSP_PKT_NOT_READY\n", "FSP_PKT_INVALID\n", "FSP_PKT_WRONG_ADR\n",
		"FSP_PKT_ERROR\n", "FSP_PKT_CRC_FAIL\n", "FSP_PKT_WRONG_LENGTH\n" };

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

fsp_packet_t s_FSP_TX_Packet;
fsp_packet_t s_FSP_RX_Packet;
FSP_Payload *ps_FSP_TX = (FSP_Payload*) (&s_FSP_TX_Packet.payload);
FSP_Payload *ps_FSP_RX = (FSP_Payload*) (&s_FSP_RX_Packet.payload);

fsp_line_typedef FSP_line;
char g_FSP_line_buffer[FSP_BUF_LEN];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: FSP Line Task Init :::::::: */
void FSP_Line_Task_Init() {
	usart2_init();
	FSP_line.p_buffer = g_FSP_line_buffer;
	FSP_line.buffer_size = FSP_BUF_LEN;
	FSP_line.write_index = 0;

	if (FSP_line.buffer_size != 0)
		memset((void*) FSP_line.p_buffer, 0, sizeof(FSP_line.p_buffer));

	fsp_init(FSP_ADR_HMI);
}

/* :::::::::: FSP Line Task ::::::::::::: */
void FSP_Line_Task(void*) {
	uint8_t data, time_out, error = 0;

	for (time_out = 50;
			(!rbuffer_empty(&p_UART2_meta->rb_rx)) && (time_out != 0);
			time_out--) {
		data = rbuffer_remove(&p_UART2_meta->rb_rx);

		if (data == FSP_PKT_SOD) {
			FSP_line.write_index = 0;
		} else if (data == FSP_PKT_EOF) {
			error = frame_decode((uint8_t*) FSP_line.p_buffer,
					FSP_line.write_index, &s_FSP_RX_Packet);

//			frame_decode((uint8_t*) FSP_line.p_buffer, FSP_line.write_index,
//					&s_FSP_RX_Packet);
//			UART_Printf(&RS232_UART, "%s> ", ErrorCode[FSP_return]);

			if (error == FSP_PKT_READY)
				FSP_Line_Process();

			FSP_line.write_index = 0;
		} else {
			FSP_line.p_buffer[FSP_line.write_index] = data;
			FSP_line.write_index++;

			if (FSP_line.write_index > FSP_line.buffer_size)
				FSP_line.write_index = 0;

		}
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
