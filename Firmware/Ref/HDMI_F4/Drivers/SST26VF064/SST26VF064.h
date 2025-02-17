/*
 * SST26VF064.h
 *
 *  Created on: Nov 18, 2024
 *      Author: thanh
 */


#ifndef SST26VF064_SST26VF064_H_
#define SST26VF064_SST26VF064_H_

#include "main.h"

/******************Configuration******************/
#define SST26_NOP				0x00
#define SST26_RSTEN				0x66
#define SST26_RST				0x99
#define SST26_EQIO				0x38
#define SST26_RSTQIO			0xFF
#define SST26_RDSR				0x05
#define SST26_WRSR				0x01
#define SST26_RDCR				0x35

/******************Read Operation******************/
#define SST26_READ				0x03
#define SST26_HIGH_SPEED_READ 	0x0B
#define SST26_SQOR				0x6B
#define SST26_SQIOR				0xEB
#define SST26_SDOR				0x3B
#define SST26_SDIOR				0xBB
#define SST26_SB				0xC0
#define SST26_RBSQI				0x0C
#define SST26_RBSPI				0xEC

/*************Identification Operation*************/
#define SST26_JEDEC_ID			0x9F
#define SST26_Quad_J_ID	  	 	0xAF
#define SST26_SFDP				0x5A

/******************Write Operation******************/
#define SST26_WREN				0x06
#define SST26_WRDI				0x04
#define SST26_SE				0x20
#define SST26_BE				0xD8
#define SST26_CE				0xC7
#define SST26_PP				0x02
#define SST26_SPI_Quad_PP		0x32
#define SST26_WRSU				0xB0
#define SST26_WRRE				0x30

/****************Protection Operation****************/
#define SST26_RBPR				0x72
#define SST26_WBPR				0x42
#define SST26_LBPR				0x8D
#define SST26_nVWLDR			0xE8
#define SST26_ULBPR				0x98
#define SST26_RSID				0x88
#define SST26_PSID				0xA5
#define SST26_LSID				0x85

#define SST26_SPI	&hspi1
#define SET_CE	ROM_CS_GPIO_Port->BSRR = ROM_CS_Pin
#define CLR_CE 	ROM_CS_GPIO_Port->BSRR = (uint32_t)ROM_CS_Pin << 16

void Send_Byte(uint8_t data);
uint8_t Get_Byte();
uint8_t SPI_Read_Configuration_Register();
void SPI_WREN();
void SPI_WRDI();
void Jedec_ID_Read();
uint8_t Read(uint32_t Dst);
void Read_Cont(uint32_t Dst, uint32_t no_bytes);
uint8_t SPI_HighSpeed_Read(uint32_t Dst);
void SPI_HighSpeed_Read_Cont(uint32_t Dst, uint32_t no_bytes);
void SPI_Set_Burst(uint8_t byte);
void SPI_Page_Program(uint32_t Dst);
void SPI_Chip_Erase();
void SPI_Sector_Erase(uint32_t Dst);
void SPI_Block_Erase(uint32_t Dst);
void SPI_NoOp();
void SPI_ResetEn();
void SPI_Reset();
void En_QIO();
void SPI_Reset_QIO();
void SPI_Write_Suspend();
void SPI_Write_Resume();
void SPI_Write_Status_Register(unsigned int data1, uint8_t datalen);
void SPI_ReadSID(uint8_t *security_ID, uint32_t Dst, uint32_t security_length);
void SPI_ProgSID(uint8_t *security_ID, uint32_t Dst, uint32_t security_length);
void SPI_LockSID();
void SPI_ReadBlockProtection();
void SPI_WriteBlockProtection();
void SPI_Global_Block_Protection_Unlock();
void SPI_LockBlockProtection();
void SPI_NonVolWriteLockProtection();
uint8_t SPI_SFDP_Read(uint32_t Dst);


#endif /* SST26VF064_SST26VF064_H_ */
