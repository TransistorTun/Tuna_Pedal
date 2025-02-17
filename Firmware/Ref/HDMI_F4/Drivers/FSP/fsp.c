/*
 * fsp.c
 *
 *  Created on: May 25, 2024
 *      Author: CAO HIEU
 */

#include "fsp.h"
#include "crc.h"

#include <string.h>
#include <stdio.h>

uint8_t fsp_my_adr;

void fsp_init(uint8_t module_adr)
{
    fsp_my_adr = module_adr;

}

void fsp_gen_data_pkt(uint8_t *data, uint8_t data_len, uint8_t dst_adr, uint8_t ack, fsp_packet_t *fsp)
{
    if (ack == FSP_PKT_WITH_ACK)
    {
        fsp_gen_pkt((void*)0, data, data_len, dst_adr, FSP_PKT_TYPE_DATA_WITH_ACK, fsp);
    }
    else
    {
        fsp_gen_pkt((void*)0, data, data_len, dst_adr, FSP_PKT_TYPE_DATA, fsp);
    }
}

void fsp_gen_cmd_pkt(uint8_t cmd, uint8_t dst_adr, uint8_t ack, fsp_packet_t *fsp)
{
    if (ack == FSP_PKT_WITH_ACK)
    {
        fsp_gen_pkt(&cmd,(void*)0, 0, dst_adr, FSP_PKT_TYPE_CMD_WITH_ACK, fsp);
    }
    else
    {
        fsp_gen_pkt(&cmd,(void*)0,  0, dst_adr, FSP_PKT_TYPE_CMD, fsp);
    }
}

void fsp_gen_cmd_w_data_pkt(uint8_t cmd, uint8_t *data, uint8_t data_len, uint8_t dst_adr, uint8_t ack, fsp_packet_t *fsp)
{
    if (ack == FSP_PKT_WITH_ACK)
    {
        fsp_gen_pkt(&cmd, data, data_len, dst_adr, FSP_PKT_TYPE_CMD_W_DATA_ACK, fsp);
    }
    else
    {
        fsp_gen_pkt(&cmd, data, data_len, dst_adr, FSP_PKT_TYPE_CMD_W_DATA, fsp);
    }
}


void fsp_gen_ack_pkt(uint8_t dst_adr, fsp_packet_t *fsp)
{
    fsp_gen_pkt((void*)0, (void*)0, 0, dst_adr, FSP_PKT_TYPE_ACK, fsp);
}

void fsp_gen_nack_pkt(uint8_t dst_adr, fsp_packet_t *fsp)
{
    fsp_gen_pkt((void*)0, (void*)0, 0, dst_adr, FSP_PKT_TYPE_NACK, fsp);
}


void fsp_gen_pkt(uint8_t *cmd, uint8_t *payload, uint8_t payload_len, uint8_t dst_adr, uint8_t type, fsp_packet_t *fsp)
{
    fsp->sod        = FSP_PKT_SOD;
    fsp->src_adr    = fsp_my_adr;
    fsp->dst_adr    = dst_adr;
    fsp->length     = payload_len;
    fsp->type       = type;

    uint8_t i = 0;
    uint8_t j = 0;

    // Copy cmd payload
    if (cmd != NULL) {
        fsp->length++; // length + byte cmd
        fsp->payload[j++] = *cmd;
    }

    // Copy payload fsp->payload
    for(i=0; i<payload_len; i++)
    {
        fsp->payload[j++] = payload[i];
    }

    fsp->crc16 = crc16_CCITT(FSP_CRC16_INITIAL_VALUE, &fsp->src_adr, fsp->length + 4);

}


void frame_encode(fsp_packet_t *fsp, uint8_t *frame, uint8_t *frame_len)
{
    //frame
    uint8_t encoded_frame[FSP_PKT_MAX_LENGTH];
    uint8_t encoded_length = 0;

    encoded_frame[encoded_length++] = fsp->sod;
    encoded_frame[encoded_length++] = fsp->src_adr;
    encoded_frame[encoded_length++] = fsp->dst_adr;
    encoded_frame[encoded_length++] = fsp->length;
    encoded_frame[encoded_length++] = fsp->type;

    for(int i=0; i<fsp->length; i++)
    {
        if (fsp->payload[i] == FSP_PKT_SOD) {
            encoded_frame[encoded_length++] = FSP_PKT_ESC;
            encoded_frame[encoded_length++] = FSP_PKT_TSOD;
        } else if (fsp->payload[i] == FSP_PKT_EOF) {
            encoded_frame[encoded_length++] = FSP_PKT_ESC;
            encoded_frame[encoded_length++] = FSP_PKT_TEOF;
        } else if (fsp->payload[i] == FSP_PKT_ESC) {
            encoded_frame[encoded_length++] = FSP_PKT_ESC;
            encoded_frame[encoded_length++] = FSP_PKT_TESC;
        } else {
            encoded_frame[encoded_length++] = fsp->payload[i];
        }
    }

    // CRC16
    uint8_t crc_msb = (uint8_t)(fsp->crc16 >> 8);
    uint8_t crc_lsb = (uint8_t)(fsp->crc16 & 0xFF);

    if (crc_msb == FSP_PKT_SOD) {
        encoded_frame[encoded_length++] = FSP_PKT_ESC;
        encoded_frame[encoded_length++] = FSP_PKT_TSOD;
    } else if (crc_msb == FSP_PKT_EOF) {
        encoded_frame[encoded_length++] = FSP_PKT_ESC;
        encoded_frame[encoded_length++] = FSP_PKT_TEOF;
    } else if (crc_msb == FSP_PKT_ESC) {
        encoded_frame[encoded_length++] = FSP_PKT_ESC;
        encoded_frame[encoded_length++] = FSP_PKT_TESC;
    } else {
        encoded_frame[encoded_length++] = crc_msb;
    }

    if (crc_lsb == FSP_PKT_SOD) {
        encoded_frame[encoded_length++] = FSP_PKT_ESC;
        encoded_frame[encoded_length++] = FSP_PKT_TSOD;
    } else if (crc_lsb == FSP_PKT_EOF) {
        encoded_frame[encoded_length++] = FSP_PKT_ESC;
        encoded_frame[encoded_length++] = FSP_PKT_TEOF;
    } else if (crc_lsb == FSP_PKT_ESC) {
        encoded_frame[encoded_length++] = FSP_PKT_ESC;
        encoded_frame[encoded_length++] = FSP_PKT_TESC;
    } else {
        encoded_frame[encoded_length++] = crc_lsb;
    }

    encoded_frame[encoded_length++] = FSP_PKT_EOF;

    memcpy(frame, encoded_frame, encoded_length);
    *frame_len = encoded_length;
}


void fsp_encode(fsp_packet_t *fsp, uint8_t *pkt, uint8_t *pkt_len)
{
    uint8_t i = 0;
    uint8_t crc_msb = (uint8_t)(fsp->crc16 >> 8);
    uint8_t crc_lsb = (uint8_t)(fsp->crc16 & 0xFF);
    pkt[i++] = fsp->sod;
    pkt[i++] = fsp->src_adr;
    pkt[i++] = fsp->dst_adr;
    pkt[i++] = fsp->length;
    pkt[i++] = fsp->type;

    uint8_t j = 0;
    for(j=0; j<fsp->length; j++)
    {
		if (fsp->payload[j] == FSP_PKT_SOD) {
			pkt[i++] = FSP_PKT_ESC;
			pkt[i++] = FSP_PKT_TSOD;
		} else if (fsp->payload[j] == FSP_PKT_EOF) {
			pkt[i++] = FSP_PKT_ESC;
			pkt[i++] = FSP_PKT_TEOF;
		} else if (fsp->payload[j] == FSP_PKT_ESC) {
			pkt[i++] = FSP_PKT_ESC;
			pkt[i++] = FSP_PKT_TESC;
		} else
			pkt[i++] = fsp->payload[j];
    }

    if (crc_msb == FSP_PKT_SOD) {
		pkt[i++] = FSP_PKT_ESC;
		pkt[i++] = FSP_PKT_TSOD;
	} else if (crc_msb == FSP_PKT_EOF) {
		pkt[i++] = FSP_PKT_ESC;
		pkt[i++] = FSP_PKT_TEOF;
	} else if (crc_msb == FSP_PKT_ESC) {
		pkt[i++] = FSP_PKT_ESC;
		pkt[i++] = FSP_PKT_TESC;
	} else
	    pkt[i++] = crc_msb;

    if (crc_lsb == FSP_PKT_SOD) {
	    pkt[i++] = FSP_PKT_ESC;
	    pkt[i++] = FSP_PKT_TSOD;
	} else if (crc_lsb == FSP_PKT_EOF) {
	    pkt[i++] = FSP_PKT_ESC;
	    pkt[i++] = FSP_PKT_TEOF;
	} else if (crc_lsb == FSP_PKT_ESC) {
	    pkt[i++] = FSP_PKT_ESC;
	    pkt[i++] = FSP_PKT_TESC;
    } else
		pkt[i++] = crc_lsb;

	pkt[i++] = FSP_PKT_EOF;
	*pkt_len = i;
}

uint8_t fsp_decode(uint8_t byte, fsp_packet_t *fsp)
{
    /*
    switch(fsp_decode_pos)
    {
        case FSP_PKT_POS_SOD:
            if (byte == FSP_PKT_SOD)
            {
                fsp->sod = byte;
                fsp_decode_pos++;
                return FSP_PKT_NOT_READY;
            }
            else
            {
                fsp_decode_pos = FSP_PKT_POS_SOD;
                return FSP_PKT_INVALID;
            }
        case FSP_PKT_POS_SRC_ADR:
            fsp->src_adr = byte;
            fsp_decode_pos++;
            return FSP_PKT_NOT_READY;
        case FSP_PKT_POS_DST_ADR:
            fsp->dst_adr = byte;

            if (byte == fsp_my_adr)
            {
                fsp_decode_pos++;
                return FSP_PKT_NOT_READY;
            }
            else
            {
                fsp_decode_pos = FSP_PKT_POS_SOD;
                return FSP_PKT_WRONG_ADR;
            }
        case FSP_PKT_POS_LEN:
            if (byte > FSP_PAYLOAD_MAX_LENGTH)
            {
                fsp_decode_pos = FSP_PKT_POS_SOD;
                return FSP_PKT_INVALID;
            }
            else
            {
                fsp->length = byte;
                fsp_decode_pos++;
                return FSP_PKT_NOT_READY;
            }
        case FSP_PKT_POS_TYPE:
            fsp->type = byte;
            fsp_decode_pos++;
            return FSP_PKT_NOT_READY;
        default:
            if (fsp_decode_pos < (FSP_PKT_POS_TYPE + fsp->length + 1))          // Payload
            {
                fsp->payload[fsp_decode_pos - FSP_PKT_POS_TYPE - 1] = byte;
                fsp_decode_pos++;
                return FSP_PKT_NOT_READY;
            }
            else if (fsp_decode_pos == (FSP_PKT_POS_TYPE + fsp->length + 1))    // CRC16 MSB
            {
                fsp->crc16 = (uint16_t)(byte << 8);
                fsp_decode_pos++;
                return FSP_PKT_NOT_READY;
            }
            else if (fsp_decode_pos == (FSP_PKT_POS_TYPE + fsp->length + 2))    // CRC16 LSB
            {
                fsp->crc16 |= (uint16_t)(byte);

                if (fsp->crc16 == crc16_CCITT(FSP_CRC16_INITIAL_VALUE, &fsp->src_adr, fsp->length + 4))
                {
                    fsp_decode_pos = FSP_PKT_POS_SOD;
                    return FSP_PKT_READY;
                }
                else
                {
                    fsp_decode_pos = FSP_PKT_POS_SOD;
                    return FSP_PKT_INVALID;
                }
            }
            else
            {
                fsp_decode_pos = FSP_PKT_POS_SOD;
                return FSP_PKT_ERROR;
            }
    }
    */
	return 0;
}

int frame_decode(uint8_t *buffer, uint8_t length, fsp_packet_t *pkt){

//    fsp_packet_t fsp_pkt;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t escape = 0;
    uint8_t decoded_payload[FSP_PAYLOAD_MAX_LENGTH];


	if (length < FSP_PKT_MIN_LENGTH - 2) {
	        return FSP_PKT_INVALID;
	}


	while (i < length){
        uint8_t byte = buffer[i++];
        if (escape) {
            if (byte == FSP_PKT_TSOD) {
                decoded_payload[j++] = FSP_PKT_SOD;
            } else if (byte == FSP_PKT_TEOF) {
                decoded_payload[j++] = FSP_PKT_EOF;
            } else if (byte == FSP_PKT_TESC) {
                decoded_payload[j++] = FSP_PKT_ESC;
            } else {
            	return FSP_PKT_INVALID;
            }
            escape = 0;
        } else if (byte == FSP_PKT_ESC) {
        	escape = 1;
        } else {
            decoded_payload[j++] = byte;
        }
	}

    i = 0;
    pkt->src_adr = decoded_payload[i++];
    pkt->dst_adr = decoded_payload[i++];
    pkt->length = decoded_payload[i++];
    pkt->type = decoded_payload[i++];

    if (pkt->length > FSP_PAYLOAD_MAX_LENGTH || pkt->length != j - FSP_PKT_HEADER_LENGTH  - FSP_PKT_CRC_LENGTH) {
        return FSP_PKT_WRONG_LENGTH;
    }

    memcpy(pkt->payload, &decoded_payload[i], pkt->length);
    i += pkt->length;
    //CRC
    uint16_t crc_received = (uint16_t)(decoded_payload[i++] << 8);
    crc_received |= (uint16_t)(decoded_payload[i++]);


    // CAL CRC
    uint16_t crc_calculated = crc16_CCITT(FSP_CRC16_INITIAL_VALUE, &pkt->src_adr, pkt->length + 4);


    // CHECK CRC
    if (crc_received != crc_calculated) {
        return FSP_PKT_CRC_FAIL;
    }

    // Address
    if (pkt->dst_adr != fsp_my_adr) {
        return FSP_PKT_WRONG_ADR;
    }

//    *pkt = fsp_pkt;

//    frame_processing(&fsp_pkt);
    return FSP_PKT_READY;
}
