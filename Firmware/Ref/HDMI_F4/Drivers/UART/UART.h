/*
 * UART2.h
 *
 *  Created on: Nov 16, 2024
 *      Author: thanh
 */

#ifndef UART_H_
#define UART_H_

#include "RingBuffer.h"

// DEFINE RING BUFFER SIZE; MUST BE 2, 4, 8, 16, 32, 64 or 128
#define RBUFFER_SIZE 64

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART META STRUCT
typedef struct {
	volatile ringbuffer_t rb_rx;    // Rx ringbuffer
	volatile ringbuffer_t rb_tx;    // Tx ringbuffer
	volatile uint16_t usart_error;   // Holds error from RXDATAH
} usart_meta_t;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS
void usart2_init(void);			//init at 9600 bps, FCPU = 8Mhz
void usart2_send_char(char c);
void usart2_send_string(const char *str);
void usart2_send_array(const char *str, uint8_t len);
//void usart2_send_string_P(const char *chr);
uint8_t usart2_rx_count(void);
uint16_t usart2_read_char(void);
void usart2_close(void);
volatile ringbuffer_t* uart_get_usart2_rx_buffer_address(void);
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//UART ISR
void USART2_IRQ();
#endif /* UART_H_ */
