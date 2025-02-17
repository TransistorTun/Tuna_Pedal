/*
 * SST26VF064.c
 *
 *  Created on: Nov 18, 2024
 *      Author: thanh
 */
#include "SST26VF064.h"
uint8_t data_256[256]; /* global array to store read data */
uint8_t block_protection_18[18]; /* global array to store block_protection data */
uint8_t Manufacturer_Id, Device_Type, Device_Id; /* global array to store Manufacturer and Device id information */

uint8_t Sw;
uint8_t buffer[15];
uint32_t deviceaddress;

/************************************************************************/
/* PROCEDURE: Send_Byte							*/
/*									*/
/* This procedure outputs a byte shifting out 1-bit per clock rising	*/
/* edge on the the SI pin (SIO0 pin) LSB 1st.				*/
/************************************************************************/
void Send_Byte(uint8_t data) {

	while (!__HAL_SPI_GET_FLAG(SST26_SPI, SPI_FLAG_TXE))
		;
	HAL_SPI_Transmit(SST26_SPI, &data, 1, 100);
}
/************************************************************************/
/* PROCEDURE: Get_Byte							*/
/*									*/
/* This procedure inputs a byte shifting in 1-bit per clock falling	*/
/* edge on the SIO1 pin(LSB 1st).					*/
/************************************************************************/
uint8_t Get_Byte() {
	uint8_t dummy, data;
	dummy = 0xFF;
	while (!__HAL_SPI_GET_FLAG(SST26_SPI, SPI_FLAG_TXE))
		;
	HAL_SPI_TransmitReceive(SST26_SPI, &dummy, &data, 1, 100);
	return data;
}
/************************************************************************/
/* PROCEDURE: Read_Configuration_Register				*/
/*									*/
/* This procedure reads the configuration register and returns the byte.*/
/************************************************************************/
uint8_t SPI_Read_Configuration_Register() {
	uint8_t byte = 0;
	CLR_CE; /* enable device */
	Send_Byte(0x35); /* send RDSR command */
	byte = Get_Byte(); /* receive byte */
	SET_CE; /* disable device */
	return byte;
}

/************************************************************************/
/* PROCEDURE: WREN							*/
/*									*/
/* This procedure enables the Write Enable Latch.               	*/
/************************************************************************/

void SPI_WREN() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_WREN); /* send WREN command */
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: WRDI							*/
/*									*/
/* This procedure disables the Write Enable Latch.			*/
/************************************************************************/

void SPI_WRDI() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_WRDI); /* send WRDI command */
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: QuadJ_ID							*/
/*									*/
/* This procedure Reads the manufacturer's ID, device Type and device ID.  It will 	*/
/* use AFh as the command to read the ID.                               */
/* Returns:								*/
/*	ID1(Manufacture's ID = BFh, Device Type =26h , Device ID = 02h)	*/
/*									*/
/************************************************************************/
void Jedec_ID_Read() {

	CLR_CE; /* enable device */
	Send_Byte(SST26_JEDEC_ID); /* send JEDEC ID command (9Fh) */
	Manufacturer_Id = Get_Byte(); /* receive byte */
	Device_Type = Get_Byte(); /* receive byte */
	Device_Id = Get_Byte(); /* receive byte */

	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Read							*/
/*									*/
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.						*/
/* Input:								*/
/*		Dst:	Destination Address 000000H - 7FFFFFH		*/
/************************************************************************/
uint8_t Read(uint32_t Dst) {
	uint8_t byte = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_READ); /* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	byte = Get_Byte();
	SET_CE; /* disable device */
	return byte; /* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	Read_Cont						*/
/*									*/
/* This procedure reads multiple addresses of the device and stores	*/
/* data into 256 byte buffer. Maximum number of bytes read is limited 256 bytes*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 7FFFFFH	*/
/*      	no_bytes	Number of bytes to read	(max = 256)	*/
/************************************************************************/
void Read_Cont(uint32_t Dst, uint32_t no_bytes) {
	uint32_t i = 0;
	CLR_CE; /* enable device */
	Send_Byte(SST26_READ); /* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	if (no_bytes > 256) {
		no_bytes = 256;
	}
	for (i = 0; i < no_bytes; i++) /* read until no_bytes is reached */
	{
		data_256[i] = Get_Byte();
	}
	SET_CE; /* disable device */

}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read						*/
/*									*/
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.						*/
/* Input:								*/
/*		Dst:	Destination Address 000000H - 7FFFFFH		*/
/************************************************************************/
uint8_t SPI_HighSpeed_Read(uint32_t Dst) {
	uint8_t byte = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_HIGH_SPEED_READ); /* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF); /*dummy byte*/
	byte = Get_Byte();
	SET_CE; /* disable device */
	return byte; /* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read_Cont					*/
/*									*/
/* This procedure reads multiple addresses of the device and stores	*/
/* data into 256 byte buffer. Maximum number of bytes read is limited to 256 bytes*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 7FFFFFH	*/
/*      	no_bytes	Number of bytes to read	(max = 256)	*/
/************************************************************************/
void SPI_HighSpeed_Read_Cont(uint32_t Dst, uint32_t no_bytes) {
	uint32_t i = 0;
	CLR_CE; /* enable device */
	Send_Byte(SST26_HIGH_SPEED_READ); /* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF); /*dummy byte*/
	if (no_bytes > 256) {
		no_bytes = 256;
	}

	for (i = 0; i < no_bytes; i++) /* read until no_bytes is reached */
	{
		data_256[i] = Get_Byte();
	}
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Set_Burst						*/
/*									*/
/* This procedure sets the burst length to either 8bytes or 16bytes or 32bytes or 64bytes.			*/
/* Input:								*/
/*		byte:	00h,01h,02h or 03h for setting burst length	*/
/************************************************************************/
void SPI_Set_Burst(uint8_t byte) {
	CLR_CE; /* enable device */
	Send_Byte(SST26_SB); /* send Byte Program command */
	Send_Byte(byte); /* send byte to be programmed */
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Page_Program						*/
/*									*/
/* This procedure does page programming.  The destination               */
/* address should be provided.                                  	*/
/* The data array of 256 bytes contains the data to be programmed.      */
/* Since the size of the data array is 256 bytes rather than 256 bytes, this page program*/
/* procedure programs only 256 bytes                                    */
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.					*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 7FFFFFH	*/
/*		data_256[256] containing 256 bytes of data will be programmed using this function */
/************************************************************************/
void SPI_Page_Program(uint32_t Dst) {
	unsigned int i;
	i = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_PP); /* send Byte Program command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	for (i = 0; i < 256; i++) {
		Send_Byte(data_256[i]); /* send byte to be programmed */
	}
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Chip_Erase						*/
/*									*/
/* This procedure erases the entire Chip.				*/
/************************************************************************/
void SPI_Chip_Erase() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_CE); /* send Chip Erase command (C7h) */
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Sector_Erase						*/
/*									*/
/* This procedure Sector Erases the Chip.				*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 7FFFFFH	*/
/************************************************************************/
void SPI_Sector_Erase(uint32_t Dst) {

	CLR_CE; /* enable device */
	Send_Byte(SST26_SE); /* send Sector Erase command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Block_Erase						*/
/*									*/
/* This procedure Block Erases 8Kbyte, 32 KByte or 64 KByte of the Chip.*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 7FFFFFH	*/
/************************************************************************/
void SPI_Block_Erase(uint32_t Dst) {
	CLR_CE; /* enable device */
	Send_Byte(SST26_BE); /* send Block Erase command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: NoOp                                              	*/
/*									*/
/* No operation is performed.                                           */
/************************************************************************/
void SPI_NoOp() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_NOP);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: ResetEn                                                   */
/*									*/
/* This procedure Enables acceptance of the RST (Reset) operation.	*/
/************************************************************************/
void SPI_ResetEn() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_RSTEN);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Reset                                     		*/
/*									*/
/* This procedure resets the device in to normal operating Ready mode.	*/
/*									*/
/************************************************************************/
void SPI_Reset() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_RST);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: En_QIO                                    		*/
/*									*/
/* This procedure enables quad I/O operation.           		*/
/************************************************************************/
void En_QIO() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_EQIO);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Reset_QIO                                 		*/
/*									*/
/* This procedure resets the device to 1-bit SPI protocol operation.    */
/************************************************************************/
void SPI_Reset_QIO() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_RSTQIO);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Write_Suspend						*/
/*									*/
/* This procedure suspends Program/Erase operation.			*/
/************************************************************************/
void SPI_Write_Suspend() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_WRSU);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Write_Resume						*/
/*									*/
/* This procedure resumes Program/Erase operation.			*/
/************************************************************************/
void SPI_Write_Resume() {
	CLR_CE; /* enable device */
	Send_Byte(SST26_WRRE);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE: Write_Status_Register					*/
/*									*/
/* This procedure resumes Program/Erase operation.			*/
/************************************************************************/
void SPI_Write_Status_Register(unsigned int data1, uint8_t datalen) { //For data1 - top 8 bits are status reg bits , lower 8 bits are configuration reg bits
	CLR_CE; /* enable device */
	Send_Byte(SST26_WRSR);
	Send_Byte((data1 >> 8) & 0xff);
	if (datalen == 2) {
		Send_Byte((data1) & 0xff);
	}

	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	ReadSID	(Read Security ID)				*/
/*									*/
/* This procedure reads the security ID					*/
/************************************************************************/
void SPI_ReadSID(uint8_t *security_ID, uint32_t Dst, uint32_t security_length) {

	uint32_t i;
	i = 0;
	if (security_length > 2048) {
		security_length = 2048;
	}

	CLR_CE; /* enable device */
	Send_Byte(SST26_RSID);
	Send_Byte((Dst >> 8) & 0xFF);
	Send_Byte(Dst & 0xFF);
	Send_Byte(Dst & 0xFF);  //dummy

	for (i = 0; i < security_length; i++) {
		*security_ID = Get_Byte();
		security_ID++;
	}
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	ProgSID	(Program Security ID)                           */
/*									*/
/* This procedure programs the security ID				*/
/*									*/
/************************************************************************/
void SPI_ProgSID(uint8_t *security_ID, uint32_t Dst, uint32_t security_length) {
	uint32_t i;

	i = 0;

	if (security_length > 256) {
		security_length = 256;
	}

	CLR_CE; /* enable device */
	Send_Byte(SST26_PSID);
	Send_Byte((Dst >> 8) & 0xFF);
	Send_Byte(Dst & 0xFF);

	for (i = 0; i < security_length; i++) {
		Send_Byte(*security_ID);
		security_ID++;
	}

	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	LockSID							*/
/*									*/
/* This procedure Locks the security ID setting				*/
/*									*/
/************************************************************************/
void SPI_LockSID() {

	CLR_CE; /* enable device */
	Send_Byte(SST26_LSID);
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	ReadBlockProtection			  		*/
/*									*/
/* This procedure reads block protection register			*/
/*									*/
/************************************************************************/

void SPI_ReadBlockProtection() {

	uint8_t i;
	i = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_RBPR);

	for (i = 18; i > 0; i--) {
		block_protection_18[i - 1] = Get_Byte();
	}
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	WriteBlockProtection					*/
/*									*/
/* This procedure writes to block protection register			*/
/*									*/
/************************************************************************/
void SPI_WriteBlockProtection() {

	uint8_t i;
	i = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_WBPR); /* read command */

	for (i = 18; i > 0; i--) {
		Send_Byte(block_protection_18[i - 1]);
	}
	SET_CE; /* disable device */
}
/************************************************************************/
/* PROCEDURE:	Global Block Protection Unlock				*/
/*									*/
/* This procedure clears all block protection				*/
/************************************************************************/
void SPI_Global_Block_Protection_Unlock() {

	CLR_CE; /* enable device */
	Send_Byte(SST26_ULBPR); /* read command */
	SET_CE; /* disable device */
}
/************************************************************************/
/* PROCEDURE:	LockBlockProtection					*/
/*									*/
/* This procedure locks the block protection register			*/
/************************************************************************/
void SPI_LockBlockProtection() {

	CLR_CE; /* enable device */
	Send_Byte(SST26_LBPR); /* read command */
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Non Volatile Write Lock Protection			*/
/*									*/
/* This procedure writes to block protection register			*/
/*									*/
/************************************************************************/
void SPI_NonVolWriteLockProtection() {

	uint8_t i;
	i = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_nVWLDR); /* read command */

	for (i = 18; i > 0; i--) {
		Send_Byte(block_protection_18[i - 1]);
	}
	SET_CE; /* disable device */
}

/************************************************************************/
/* PROCEDURE:	SPI_SFDP_Read						*/
/*									*/
/* This procedure reads SFDP Table.					*/
/*									*/
/************************************************************************/
uint8_t SPI_SFDP_Read(uint32_t Dst) {

	uint8_t byte = 0;

	CLR_CE; /* enable device */
	Send_Byte(SST26_SFDP); /* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF); /*dummy byte*/
	byte = Get_Byte();
	SET_CE; /* disable device */
	return byte; /* return one byte read */
}
