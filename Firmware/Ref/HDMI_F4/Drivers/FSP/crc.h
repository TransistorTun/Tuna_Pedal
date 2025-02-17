/*
 * crc.h
 *
 *  Created on: May 25, 2024
 *      Author: CAO HIEU
 */

#ifndef LIBFSP_CRC_H_
#define LIBFSP_CRC_H_

#include <stdint.h>

/**
 * \brief Computes the CRC16-CCITT value of an array of data.
 *
 * \param initial_value is the initial value to compute the crc16 value.
 * \param data is the data to compute the crc16 value.
 * \param size is the length of the data array.
 *
 * \return Returns the crc16 value of the data.
 */
uint16_t crc16_CCITT(uint16_t initial_value, uint8_t* data, uint8_t size);


#endif /* LIBFSP_CRC_H_ */
