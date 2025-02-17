/*
 * crc16.c
 *
 *  Created on: May 25, 2024
 *      Author: CAO HIEU
 */



#include "crc.h"

uint16_t crc16_CCITT(uint16_t initial_value, uint8_t* data, uint8_t size)
{
    uint8_t x;
    uint16_t crc = initial_value;

    while(size--)
    {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }

    return crc;
}
