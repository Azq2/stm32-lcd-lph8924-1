#pragma once

#include <stm32f10x.h>
#include <lph8924-1-config.h>

#define LPH8924_WIDTH	132
#define LPH8924_HEIGHT	176

#ifndef LPH8924_PIN_VD
	#define	LPH8924_PIN_VD 0
#endif

#ifndef LPH8924_PIN_CS
	#define	LPH8924_PIN_CS 0
#endif

/*
 * Commands ids list
 * */
enum {
	LPH8924_CMD_NOP						= 0x00, 
	LPH8924_CMD_RESET					= 0x01, 
	
	LPH8924_CMD_READ_ID					= 0x04, 
	LPH8924_CMD_READ_STATUS				= 0x09, 
	
	LPH8924_CMD_SLEEP_IN				= 0x10, 
	LPH8924_CMD_SLEEP_OUT				= 0x11, 
	
	LPH8924_CMD_INVERT_ON				= 0x20, 
	LPH8924_CMD_INVERT_OFF				= 0x21, 
	
	LPH8924_CMD_SET_GAMMA_CURVE			= 0x26, 
	
	LPH8924_CMD_DISPLAY_OFF				= 0x28, 
	LPH8924_CMD_DISPLAY_ON				= 0x29, 
	
	LPH8924_CMD_SET_COLUMN_ADDR			= 0x2A, 
	LPH8924_CMD_SET_PAGE_ADDR			= 0x2B, 
	
	LPH8924_CMD_WRITE_MEMORY_START		= 0x2C, 
	LPH8924_CMD_READ_MEMORY_START		= 0x2E, 
	
	LPH8924_SET_ADDRESS_MODE			= 0x36, 
	
	LPH8924_IDLE_OUT					= 0x38, 
	LPH8924_IDLE_IN						= 0x39, 
	
	LPH8924_SET_PIXEL_FORMAT			= 0x3A, 
	
	LPH8924_CMD_WRITE_MEMORY_CONTINUE	= 0x3C, 
	LPH8924_CMD_READ_MEMORY_CONTINUE	= 0x3E, 
};

enum {
	LPH8924_COLOR_8BIT		= 2, 
	LPH8924_COLOR_12BIT		= 3, 
	LPH8924_COLOR_16BIT		= 5, 
	LPH8924_COLOR_24BIT		= 7, 
};

typedef union {
	struct {
		uint8_t ST24:1;
		uint8_t MH:1;
		uint8_t RGB:1;
		uint8_t ML:1;
		uint8_t MV:1;
		uint8_t MX:1;
		uint8_t MY:1;
		uint8_t BOTSON:1;
		uint8_t NORON:1;
		uint8_t SLOUT:1;
		uint8_t PTLON:1;
		uint8_t IDMON:1;
		uint8_t IFPF:3;
		uint8_t ST23:1;
		uint8_t GCS2:1;
		uint8_t TEON:1;
		uint8_t DISON:1;
		uint8_t ST11:1;
		uint8_t ST12:1;
		uint8_t INVON:1;
		uint8_t ST14:1;
		uint8_t VSSON:1;
		uint8_t ST0:1;
		uint8_t DEON:1;
		uint8_t PCKON:1;
		uint8_t VSON:1;
		uint8_t HSON:1;
		uint8_t TELOM:1;
		uint8_t GCS0:1;
		uint8_t GCS1:1;
	};
	uint8_t ST[4];
	uint32_t value;
} __attribute__((aligned(1))) lph8924_status_t;

typedef union {
	struct {
		uint8_t ID1;
		uint8_t ID2;
		uint8_t ID3;
	};
	uint8_t ID[3];
} __attribute__((aligned(1))) lph8924_id_t;

/*
 * Hardware and software LCD initialization
 * */
void lph8924_init();

/*
 * IO API
 * */
void lph8924_read(void *data, uint32_t size);
void lph8924_write(uint8_t is_data, uint8_t b);

inline void lph8924_data(uint8_t b) {
	lph8924_write(1, b);
}

inline void lph8924_cmd(uint8_t b) {
	lph8924_write(0, b);
}

void lph8924_read_begin();
void lph8924_read_end();
uint8_t lph8924_read_byte();

/*
 * Commands
 * */
inline void lph8924_nop() {
	lph8924_cmd(LPH8924_CMD_NOP);
}

void lph8924_read_id(lph8924_id_t *id);
void lph8924_read_status(lph8924_status_t *status);

inline void lph8924_sleep(uint8_t flag) {
	lph8924_cmd(flag ? LPH8924_CMD_SLEEP_IN : LPH8924_CMD_SLEEP_OUT);
}

inline void lph8924_idle(uint8_t flag) {
	lph8924_cmd(flag ? LPH8924_IDLE_IN : LPH8924_IDLE_OUT);
}

inline void lph8924_invert(uint8_t flag) {
	lph8924_cmd(flag ? LPH8924_CMD_INVERT_ON : LPH8924_CMD_INVERT_OFF);
}

inline void lph8924_enable(uint8_t flag) {
	lph8924_cmd(flag ? LPH8924_CMD_DISPLAY_ON : LPH8924_CMD_DISPLAY_OFF);
}

void lph8924_set_color_mode(uint8_t color);
void lph8924_read_ram_start(uint8_t continue_read);

inline void lph8924_write_ram_start(uint8_t continue_write) {
	lph8924_cmd(continue_write ? LPH8924_CMD_WRITE_MEMORY_CONTINUE : LPH8924_CMD_WRITE_MEMORY_START);
}

void lph8924_set_address_mode(uint8_t mx, uint8_t my, uint8_t mv, uint8_t ml, uint8_t bgr, uint8_t mh);
void lph8924_set_ram_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void lph8924_send_rgb24_end();
void lph8924_send_rgb24(uint32_t color);
