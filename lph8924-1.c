#include "lph8924-1.h"
#include "printf.h"

static uint8_t pixel_mode = LPH8924_COLOR_8BIT;

static uint8_t partial = 0;
static uint8_t partial_bits = 0;

static void lph8924_delay(uint32_t ms);
static void lph8924_set_input_mode(uint8_t flag);

void lph8924_read_id(lph8924_id_t *id) {
	lph8924_cmd(LPH8924_CMD_READ_ID);
	lph8924_read(id, sizeof(lph8924_id_t));
}

void lph8924_read_status(lph8924_status_t *status) {
	lph8924_cmd(LPH8924_CMD_READ_STATUS);
	lph8924_read(status, sizeof(lph8924_status_t));
}

void lph8924_init() {
	lph8924_status_t status;
	
	// Init display pins
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	gpio.GPIO_Pin = LPH8924_PIN_CS;
	GPIO_Init(LPH8924_PORT_CS, &gpio);
	
	gpio.GPIO_Pin = LPH8924_PIN_CD;
	GPIO_Init(LPH8924_PORT_CD, &gpio);
	
	gpio.GPIO_Pin = LPH8924_PIN_RD;
	GPIO_Init(LPH8924_PORT_RD, &gpio);
	
	gpio.GPIO_Pin = LPH8924_PIN_WR;
	GPIO_Init(LPH8924_PORT_WR, &gpio);
	
	gpio.GPIO_Pin = LPH8924_PIN_RESET;
	GPIO_Init(LPH8924_PORT_RESET, &gpio);
	
	gpio.GPIO_Pin = LPH8924_PIN_POWER;
	GPIO_Init(LPH8924_PORT_POWER, &gpio);
	
	#if LPH8924_DATA_SEPARATED_GPIO
		gpio.GPIO_Pin = LPH8924_PIN_D0;
		GPIO_Init(LPH8924_PORT_D0, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D1;
		GPIO_Init(LPH8924_PORT_D1, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D2;
		GPIO_Init(LPH8924_PORT_D2, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D3;
		GPIO_Init(LPH8924_PORT_D3, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D4;
		GPIO_Init(LPH8924_PORT_D4, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D5;
		GPIO_Init(LPH8924_PORT_D5, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D6;
		GPIO_Init(LPH8924_PORT_D6, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D7;
		GPIO_Init(LPH8924_PORT_D7, &gpio);
	#else
		gpio.GPIO_Pin = (0xFF << LPH8924_DATA_SHIFT);
		GPIO_Init(LPH8924_PORT_POWER, &gpio);
	#endif
	
	// Init pin default values
	LPH8924_PORT_CS->ODR &= ~LPH8924_PIN_CS;
	LPH8924_PORT_CD->ODR &= ~LPH8924_PIN_CD;
	LPH8924_PORT_WR->ODR |= LPH8924_PIN_WR;
	LPH8924_PORT_RD->ODR |= LPH8924_PIN_RD;
	LPH8924_PORT_RESET->ODR |= LPH8924_PIN_RESET;
	
	// Power reset
	LPH8924_PORT_POWER->ODR &= ~LPH8924_PIN_POWER;
	lph8924_delay(1);
	LPH8924_PORT_POWER->ODR |= LPH8924_PIN_POWER;
	
	// HW reset
	LPH8924_PORT_RESET->ODR &= ~LPH8924_PIN_RESET;
	lph8924_delay(200);
	LPH8924_PORT_RESET->ODR |= LPH8924_PIN_RESET;
	
	// SW reset
	lph8924_cmd(LPH8924_CMD_RESET);
	do { lph8924_read_status(&status); } while (!status.NORON);
	
	// wakeup
	lph8924_cmd(LPH8924_CMD_SLEEP_OUT);
	do { lph8924_read_status(&status); } while (!status.SLOUT);
	
	// save pixel mode
	pixel_mode = status.IFPF;
}

void lph8924_write(uint8_t is_data, uint8_t b) {
	#if LPH8924_DATA_SEPARATED_GPIO
		LPH8924_PORT_CD->ODR = (LPH8924_PORT_CD->ODR & ~LPH8924_PIN_CD) | (is_data ? LPH8924_PIN_CD : 0);
		LPH8924_PORT_WR->ODR &= ~LPH8924_PIN_WR;
		
		LPH8924_PORT_D0->ODR = (b >> 0) & 1 ? (LPH8924_PORT_D0->ODR | LPH8924_PIN_D0) : (LPH8924_PORT_D0->ODR & ~LPH8924_PIN_D0);
		LPH8924_PORT_D1->ODR = (b >> 1) & 1 ? (LPH8924_PORT_D1->ODR | LPH8924_PIN_D1) : (LPH8924_PORT_D1->ODR & ~LPH8924_PIN_D1);
		LPH8924_PORT_D2->ODR = (b >> 2) & 1 ? (LPH8924_PORT_D2->ODR | LPH8924_PIN_D2) : (LPH8924_PORT_D2->ODR & ~LPH8924_PIN_D2);
		LPH8924_PORT_D3->ODR = (b >> 3) & 1 ? (LPH8924_PORT_D3->ODR | LPH8924_PIN_D3) : (LPH8924_PORT_D3->ODR & ~LPH8924_PIN_D3);
		LPH8924_PORT_D4->ODR = (b >> 4) & 1 ? (LPH8924_PORT_D4->ODR | LPH8924_PIN_D4) : (LPH8924_PORT_D4->ODR & ~LPH8924_PIN_D4);
		LPH8924_PORT_D5->ODR = (b >> 5) & 1 ? (LPH8924_PORT_D5->ODR | LPH8924_PIN_D5) : (LPH8924_PORT_D5->ODR & ~LPH8924_PIN_D5);
		LPH8924_PORT_D6->ODR = (b >> 6) & 1 ? (LPH8924_PORT_D6->ODR | LPH8924_PIN_D6) : (LPH8924_PORT_D6->ODR & ~LPH8924_PIN_D6);
		LPH8924_PORT_D7->ODR = (b >> 7) & 1 ? (LPH8924_PORT_D7->ODR | LPH8924_PIN_D7) : (LPH8924_PORT_D7->ODR & ~LPH8924_PIN_D7);
		
		LPH8924_PORT_WR->ODR |= LPH8924_PIN_WR;
	#else
		if (LPH8924_PORT_DATA == LPH8924_PORT_WR && LPH8924_PORT_DATA == LPH8924_PORT_CD) {
			LPH8924_PORT_DATA->ODR = (LPH8924_PORT_DATA->ODR & ~(LPH8924_PIN_CD | LPH8924_PIN_WR)) | (is_data ? LPH8924_PIN_CD : 0);
			LPH8924_PORT_DATA->ODR = (LPH8924_PORT_DATA->ODR & ~(0xFF << LPH8924_DATA_SHIFT)) | b << LPH8924_DATA_SHIFT | LPH8924_PIN_WR;
		} else {
			LPH8924_PORT_CD->ODR = (LPH8924_PORT_CD->ODR & ~LPH8924_PIN_CD) | (is_data ? LPH8924_PIN_CD : 0);
			LPH8924_PORT_WR->ODR &= ~LPH8924_PIN_WR;
			LPH8924_PORT_DATA->ODR = (LPH8924_PORT_DATA->ODR & ~(0xFF << LPH8924_DATA_SHIFT)) | (b << LPH8924_DATA_SHIFT);
			LPH8924_PORT_WR->ODR |= LPH8924_PIN_WR;
		}
	#endif
}

uint8_t lph8924_read_byte() {
	LPH8924_PORT_RD->ODR &= ~LPH8924_PIN_RD;
	LPH8924_PORT_RD->ODR |= LPH8924_PIN_RD;
	
	#if LPH8924_DATA_SEPARATED_GPIO
		uint8_t result = 0;
		
		result |= (LPH8924_PORT_D0->IDR & LPH8924_PIN_D0) ? 1 << 0 : 0;
		result |= (LPH8924_PORT_D1->IDR & LPH8924_PIN_D1) ? 1 << 1 : 0;
		result |= (LPH8924_PORT_D2->IDR & LPH8924_PIN_D2) ? 1 << 2 : 0;
		result |= (LPH8924_PORT_D3->IDR & LPH8924_PIN_D3) ? 1 << 3 : 0;
		result |= (LPH8924_PORT_D4->IDR & LPH8924_PIN_D4) ? 1 << 4 : 0;
		result |= (LPH8924_PORT_D5->IDR & LPH8924_PIN_D5) ? 1 << 5 : 0;
		result |= (LPH8924_PORT_D6->IDR & LPH8924_PIN_D6) ? 1 << 6 : 0;
		result |= (LPH8924_PORT_D7->IDR & LPH8924_PIN_D7) ? 1 << 7 : 0;
		
		return result;
	#else
		return (LPH8924_PORT_DATA->IDR >> LPH8924_DATA_SHIFT) & 0xFF;
	#endif
}

void lph8924_read(void *data, uint32_t size) {
	lph8924_read_begin();
	uint8_t *p = (uint8_t *) data;
	for (uint32_t i = 0;  i < size + 1; ++i) {
		uint8_t byte = lph8924_read_byte();
		if (i > 0)
			p[i - 1] = byte;
	}
	lph8924_read_end();
}

void lph8924_set_address_mode(uint8_t mx, uint8_t my, uint8_t mv, uint8_t ml, uint8_t bgr, uint8_t mh) {
	lph8924_cmd(LPH8924_SET_ADDRESS_MODE);
	lph8924_data(((mx ? 1 : 0) << 7) | ((my ? 1 : 0) << 6) | ((mv ? 1 : 0) << 5) | ((ml ? 1 : 0) << 4) | ((bgr ? 1 : 0) << 3) | ((ml ? 1 : 0) << 2));
}

void lph8924_set_ram_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	lph8924_cmd(LPH8924_CMD_SET_COLUMN_ADDR);
	lph8924_data(x1);
	lph8924_data(x2);
	
	lph8924_cmd(LPH8924_CMD_SET_PAGE_ADDR);
	lph8924_data(y1);
	lph8924_data(y2);
}

void lph8924_read_ram_start(uint8_t continue_read) {
	lph8924_cmd(continue_read ? LPH8924_CMD_READ_MEMORY_CONTINUE : LPH8924_CMD_READ_MEMORY_START);
	lph8924_read_begin();
	lph8924_read_byte(); // dummy read
}

void lph8924_set_color_mode(uint8_t color) {
	lph8924_cmd(LPH8924_SET_PIXEL_FORMAT);
	lph8924_data(color);
	
	pixel_mode = color;
	partial_bits = 0;
}

void lph8924_send_rgb24_end() {
	if (partial_bits) {
		lph8924_data(partial);
		partial_bits = 0;
	}
}

void lph8924_send_rgb24(uint32_t color) {
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = (color >> 0) & 0xFF;
	
	switch (pixel_mode) {
		// 16M colors, RGB 8-8-8-bits input
		case LPH8924_COLOR_24BIT:
			lph8924_data(r);
			lph8924_data(g);
			lph8924_data(b);
		break;
		
		// 65K colors, RGB 5-6-5-bits input
		case LPH8924_COLOR_16BIT:
			color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
			lph8924_data((color >> 8) & 0xFF);
			lph8924_data(color & 0xFF);
		break;
		
		// 4k colors, RGB 4-4-4-bits input
		case LPH8924_COLOR_12BIT:
			color = ((r & 0xF0) << 4) | (g & 0xF0) | (b >> 4);
			if (partial_bits) {
				lph8924_data(partial | (color >> 8));
				lph8924_data(color & 0xFF);
				partial_bits = 0;
			} else {
				lph8924_data(color >> 4);
				partial = (color & 0x0F) << 4;
				partial_bits = 4;
			}
		break;
		
		// 256 colors, RGB 3-2-3-bits input
		case LPH8924_COLOR_8BIT:
			color = (r & 0xE0) | ((g & 0xC0) >> 3) | (b >> 5);
			lph8924_data(color & 0xFF);
		break;
	}
}

void lph8924_read_begin() {
	LPH8924_PORT_CD->ODR |= LPH8924_PIN_CD;
	lph8924_set_input_mode(1);
}

void lph8924_read_end() {
	lph8924_set_input_mode(0);
}

static void lph8924_delay(uint32_t ms) {
	#ifdef LPH8924_CUSTOM_DELAY
		LPH8924_CUSTOM_DELAY(ms);
	#else
		uint32_t cycles = ((SystemCoreClock / 3000L) * ms) / 3;
		__asm__ volatile (
			"1: \n\t"
			"SUB %0, #1\n\t"
			"CMP %0, #0\n\t"
			"BNE 1b \n\t" : : "r" (cycles) : "memory"
		);
	#endif
}

static void lph8924_set_input_mode(uint8_t flag) {
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = flag ? GPIO_Mode_IPD : GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	#if LPH8924_DATA_SEPARATED_GPIO
		gpio.GPIO_Pin = LPH8924_PIN_D0;
		GPIO_Init(LPH8924_PORT_D0, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D1;
		GPIO_Init(LPH8924_PORT_D1, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D2;
		GPIO_Init(LPH8924_PORT_D2, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D3;
		GPIO_Init(LPH8924_PORT_D3, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D4;
		GPIO_Init(LPH8924_PORT_D4, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D5;
		GPIO_Init(LPH8924_PORT_D5, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D6;
		GPIO_Init(LPH8924_PORT_D6, &gpio);
		
		gpio.GPIO_Pin = LPH8924_PIN_D7;
		GPIO_Init(LPH8924_PORT_D7, &gpio);
	#else
		gpio.GPIO_Pin = (0xFF << LPH8924_DATA_SHIFT);
		GPIO_Init(LPH8924_PORT_DATA, &gpio);
	#endif
}
