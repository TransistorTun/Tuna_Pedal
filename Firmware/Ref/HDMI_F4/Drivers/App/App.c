/*
 * App.c
 *
 *  Created on: Nov 14, 2024
 *      Author: thanh
 */
#define MAX_MESSAGES 5
#define DEFAUT_MESSAGES_TIMEOUT 3000

#include "App.h"
#include "main.h"
#include "AppDrv.h"
#include "ui.h"
#include "System.h"
#include "fsp.h"
#include "fsp_frame.h"
#include <stdbool.h>
static const char *btnm_map[] = { "1", "2", "3", "\n", "4", "5", "6", "\n", "7",
		"8", "9", "\n", "0", LV_SYMBOL_BACKSPACE, "" };
const char *btns[] = { "OK", "Cancel", "" };

//lv_obj_t *ui_Chart2;
static lv_style_t style_line_back;
static lv_style_t style_line_highside;
static lv_style_t style_line_lowside;
lv_obj_t *line2;
lv_obj_t *line3;
lv_obj_t *list;

static lv_point_t line_points_high[200];
static lv_point_t line_points_low[200];

static const char *message_queue[MAX_MESSAGES];
static uint8_t message_queue_head = 0;
static uint8_t message_queue_tail = 0;
static lv_obj_t *msg_box = NULL;
static lv_obj_t *label = NULL;
static lv_obj_t *close_btn = NULL;
static lv_timer_t *timer = NULL;

static void keyneba(lv_event_t *e);
uint8_t check_sequence_validity(s_sequence *seq);
static void check_and_call_pulse_status(const s_pulse_status *status,
bool isHigh);
static void update_sequence_screen(s_sequence *sequence);
static void calculate_totals_and_update();

static void Sequence_Delete(lv_event_t *e) {
	s_sequence sequence = { 0 };
	Sequence[Running_Paramater.Sequence_Index] = sequence;
	if (Running_Paramater.Sequence_Index > 0)
		Running_Paramater.Sequence_Index--;
	else {
		lv_textarea_set_text(ui_TextArea1, "");
		lv_textarea_set_text(ui_TextArea2, "");
		Running_Paramater.HS_SetVolt = 0;
		Running_Paramater.LS_SetVolt = 0;
	}
	if (Running_Paramater.Total_Sequence > 0)
		Running_Paramater.Total_Sequence--;
	lv_dropdown_set_selected(ui_Dropdown1, Running_Paramater.Sequence_Index);
	update_sequence_screen(&Sequence[Running_Paramater.Sequence_Index]);
	calculate_totals_and_update();
}

static void Sequence_Save(lv_event_t *e) {
	writeCSV();
}
void Draw_Chart(uint8_t drawhigh, uint8_t drawlow) {
	if (drawhigh)
		Draw_Rect(30, 77,
				Sequence[Running_Paramater.Sequence_Index].HighSide.OnTime,
				Sequence[Running_Paramater.Sequence_Index].HighSide.OffTime,
				Sequence[Running_Paramater.Sequence_Index].HighSide.Delay,
				Sequence[Running_Paramater.Sequence_Index].HighSide.NumNeg,
				Sequence[Running_Paramater.Sequence_Index].HighSide.NumPos, 1);
	if (drawlow)
		Draw_Rect(30, 191,
				Sequence[Running_Paramater.Sequence_Index].LowSide.OnTime,
				Sequence[Running_Paramater.Sequence_Index].LowSide.OffTime,
				Sequence[Running_Paramater.Sequence_Index].LowSide.Delay,
				Sequence[Running_Paramater.Sequence_Index].LowSide.NumNeg,
				Sequence[Running_Paramater.Sequence_Index].LowSide.NumPos, 0);

}

void Chart_Init() {
	lv_style_init(&style_line_back);
	lv_style_set_line_width(&style_line_back, 1);
	lv_style_set_line_color(&style_line_back, lv_color_hex(0x000000));
	lv_style_set_line_rounded(&style_line_back, true);

	lv_style_init(&style_line_highside);
	lv_style_set_line_width(&style_line_highside, 1);
	lv_style_set_line_color(&style_line_highside, lv_color_hex(0xFF0000));
	lv_style_set_line_rounded(&style_line_highside, true);

	lv_style_init(&style_line_lowside);
	lv_style_set_line_width(&style_line_lowside, 1);
	lv_style_set_line_color(&style_line_lowside, lv_color_hex(0x0000FF));
	lv_style_set_line_rounded(&style_line_lowside, true);

	static lv_point_t line_points[10] = { { 20, 20 }, { 20, 248 }, { 20, 191 },
			{ 554, 191 }, { 20, 191 }, { 20, 77 }, { 554, 77 } };
	/*Create a line and apply the new style*/
	lv_obj_t *line1 = lv_line_create(ui_Container3);
	line2 = lv_line_create(ui_Container3);
	line3 = lv_line_create(ui_Container3);

	lv_line_set_points(line1, line_points, 7); /*Set the points*/
	lv_obj_add_style(line1, &style_line_back, 0);
	lv_obj_add_style(line2, &style_line_highside, 0);
	lv_obj_add_style(line3, &style_line_lowside, 0);

	create_file_list_screen();
}

static void message_changed(lv_event_t *e) {
	lv_obj_t *obj = lv_event_get_current_target(e);
	const char *txt = lv_msgbox_get_active_btn_text(obj);

	if (strcmp(txt, "OK") == 0) {
		SystemStatus = POWER_OFF;
		POWER_ON_GPIO_Port->BSRR = (uint32_t) (POWER_ON_Pin) << 16;
	}
	lv_msgbox_close(obj);
}

void Poweroff(lv_event_t *e) {
	lv_obj_t *mbox1 = lv_msgbox_create(ui_Main, "System message",
			"Do you want to shut down", btns, false);

	lv_obj_add_event_cb(mbox1, message_changed, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_center(mbox1);
}

void Numpad_Init() {
	ui_Keyboard1 = lv_btnmatrix_create(ui_Run);
	lv_obj_remove_style_all(ui_Keyboard1);
	lv_obj_set_width(ui_Keyboard1, 203);
	lv_obj_set_height(ui_Keyboard1, 142);
	lv_obj_set_x(ui_Keyboard1, 292);
	lv_obj_set_y(ui_Keyboard1, 169);
	lv_obj_clear_flag(ui_Keyboard1, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_btnmatrix_set_map(ui_Keyboard1, btnm_map);
	lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);   /// Flags
	lv_obj_set_style_radius(ui_Keyboard1, 5, LV_PART_ITEMS | LV_STATE_DEFAULT);

	lv_obj_set_style_pad_left(ui_Keyboard1, 10,
			LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui_Keyboard1, 10,
			LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui_Keyboard1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui_Keyboard1, 10,
			LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_row(ui_Keyboard1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_column(ui_Keyboard1, 10,
			LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_style_bg_opa(ui_Keyboard1, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui_Keyboard1, 1,
			LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_outline_color(ui_Keyboard1, lv_color_hex(0x000000),
			LV_PART_ANY);
	lv_obj_set_style_outline_opa(ui_Keyboard1, 0, LV_PART_ANY);
	lv_obj_set_style_shadow_opa(ui_Keyboard1, 0,
			LV_PART_ITEMS | LV_STATE_DEFAULT);

	lv_obj_set_style_bg_color(ui_Keyboard1, lv_color_hex(0xFF797B),
			LV_PART_ITEMS | LV_STATE_PRESSED);
	lv_obj_set_style_bg_opa(ui_Keyboard1, 255,
			LV_PART_ITEMS | LV_STATE_PRESSED);

	lv_obj_add_event_cb(ui_Keyboard1, keyneba, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_set_style_base_dir(ui_Keyboard1, LV_BASE_DIR_LTR, 0);
	lv_keyboard_t *keyboard = (lv_keyboard_t*) ui_Keyboard1;
	keyboard->ta = NULL;
}

void Edit_HV(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea1));
	if (value > DEFAUT_HS_MAX_VOLT)
		lv_textarea_set_text(ui_TextArea1, "300");
	else
		Running_Paramater.HS_SetVolt = value;
	Draw_Chart(1, 0);
}

void Edit_HOnT(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea3));
	if (value > DEFAUT_HS_MAX_ONTIME)
		lv_textarea_set_text(ui_TextArea3, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].HighSide.OnTime = value;
	Draw_Chart(1, 0);
}

void Edit_HOfT(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea4));
	if (value > DEFAUT_HS_MAX_OFFTIME)
		lv_textarea_set_text(ui_TextArea4, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].HighSide.OffTime = value;
	Draw_Chart(1, 0);
}

void Edit_HPC(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea5));
	if (value > DEFAUT_HS_MAX_PCOUNT)
		lv_textarea_set_text(ui_TextArea5, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].HighSide.NumPos = value;
	Draw_Chart(1, 0);
}

void Edit_HNC(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea6));
	if (value > DEFAUT_HS_MAX_NCOUNT)
		lv_textarea_set_text(ui_TextArea6, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].HighSide.NumNeg = value;
	Draw_Chart(1, 0);
}

void Edit_LV(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea2));
	if (value > DEFAUT_LS_MAX_VOLT)
		lv_textarea_set_text(ui_TextArea2, "50");
	else
		Running_Paramater.LS_SetVolt = value;
	Draw_Chart(0, 1);
}

void Edit_LOnT(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea7));
	if (value > DEFAUT_LS_MAX_ONTIME)
		lv_textarea_set_text(ui_TextArea7, "1000");
	else
		Sequence[Running_Paramater.Sequence_Index].LowSide.OnTime = value;
	Draw_Chart(0, 1);
}

void Edit_LOfT(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea8));
	if (value > DEFAUT_LS_MAX_OFFTIME)
		lv_textarea_set_text(ui_TextArea8, "1000");
	else
		Sequence[Running_Paramater.Sequence_Index].LowSide.OffTime = value;
	Draw_Chart(0, 1);
}

void Edit_LPC(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea9));
	if (value > DEFAUT_LS_MAX_PCOUNT)
		lv_textarea_set_text(ui_TextArea9, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].LowSide.NumPos = value;
	Draw_Chart(0, 1);
}

void Edit_LNC(lv_event_t *e) {
	uint16_t value = atoi(lv_textarea_get_text(ui_TextArea10));
	if (value > DEFAUT_LS_MAX_NCOUNT) {
		lv_textarea_set_text(ui_TextArea10, "20");
	} else {
		Sequence[Running_Paramater.Sequence_Index].LowSide.NumNeg = value;
	}
	Draw_Chart(0, 1);
}

void Edit_DHV(lv_event_t *e) {
	lv_obj_t *textarea = ui_comp_get_child(ui_Container16,
	UI_COMP_CONTAINER1_TEXTAREA1);
	uint16_t value = atoi(lv_textarea_get_text(textarea));
	if (value > DEFAUT_HS_DELAY_PN)
		lv_textarea_set_text(textarea, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].HighSide.Delay = value;
	Draw_Chart(1, 0);
}

void Edit_DLV(lv_event_t *e) {
	lv_obj_t *textarea = ui_comp_get_child(ui_Container17,
	UI_COMP_CONTAINER1_TEXTAREA1);
	uint16_t value = atoi(lv_textarea_get_text(textarea));
	if (value > DEFAUT_LS_DELAY_PN)
		lv_textarea_set_text(textarea, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].LowSide.Delay = value;
	Draw_Chart(0, 1);
}

void Edit_DHL(lv_event_t *e) {
	lv_obj_t *textarea = ui_comp_get_child(ui_Container18,
	UI_COMP_CONTAINER1_TEXTAREA1);
	uint16_t value = atoi(lv_textarea_get_text(textarea));
	if (value > DEFAUT_HLS_DELAY)
		lv_textarea_set_text(textarea, "20");
	else
		Sequence[Running_Paramater.Sequence_Index].Delay = value;
	Draw_Chart(0, 1);
}

void Run_SW_PRESS(lv_event_t *e) {
	is_Confirm = false;
	is_Sequence_Send = false;
	Send_Sequence_Index = 0;
	Total_Sequence = Running_Paramater.Total_Sequence ;
	Running_Paramater.Status = WAITs;
}

void PAUSE_SW_PRESS(lv_event_t *e) {
	ps_FSP_TX->CMD = FSP_CMD_KILL;
	ps_FSP_TX->Payload.set_kill.State = 0x03;
	fsp_print(2);
}

void Edit_Sequence(lv_event_t *e) {
	uint8_t set_sequence_index = lv_dropdown_get_selected(ui_Dropdown1);
	uint8_t current_sequence_index = Running_Paramater.Sequence_Index;
	s_sequence *sequence = &Sequence[current_sequence_index];
//	if (set_sequence_index == current_sequence_index) {
//		Running_Paramater.Total_Sequence = current_sequence_index + 1;
//		return;
//	}

	if (set_sequence_index == 0) {
		sequence = &Sequence[0]; // Cập nhật lại sequence 0
		Running_Paramater.Sequence_Index = 0;
		Running_Paramater.Total_Sequence = 1;
		update_sequence_screen(sequence); // Cập nhật màn hình
		calculate_totals_and_update();
		return;
	}

	if (set_sequence_index >= current_sequence_index) {
		if (!check_sequence_validity(sequence)) {
			lv_dropdown_set_selected(ui_Dropdown1, current_sequence_index);
			update_sequence_screen(sequence);
			return;
		} else {
			lv_dropdown_set_selected(ui_Dropdown1, ++current_sequence_index);
			Running_Paramater.Sequence_Index = current_sequence_index;
			Running_Paramater.Total_Sequence = current_sequence_index + 1;
			sequence = &Sequence[current_sequence_index];
			update_sequence_screen(sequence);
			calculate_totals_and_update();
			return;
		}

	}

	if (set_sequence_index < current_sequence_index) {
		sequence = &Sequence[set_sequence_index];
		Running_Paramater.Total_Sequence = set_sequence_index + 1;
		Running_Paramater.Sequence_Index = set_sequence_index;
		update_sequence_screen(sequence);
		calculate_totals_and_update();
		return;
	}
}

static void update_sequence_screen(s_sequence *sequence) {
	char buf[5];

	// Cập nhật các dropdown với các cực của sequence
	lv_dropdown_set_selected(ui_Dropdown2, sequence->Positive_Pole);
	lv_dropdown_set_selected(ui_Dropdown3, sequence->Negative_Pole);

	// Kiểm tra và gọi status pulse cho HighSide và LowSide
	check_and_call_pulse_status(&sequence->HighSide, true);
	check_and_call_pulse_status(&sequence->LowSide, false);

	// Cập nhật Delay và hiển thị trên giao diện
	uint8_t current_sequence_index = sequence->Delay;
	if (current_sequence_index == 0) {
		lv_textarea_set_text(
				ui_comp_get_child(ui_Container18, UI_COMP_CONTAINER1_TEXTAREA1),
				"");
	} else {
		itoa(current_sequence_index, buf, 10);
		lv_textarea_set_text(
				ui_comp_get_child(ui_Container18, UI_COMP_CONTAINER1_TEXTAREA1),
				buf);
	}

	// Vẽ lại biểu đồ
	Draw_Chart(1, 1);
}

void Edit_PositivePole(lv_event_t *e) {
	uint8_t pole = lv_dropdown_get_selected(ui_Dropdown2);
	if (pole == Sequence[Running_Paramater.Sequence_Index].Negative_Pole) {
		if (pole < 6)
			pole++;
		else
			pole--;
		show_message(
				"The positive pole must be different from the negative pole",
				DEFAUT_MESSAGES_TIMEOUT);
	}
	Sequence[Running_Paramater.Sequence_Index].Positive_Pole = pole;
	lv_dropdown_set_selected(ui_Dropdown2, pole);
}

void Edit_NegativePole(lv_event_t *e) {
	uint8_t pole = lv_dropdown_get_selected(ui_Dropdown3);
	if (pole == Sequence[Running_Paramater.Sequence_Index].Positive_Pole) {
		if (pole < 6)
			pole++;
		else
			pole--;
		show_message(
				"The negative pole must be different from the positive pole",
				DEFAUT_MESSAGES_TIMEOUT);
	}
	Sequence[Running_Paramater.Sequence_Index].Negative_Pole = pole;
	lv_dropdown_set_selected(ui_Dropdown3, pole);
}

static void keyneba(lv_event_t *e) {
	lv_obj_t *obj = lv_event_get_target(e);

	LV_ASSERT_OBJ(obj, MY_CLASS);
	lv_keyboard_t *keyboard = (lv_keyboard_t*) obj;
	uint16_t btn_id = lv_btnmatrix_get_selected_btn(obj);
	if (btn_id == LV_BTNMATRIX_BTN_NONE)
		return;

	const char *txt = lv_btnmatrix_get_btn_text(obj,
			lv_btnmatrix_get_selected_btn(obj));
	if (txt == NULL)
		return;

	/*Add the characters to the text area if set*/
	if (keyboard->ta == NULL)
		return;
	if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
		lv_textarea_del_char(keyboard->ta);
	} else {
		lv_textarea_add_text(keyboard->ta, txt);
	}
}

uint16_t Draw_Rect(uint16_t x_coordinates, uint16_t y_coordinates, uint8_t Ton,
		uint8_t Toff, uint8_t delay, uint8_t NegCount, uint8_t PosCount,
		uint8_t isHigh) {
	lv_point_t *line_points;
	uint8_t Start = 0;
	uint16_t Height;

	if (Ton == 0 || Toff == 0) {
		if (isHigh)
			lv_obj_add_flag(line2, LV_OBJ_FLAG_HIDDEN);
		else
			lv_obj_add_flag(line3, LV_OBJ_FLAG_HIDDEN);
		return x_coordinates;
	}
	if (isHigh) {
		line_points = line_points_high;
		Height = Running_Paramater.HS_SetVolt / 6;
	} else {
		line_points = line_points_low;
		x_coordinates += Sequence[Running_Paramater.Sequence_Index].Delay;
		Height = Running_Paramater.LS_SetVolt / 2;
	}
	uint32_t total_width = (PosCount + NegCount) * (Ton + Toff) + delay;

	float available_width = 554 - x_coordinates;
	float scale_factor =
			(total_width > available_width) ?
					(available_width / total_width) : 1.0;

	// Scale lại Ton và Toff
	Ton = Ton * scale_factor;
	Toff = Toff * scale_factor;
	delay = delay * scale_factor;

	if (Height == 0) {
		if (isHigh)
			lv_obj_add_flag(line2, LV_OBJ_FLAG_HIDDEN);
		else
			lv_obj_add_flag(line3, LV_OBJ_FLAG_HIDDEN);
		return x_coordinates;
	} else {
		if (isHigh)
			lv_obj_clear_flag(line2, LV_OBJ_FLAG_HIDDEN);
		else
			lv_obj_clear_flag(line3, LV_OBJ_FLAG_HIDDEN);
	}

	// Vẽ xung dương
	for (uint8_t i = 0; i < PosCount; i++) {
		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates;

		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates - Height;

		x_coordinates += Ton;
		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates - Height;

		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates;

		x_coordinates += Toff;
		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates;
	}

	x_coordinates += delay;
	// Vẽ xung âm
	for (uint8_t i = 0; i < NegCount; i++) {
		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates;

		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates + Height;

		x_coordinates += Ton;
		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates + Height;

		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates;

		x_coordinates += Toff;
		line_points[Start].x = x_coordinates;
		line_points[Start++].y = y_coordinates;
	}

	if (isHigh) {
		lv_line_set_points(line2, line_points, Start);
	} else {
		lv_line_set_points(line3, line_points, Start);
	}

	return x_coordinates;
}

static void check_and_call_pulse_status(const s_pulse_status *status,
bool isHigh) {
	const uint16_t *fields[] = { &status->OnTime, &status->OffTime,
			(uint16_t*) &status->NumPos, (uint16_t*) &status->NumNeg,
			(uint16_t*) &status->Delay };

	lv_obj_t *TextAreas_HighSide[] = { ui_TextArea3, ui_TextArea4, ui_TextArea5,
			ui_TextArea6, ui_comp_get_child(ui_Container16,
			UI_COMP_CONTAINER1_TEXTAREA1), ui_TextArea7, ui_TextArea8,
			ui_TextArea9, ui_TextArea10, ui_comp_get_child(ui_Container17,
			UI_COMP_CONTAINER1_TEXTAREA1) };

	char buf[5];
	uint16_t value;
	uint8_t adr_offset = 0;
	if (!isHigh)
		adr_offset = 5;
	for (uint8_t i = 0; i < 5; i++) {
		value = *fields[i];
		if (value == 0) {
			lv_textarea_set_text(TextAreas_HighSide[i + adr_offset], "");
		} else {
			itoa(value, buf, 10);
			lv_textarea_set_text(TextAreas_HighSide[i + adr_offset], buf);
		}
	}
}

void close_message_event_cb(lv_event_t *e) {
	lv_obj_del(msg_box);
	msg_box = NULL;
	label = NULL;
	close_btn = NULL;

	if (message_queue_head != message_queue_tail) {
		const char *next_message = message_queue[message_queue_head];
		message_queue_head = (message_queue_head + 1) % MAX_MESSAGES;
		show_message(next_message, DEFAUT_MESSAGES_TIMEOUT); // Hiển thị thông báo tiếp theo với timeout
	}
}

void message_timer_cb(lv_timer_t *timer) {
	lv_obj_del(msg_box);
	msg_box = NULL;
	label = NULL;
	close_btn = NULL;
	lv_timer_del(timer);
	if (message_queue_head != message_queue_tail) {
		const char *next_message = message_queue[message_queue_head];
		message_queue_head = (message_queue_head + 1) % MAX_MESSAGES;
		show_message(next_message, DEFAUT_MESSAGES_TIMEOUT); // Hiển thị thông báo tiếp theo
	}
}

void show_message(const char *message, uint32_t timeout_ms) {
	if (msg_box != NULL) {
		message_queue[message_queue_tail] = message;
		message_queue_tail = (message_queue_tail + 1) % MAX_MESSAGES;
		return;
	}

	msg_box = lv_obj_create(lv_scr_act());
	lv_obj_set_size(msg_box, 400, 150);
	lv_obj_align(msg_box, LV_ALIGN_CENTER, 0, 0);
	lv_obj_clear_flag(msg_box, LV_OBJ_FLAG_SCROLLABLE);
	label = lv_label_create(msg_box);
	lv_label_set_text(label, message);
	lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label, 380);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);
	lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

	close_btn = lv_btn_create(msg_box);
	lv_obj_set_size(close_btn, 60, 30);
	lv_obj_align(close_btn, LV_ALIGN_CENTER, 0, 40);
	lv_obj_t *btn_label = lv_label_create(close_btn);
	lv_label_set_text(btn_label, "Close");
	lv_obj_center(btn_label);

	lv_obj_add_event_cb(close_btn, close_message_event_cb, LV_EVENT_CLICKED,
	NULL);

	if (timeout_ms > 0) {
		timer = lv_timer_create(message_timer_cb, timeout_ms, msg_box);
	}
}

uint8_t check_sequence_validity(s_sequence *seq) {
	if (seq->Positive_Pole == 0 && seq->Negative_Pole == 0) {
		show_message(
				"Both output poles are not configured yet, please try again",
				DEFAUT_MESSAGES_TIMEOUT);
		return 0;
	}

	if ((seq->HighSide.NumPos != 0 || seq->HighSide.NumNeg != 0)
			&& (seq->HighSide.OnTime == 0 || seq->HighSide.OffTime == 0)) {
		show_message("OnTime or OffTime of HighSide is zero, please try again",
		DEFAUT_MESSAGES_TIMEOUT);
		return 0;  // Trả về 0 nếu lỗi
	}

	if ((seq->LowSide.NumPos != 0 || seq->LowSide.NumNeg != 0)
			&& (seq->LowSide.OnTime == 0 || seq->LowSide.OffTime == 0)) {
		show_message("OnTime or OffTime of LowSide is zero, please try again",
		DEFAUT_MESSAGES_TIMEOUT);
		return 0;
	}

	if (seq->Delay == 0) {
		show_message("Invalid Delay", DEFAUT_MESSAGES_TIMEOUT);
		return 0;
	}

	return 1;
}

static void calculate_totals_and_update() {
	uint16_t total_High_Pulse = 0;
	uint16_t total_Low_Pulse = 0;
	uint8_t index = Running_Paramater.Total_Sequence;
	// Duyệt qua mảng Sequence từ 0 đến index và tính tổng HighPulse và LowPulse
	for (uint8_t i = 0; i < index; i++) {
		total_High_Pulse += Sequence[i].HighSide.NumPos
				+ Sequence[i].LowSide.NumPos;
		total_Low_Pulse += Sequence[i].HighSide.NumNeg
				+ Sequence[i].LowSide.NumNeg;
	}

	Running_Paramater.Total_High_Pulse = total_High_Pulse;
	Running_Paramater.Total_Low_Pulse = total_Low_Pulse;
}

bool ValidateSequence(s_sequence *seq) {
	// Kiểm tra HighSide
	if (seq->HighSide.NumPos > DEFAUT_HS_MAX_PCOUNT
			|| seq->HighSide.NumNeg > DEFAUT_HS_MAX_NCOUNT) {
		return false; // Số lượng xung dương hoặc âm của HighSide vượt quá giới hạn
	}
	if (seq->HighSide.OnTime > DEFAUT_HS_MAX_ONTIME
			|| seq->HighSide.OffTime > DEFAUT_HS_MAX_OFFTIME) {
		return false; // Thời gian OnTime hoặc OffTime của HighSide vượt quá giới hạn
	}
	if (seq->HighSide.Delay > DEFAUT_HS_DELAY_PN) {
		return false; // Thời gian Delay của HighSide vượt quá giới hạn
	}

	// Kiểm tra LowSide
	if (seq->LowSide.NumPos > DEFAUT_LS_MAX_PCOUNT
			|| seq->LowSide.NumNeg > DEFAUT_LS_MAX_NCOUNT) {
		return false; // Số lượng xung dương hoặc âm của LowSide vượt quá giới hạn
	}
	if (seq->LowSide.OnTime > DEFAUT_LS_MAX_ONTIME
			|| seq->LowSide.OffTime > DEFAUT_LS_MAX_OFFTIME) {
		return false; // Thời gian OnTime hoặc OffTime của LowSide vượt quá giới hạn
	}
	if (seq->LowSide.Delay > DEFAUT_LS_DELAY_PN) {
		return false; // Thời gian Delay của LowSide vượt quá giới hạn
	}

	// Kiểm tra cực dương và cực âm
	if (seq->Positive_Pole >= 8 || seq->Negative_Pole >= 8) {
		return false; // Nếu cực dương hoặc cực âm lớn hơn hoặc bằng 8
	}
	if (seq->Positive_Pole == seq->Negative_Pole) {
		return false; // Nếu cực dương và cực âm giống nhau
	}

	// Kiểm tra thời gian Delay của Sequence
	if (seq->Delay > DEFAUT_HLS_DELAY) {
		return false; // Thời gian Delay của Sequence vượt quá giới hạn
	}

	return true; // Tất cả các giá trị đều hợp lệ
}

void simulate_dropdown_change(uint8_t new_value) {
	// Thiết lập giá trị mới cho dropdown
	lv_dropdown_set_selected(ui_Dropdown1, new_value);

	// Gửi sự kiện giả lập (LV_EVENT_VALUE_CHANGED)
	lv_event_send(ui_Dropdown1, LV_EVENT_VALUE_CHANGED, NULL);
}

void create_sequence_buttons(void) {
	// Tạo nút delete
	lv_obj_t *delete_btn = lv_btn_create(ui_TabPage4);
	lv_obj_set_size(delete_btn, 120, 40);  // Kích thước nút
	lv_obj_align_to(delete_btn, ui_TabPage4, LV_ALIGN_CENTER, 165, 30);
	lv_obj_set_style_bg_color(delete_btn, lv_color_white(), LV_PART_MAIN);
	lv_obj_set_style_border_color(delete_btn, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_text_color(delete_btn, lv_color_black(), LV_PART_MAIN);

	// Tạo label cho biểu tượng và tên nút "Delete"
	lv_obj_t *delete_label = lv_label_create(delete_btn);
	lv_label_set_text(delete_label, LV_SYMBOL_TRASH " Delete"); // Kết hợp biểu tượng và tên
	lv_obj_align(delete_label, LV_ALIGN_LEFT_MID, 10, 0);

	lv_obj_add_event_cb(delete_btn, Sequence_Delete, LV_EVENT_CLICKED, NULL);

	// Tạo nút save
	lv_obj_t *save_btn = lv_btn_create(ui_TabPage4);
	lv_obj_set_size(save_btn, 120, 40);  // Kích thước nút
	lv_obj_align_to(save_btn, ui_TabPage4, LV_ALIGN_CENTER, 165, -30);

	lv_obj_set_style_bg_color(save_btn, lv_color_white(), LV_PART_MAIN);
	lv_obj_set_style_border_color(save_btn, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_text_color(save_btn, lv_color_black(), LV_PART_MAIN);

	// Tạo label cho biểu tượng và tên nút "Save"
	lv_obj_t *save_label = lv_label_create(save_btn);
	lv_label_set_text(save_label, LV_SYMBOL_SAVE " Save"); // Kết hợp biểu tượng và tên
	lv_obj_align(save_label, LV_ALIGN_LEFT_MID, 10, 0);

	lv_obj_add_event_cb(save_btn, Sequence_Save, LV_EVENT_CLICKED, NULL);
}
