///*
// * LOG.h
// *
// *  Created on: Dec 23, 2024
// *      Author: thanh
// */
//
#ifndef LOG1_LOG_H_
#define LOG1_LOG_H_

#include <stdint.h>

#include "fatfs.h"
void Update_Status_Task();
FRESULT append_to_log(const char *hfm);
void create_file_list_screen(void);
void writeCSV(void);
#endif /* LOG_LOG_H_ */
