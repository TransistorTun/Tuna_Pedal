/*
 * UART2.c
 *
 *  Created on: Nov 16, 2024
 *      Author: thanh
 */

#include "UART.h"
#include <stm32f407xx.h>
#include <stm32f4xx_ll_usart.h>
//#include "stm32f4xx_it.h"

static const char *const g_pcHex = "0123456789abcdef";

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
usart_meta_t UART2_meta;
usart_meta_t *p_UART2_meta = &UART2_meta;
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS

void USART2_IRQ(void) {
	uint8_t data;
	if (LL_USART_IsActiveFlag_TXE(USART2)) {
		if (!rbuffer_empty(&p_UART2_meta->rb_tx)) {
			data = rbuffer_remove(&p_UART2_meta->rb_tx);
			LL_USART_TransmitData9(USART2, (uint16_t) data);
		} else
			LL_USART_DisableIT_TXE(USART2);
	}
	if (LL_USART_IsActiveFlag_RXNE(USART2)) {
		data = LL_USART_ReceiveData8(USART2);
		if (!rbuffer_full(&p_UART2_meta->rb_rx)) {
			rbuffer_insert(data, &p_UART2_meta->rb_rx);
//			p_UART2_meta->usart_error = UCSR1A & USART_RX_ERROR_MASK;
		}
//		else {
//			p_UART1_meta->usart_error = ((UCSR1A & USART_RX_ERROR_MASK)
//					| USART_BUFFER_OVERFLOW);
//		}
		LL_USART_ClearFlag_RXNE(USART2);
	}
}

void usart2_init(void) {
	rbuffer_init(&p_UART2_meta->rb_tx);                        // Init Rx buffer
	rbuffer_init(&p_UART2_meta->rb_rx);                        // Init Tx buffer
	LL_USART_EnableIT_RXNE(USART2);
}

void usart2_send_char(char c) {
	while (rbuffer_full(&p_UART2_meta->rb_tx))
		;
	rbuffer_insert(c, &p_UART2_meta->rb_tx);
	LL_USART_EnableIT_TXE(USART2);
}

void usart2_send_string(const char *str) {
	while (*str) {
		usart2_send_char(*str++);
	}
}

void usart2_send_array(const char *str, uint8_t len) {
	uint8_t udx;
	for (udx = 0; udx < len; udx++)
		usart2_send_char(*str++);
}

//void usart2_send_string_P(const char *chr) {
//	char c;
//	while ((c = pgm_read_byte(chr++))) {
//		usart2_send_char(c);
//	}
//}

uint8_t usart2_rx_count(void) {
	return rbuffer_count(&p_UART2_meta->rb_rx);
}

uint16_t usart2_read_char(void) {
	return 0;										//fix sau!
	if (!rbuffer_empty(&p_UART2_meta->rb_rx)) {
//		return (((p_UART2_meta->usart_error & USART_RX_ERROR_MASK) << 8)
//				| (uint16_t) rbuffer_remove(&p_UART2_meta->rb_rx));
	} else {
//		return (((p_UART2_meta->usart_error & USART_RX_ERROR_MASK) << 8)
//				| USART_NO_DATA);     // Empty ringbuffer
	}
}

void usart2_close() {
	while (!rbuffer_empty(&p_UART2_meta->rb_tx))
		;                // Wait for Tx to transmit ALL characters in ringbuffer
//	while (!(UCSR0A & (1 << TXC)))
//		;		// Wait for Tx unit to transmit the LAST character of ringbuffer

//	_delay_ms(200);                            // Extra safety for Tx to finish!

//	UCSR0B &= ~((1 << RXEN) | (1 << TXEN) | (1 << RXCIE) | (1 << UDRIE)); //disable TX, RX, RX interrupt
//	UCSR0C &= (1 << UCSZ1) | (1 << UCSZ0);
}

volatile ringbuffer_t* uart_get_usart2_rx_buffer_address(void) {
	return p_UART2_meta;
}
