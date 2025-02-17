/*
 * LOG.c
 *
 *  Created on: Dec 23, 2024
 *      Author: thanh
 */

#include "LOG.h"
#include "main.h"
#include "AppDrv.h"
#include "App.h"
extern volatile uint32_t Run_Time;
extern Disk_drvTypeDef disk;
static FATFS fs;  // File system object
static uint8_t is_mounted = 0;
static lv_obj_t *file_list;
static char file_content_buffer[1024]; // Buffer để chứa nội dung file
static const char *StateCode[7] = { "OFF", "READY", "CHARGE", "WAIT", "RUN",
		"ERROR", "DISCONNECT" };
/******************************************************/
static FRESULT auto_mount(void);
static void list_files_on_sd(const char *path, lv_obj_t *list);
static void file_item_clicked(lv_event_t *e);
static void file_item_hold(lv_event_t *e);
static uint8_t CSV_Process(uint8_t *Row, char (*buff)[20]);

void Update_Status_Task() {
	char text[50];
	snprintf(text, 50, "%s\n%u\n%u\nN/a\nN/a\nN/a\n%u\n%u\n%u",
			StateCode[Running_Paramater.Status],
			Running_Paramater.HS_CurrentVolt, Running_Paramater.LS_CurrentVolt,
			Running_Paramater.Total_Sequence,
			Running_Paramater.Total_High_Pulse,
			Running_Paramater.Total_Low_Pulse);
	lv_label_set_text(ui_Label23, (const char*) text);
}

FRESULT append_to_log(const char *hfm) {
	FRESULT res = auto_mount();
	if (res != FR_OK) {
		return res;
	}

	FIL file;
	UINT bytesWritten;

	res = f_open(&file, "LOG.CSV", FA_WRITE | FA_OPEN_ALWAYS);
	if (res != FR_OK) {
		return res;
	}
	res = f_lseek(&file, f_size(&file));
	if (res != FR_OK) {
		f_close(&file);
		return res;
	}

	char line[256];
	snprintf(line, sizeof(line), "%s\r\n", hfm);
	res = f_write(&file, line, strlen(line), &bytesWritten);
	if (res != FR_OK || bytesWritten < strlen(line)) {
		f_close(&file);
		return res;
	}

	f_close(&file);
	return FR_OK;
}

void update_file_list(void) {
	if (file_list != NULL) {
		list_files_on_sd("0:/", file_list);
	}
}
void create_file_list_screen(void) {
	file_list = lv_list_create(ui_TabPage4);
	lv_obj_set_size(file_list, 300, 100);
	lv_obj_align(file_list, LV_ALIGN_CENTER, -70, 0);

	list_files_on_sd("0:/", file_list);
}

static FRESULT auto_mount(void) {
	if ((SD_DETECT_GPIO_Port->IDR & SD_DETECT_Pin) != SD_DETECT_Pin) {
		if (!is_mounted) {
			MX_FATFS_Init();
			FRESULT res = f_mount(&fs, "", 1);
			if (res == FR_OK) {
				is_mounted = 1;
			}
			return res;
		}
		return FR_OK;
	} else {
		if (is_mounted) {
			disk.is_initialized[0] = 0;
			f_mount(NULL, "", 1);
			is_mounted = 0;
		}
		return FR_NOT_READY;
	}
}

static void list_files_on_sd(const char *path, lv_obj_t *list) {
	FRESULT res;
	DIR dir;
	FILINFO fno;

	res = auto_mount();
	if (res != FR_OK) {
		lv_obj_clean(list);
		lv_list_add_text(list, "Error: SD card not ready!");
		return;
	}

	lv_obj_clean(list);

	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		while (1) {
			// Đọc từng mục trong thư mục
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
				break; // Hết file hoặc lỗi

			lv_obj_t *btn;
			if (fno.fattrib & AM_DIR) {
				// Nếu là thư mục, thêm tên kèm dấu "/"
				char folder_name[256];
				snprintf(folder_name, sizeof(folder_name), "%s/", fno.fname);
				btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, folder_name);
			} else {
				// Nếu là file, thêm tên file
				btn = lv_list_add_btn(list, LV_SYMBOL_FILE, fno.fname);
			}

			// Gán callback cho từng nút trong danh sách
			lv_obj_add_event_cb(btn, file_item_clicked, LV_EVENT_CLICKED, NULL);
			lv_obj_add_event_cb(btn, file_item_hold, LV_EVENT_LONG_PRESSED,
			NULL);
		}
		f_closedir(&dir);
	} else {
		// Xử lý lỗi khi không mở được thư mục
		lv_list_add_text(list, "Error: Cannot open directory!");
	}
}

static void file_item_clicked(lv_event_t *e) {
	lv_obj_t *btn = lv_event_get_target(e);
	const char *file_name = lv_list_get_btn_text(file_list, btn);

	// Đường dẫn đầy đủ tới file
	char file_path[256];
	snprintf(file_path, sizeof(file_path), "0:/%s", file_name);

	// Mở file và đọc nội dung
	FIL file;
	UINT bytes_read;
	FRESULT res = f_open(&file, file_path, FA_READ);
	if (res == FR_OK) {
		// Đọc nội dung file vào buffer
		res = f_read(&file, file_content_buffer,
				sizeof(file_content_buffer) - 1, &bytes_read);
		if (res == FR_OK) {
			file_content_buffer[bytes_read] = '\0';
		} else {
//           printf("Error reading file: %s\n", file_name);
		}
		f_close(&file);
	} else {
//       printf("Error opening file: %s\n", file_name);
	}
	parseCSVToStruct(&file_content_buffer);
}
static void file_item_hold(lv_event_t *e) {
	lv_obj_t *btn = lv_event_get_target(e);  // Lấy nút được nhấn giữ
	const char *file_name = lv_list_get_btn_text(file_list, btn);

	// Đường dẫn đầy đủ tới file
	char file_path[256];
	snprintf(file_path, sizeof(file_path), "0:/%s", file_name);

//    if (lv_msgbox_yes_no("Delete File", "Are you sure you want to delete this file?", LV_SYMBOL_OK, LV_SYMBOL_CLOSE)) {
	// Xóa file
	FRESULT res = f_unlink(file_path);
	if (res == FR_OK) {
		printf("File deleted successfully: %s\n", file_name);
		update_file_list();
//		lv_list_remove_btn(file_list, btn);
	} else {
		printf("Error deleting file: %s (Error: %d)\n", file_name, res);
	}
//    } else {
//        printf("File deletion cancelled: %s\n", file_name);
//    }

}

void parseCSVToStruct(char *buffer) {
	uint8_t col = 0;
	uint8_t row = 0;
	char data_row[22][20];
	char temp_value[20];
	uint8_t temp_index = 0;
	char data = *buffer++;
	uint8_t set_seq = 0;
	while (data) {
		if (data == ',') {
			temp_value[temp_index] = '\0';
			memcpy(data_row[col], temp_value, ++temp_index);
			col++;
			temp_index = 0;
		} else if (data == '\n') {
			temp_value[temp_index] = '\0';
			memcpy(data_row[col], temp_value, ++temp_index);
			set_seq = CSV_Process(&row, &data_row);
			if (set_seq == 0xFF)
				break;
			row++;
			col = 0;
			temp_index = 0;
		} else {
			if (data != '\r')
				temp_value[temp_index++] = data;
		}
		data = *buffer++;
	}
	s_sequence seq_tmp = { 0 };

	if (set_seq == 0xFF || set_seq != (Running_Paramater.Total_Sequence - 1)) {
		set_seq = 0;
		show_message("Data is valid please try again", 3000);
	} else {
		Running_Paramater.Sequence_Index = set_seq - 1;
		simulate_dropdown_change(set_seq - 1);

	}
	for (set_seq; set_seq < 19; set_seq++)
		Sequence[set_seq + 1] = seq_tmp;
}
static uint8_t CSV_Process(uint8_t *Row, char (*buff)[20]) {
	uint8_t seq_idx = 0;
	if (*Row == 0) {
		lv_textarea_set_text(ui_TextArea1, buff[3]);
		lv_textarea_set_text(ui_TextArea2, buff[5]);
		Running_Paramater.Total_Sequence = atoi(buff[1]);
		Running_Paramater.HS_SetVolt = atoi(buff[3]);
		Running_Paramater.LS_SetVolt = atoi(buff[5]);
		return 0;
	} else if (*Row == 1) {
		return 0;
	} else {
		seq_idx = atoi(buff[0]) - 1;
		if (seq_idx > Running_Paramater.Total_Sequence)
			return 0xFF;
		s_sequence seq_tmp;

		seq_tmp.Positive_Pole = atoi(buff[1]) - 1;
		seq_tmp.Negative_Pole = atoi(buff[2]) - 1;

		seq_tmp.HighSide.NumPos = atoi(buff[5]);
		seq_tmp.HighSide.NumNeg = atoi(buff[6]);
		seq_tmp.HighSide.OnTime = atoi(buff[3]);
		seq_tmp.HighSide.OffTime = atoi(buff[4]);
		seq_tmp.HighSide.Delay = atoi(buff[7]);

		seq_tmp.LowSide.NumPos = atoi(buff[10]);
		seq_tmp.LowSide.NumNeg = atoi(buff[11]);
		seq_tmp.LowSide.OnTime = atoi(buff[8]);
		seq_tmp.LowSide.OffTime = atoi(buff[9]);
		seq_tmp.LowSide.Delay = atoi(buff[12]);

		seq_tmp.Delay = atoi(buff[13]);

		if (ValidateSequence(&seq_tmp))
			Sequence[seq_idx] = seq_tmp;
		else
			return 0xFF;
		return seq_idx;
	}
}

int get_next_file_index(const char *dir_path) {
	// Gọi auto_mount để tự động gắn thẻ SD nếu chưa được gắn
	FRESULT mount_res = auto_mount();
	if (mount_res != FR_OK) {
		// Nếu không thể gắn thẻ SD, trả về lỗi
		return -1;
	}

	// Duyệt qua các tệp trong thư mục để tìm chỉ số lớn nhất
	DIR dir;
	FRESULT res = f_opendir(&dir, dir_path);  // Mở thư mục
	if (res != FR_OK) {
		// Nếu không thể mở thư mục, trả về lỗi
		return -1;
	}

	FILINFO fno;
	int max_index = 0;

	// Duyệt qua các tệp trong thư mục
	while ((res = f_readdir(&dir, &fno)) == FR_OK && fno.fname[0] != 0) {
		// Kiểm tra nếu tệp là một tệp CSV
		if (strstr(fno.fname, ".csv") != NULL) {
			int index;
			// Kiểm tra nếu tên tệp có định dạng data<number>.csv
			if (sscanf(fno.fname, "Sequence%d.csv", &index) == 1) {
				// Cập nhật chỉ số lớn nhất
				if (index > max_index) {
					max_index = index;
				}
			}
		}
	}

	f_closedir(&dir);  // Đóng thư mục

	return max_index + 1;  // Trả về chỉ số tiếp theo
}

void writeCSV(void) {
	FRESULT res;
	FIL fil;        // File pointer
	char buffer[256];  // Dùng để chứa từng dòng dữ liệu CSV
	UINT bytesWritten;
	char filename[32];  // Dùng để chứa tên file

	// Tạo tên file theo thứ tự File_Count
	sprintf(filename, "Sequence%u.csv", File_Count);

	// Gọi auto_mount để tự động gắn thẻ SD nếu chưa được gắn
	FRESULT mount_res = auto_mount();
	if (mount_res != FR_OK) {
		// Nếu không thể gắn thẻ SD, trả về lỗi
		return;
	}

	// Mở file CSV (Ghi đè nếu đã tồn tại)
	res = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
	if (res != FR_OK) {
		show_message("Creat file failed", 3000);
		return;
	}

	// Ghi dòng đầu tiên (Total Sequence)
	sprintf(buffer, "Total Sequence,%d,HighVolt,%d,LowVolt,%d,,,,,,,,\n",
			Running_Paramater.Total_Sequence, Running_Paramater.HS_SetVolt, // HighVolt
			Running_Paramater.LS_SetVolt); // LowVolt
	res = f_write(&fil, buffer, strlen(buffer), &bytesWritten);
	if (res != FR_OK) {
		show_message("Write first line failed", 3000);
		f_close(&fil);
		return;
	}

	// Ghi tiêu đề cột (header) sau dòng đầu tiên
	sprintf(buffer,
			"Sequence,Positive Pole,Negative Pole,Hton,Htoff,HPCount,HLCount,Hdelay,Lton,Ltoff,LPCount,LLCount,Ldelay,Delay\n");
	res = f_write(&fil, buffer, strlen(buffer), &bytesWritten);
	if (res != FR_OK) {
		show_message("Write header failed", 3000);
		f_close(&fil);
		return;
	}

	// Ghi dữ liệu cho mỗi Sequence
	for (int i = 0; i < Running_Paramater.Total_Sequence; i++) {
		s_sequence seq = Sequence[i];

		// Tạo chuỗi dữ liệu cho mỗi dòng
		sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", i + 1, // Sequence index (1-based)
		seq.Positive_Pole, seq.Negative_Pole, seq.HighSide.OnTime,
				seq.HighSide.OffTime, seq.HighSide.NumPos, seq.HighSide.NumNeg,
				seq.HighSide.Delay, seq.LowSide.OnTime, seq.LowSide.OffTime,
				seq.LowSide.NumPos, seq.LowSide.NumNeg, seq.LowSide.Delay,
				seq.Delay);

		// Ghi dòng vào file CSV
		res = f_write(&fil, buffer, strlen(buffer), &bytesWritten);
		if (res != FR_OK) {
			show_message("Write data failed", 3000);
			f_close(&fil);
			return;
		}
	}

	// Đóng file
	f_close(&fil);
	sprintf(buffer, "CSV file %s written successfully.", filename);
	show_message(&buffer, 3000);
	// Tăng File_Count cho lần sau
	File_Count++;
	update_file_list();
}
