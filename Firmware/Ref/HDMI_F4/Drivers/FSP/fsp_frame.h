#ifndef _FSP_FRAME_H

#define _FSP_FRAME_H

#include <stdint.h>

// Commands
typedef enum _e_FSP_CMD_typedef_ {
	FSP_CMD_GET_ALL,
	FSP_CMD_SET_ALL,
	FSP_CMD_CONFIRM,
	FSP_CMD_SEQUENCE,
	FSP_CMD_KILL,
	FSP_CMD_MEASURE_BMP,
	FSP_CMD_MEASURE_LSM,

} e_FSP_CMD_typedef;

typedef struct _FSP_CMD_GET_ALL_FRAME_ {
	uint8_t hv_volt_high; 	/* hs cap vol */
	uint8_t hv_volt_low; 	/* hs cap vol */
	uint8_t lv_volt; 		/* ls cap vol */
	uint8_t current_high; 	/* current */
	uint8_t current_low; 	/* current */
	uint8_t impedance_high; /* impedance */
	uint8_t impedance_low; 	/* impedance */
	uint8_t state; 		/* state cap */ //Pulse | Charge | Release | Done | Run
} FSP_CMD_GET_ALL_FRAME;

typedef struct _FSP_CMD_SET_ALL_FRAME_ {
	uint8_t sequence_index;
	uint8_t hs_volt_high; 	/* hs cap vol */
	uint8_t hs_volt_low; 	/* hs cap vol */
	uint8_t ls_volt; 		/* ls cap vol */
	uint8_t pole; 			/* pulse pole */
	uint8_t hv_pos_count; 	/* hv pulse count */
	uint8_t hv_neg_count; 	/* hv pulse count */
	uint8_t lv_pos_count; 	/* lv pulse count */
	uint8_t lv_neg_count; 	/* lv pulse count */
	uint8_t hv_delay;		/* delay between P n N*/
	uint8_t lv_delay;		/* delay between P n N*/
	uint8_t delay_high; 	/* delay between H n L*/
	uint8_t delay_low; 		/* delay between H n L*/
	uint8_t hv_OnTime; 		/* HV On time */
	uint8_t hv_OffTime; 	/* HV Off time */
	uint8_t lv_OnTime_high; /* LV On time */
	uint8_t lv_OnTime_low; 	/* LV On time */
	uint8_t lv_OffTime_high;/* LV Off time */
	uint8_t lv_OffTime_low; /* LV Off time */
} FSP_CMD_SET_ALL_FRAME;

typedef struct _FSP_CMD_CONFIRM_FRAME_ {
	uint8_t sequence_index;
} FSP_CMD_CONFIRM_FRAME;

typedef struct _FSP_CMD_SEQUENCE_FRAME_ {
	uint8_t state;	 	/* 1: Run / 2: Pause */
} FSP_CMD_SEQUENCE_FRAME;

typedef struct _FSP_CMD_KILL_FRAME_ {
	uint8_t State; /* 0: Normal, 1: Kill */
} FSP_CMD_KILL_FRAME;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ I2C Sensor Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _FSP_CMD_MEASURE_BMP_FRAME_ {
	uint8_t temp[5];
	uint8_t pressure[6];

} FSP_CMD_MEASURE_BMP_FRAME;

typedef struct _FSP_CMD_MEASURE_LSM_FRAME_ {
	uint8_t accel_x[4];
	uint8_t accel_y[4];
	uint8_t accel_z[4];
	uint8_t gyro_x[4];
	uint8_t gyro_y[4];
	uint8_t gyro_z[4];

} FSP_CMD_MEASURE_LSM_FRAME;

// Union to encapsulate all frame types
typedef union _FSP_Payload_Frame_typedef_ {
	FSP_CMD_GET_ALL_FRAME		get_all;
	FSP_CMD_SET_ALL_FRAME  		set_all;
	FSP_CMD_CONFIRM_FRAME		sequence_confirm;
	FSP_CMD_SEQUENCE_FRAME		set_sequence;
	FSP_CMD_KILL_FRAME			set_kill;

	FSP_CMD_MEASURE_BMP_FRAME	get_bmp;
	FSP_CMD_MEASURE_LSM_FRAME	get_lsm;


} FSP_Payload_Frame_typedef;

typedef struct _FSP_Payload_typedef_ {
	e_FSP_CMD_typedef CMD;
	FSP_Payload_Frame_typedef Payload;

} FSP_Payload;

/* :::::::::: FSP Line Process ::::::::::::: */
void FSP_Line_Process();
void fsp_print(uint8_t packet_length);
#endif
