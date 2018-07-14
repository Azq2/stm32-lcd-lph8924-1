/*
 * Config example. Change it and rename to: lph8924-1-config.h
 * */
#pragma once

// LCD VDD pin (Need for HW reset)
#define LPH8924_PIN_POWER	GPIO_Pin_1
#define LPH8924_PORT_POWER	GPIOB

// LCD write clock
#define LPH8924_PIN_WR		GPIO_Pin_12
#define LPH8924_PORT_WR		GPIOB

// LCD read clock
#define LPH8924_PIN_RD		GPIO_Pin_13
#define LPH8924_PORT_RD		GPIOB

// LCD hw reset pin
#define LPH8924_PIN_RESET	GPIO_Pin_14
#define LPH8924_PORT_RESET	GPIOB

// LCD data/command selection pin
#define LPH8924_PIN_CD		GPIO_Pin_15
#define LPH8924_PORT_CD		GPIOB

// LCD Tearing PIN (optional, can be permanently connected to the ground)
// Comment, if unsed
#define LPH8924_PIN_VD		GPIO_Pin_3
#define LPH8924_PORT_VD		GPIOB

// LCD chip select pin (optional, can be permanently connected to the ground)
// Comment, if unsed
#define LPH8924_PIN_CS		GPIO_Pin_0
#define LPH8924_PORT_CS		GPIOB

/*
	Support two ways for connect D0-D7
	
	1. Connect to one port and specify shift
		For example, if LPH8924_DATA_SHIFT = 4, then:
		D0 == LPH8924_DATA_SHIFT			connected to GPIO_Pin_4
		D1 == LPH8924_DATA_SHIFT + 1		connected to GPIO_Pin_5
		D2 == LPH8924_DATA_SHIFT + 2		connected to GPIO_Pin_6
		D3 == LPH8924_DATA_SHIFT + 3		connected to GPIO_Pin_7
		D4 == LPH8924_DATA_SHIFT + 4		connected to GPIO_Pin_8
		D5 == LPH8924_DATA_SHIFT + 5		connected to GPIO_Pin_9
		D6 == LPH8924_DATA_SHIFT + 6		connected to GPIO_Pin_10
		D7 == LPH8924_DATA_SHIFT + 7		connected to GPIO_Pin_11
	
	2. Change LPH8924_DATA_SEPARATED_GPIO to "1" and connect data pins for any GPIO ports in any order. This way slower then #1.
*/

#define LPH8924_DATA_SEPARATED_GPIO	0

#if LPH8924_DATA_SEPARATED_GPIO
	// Separated D0-D7 pins (slow)
	
	#define LPH8924_PIN_D0		GPIO_Pin_4
	#define LPH8924_PORT_D0		GPIOB
	
	#define LPH8924_PIN_D1		GPIO_Pin_5
	#define LPH8924_PORT_D1		GPIOB
	
	#define LPH8924_PIN_D2		GPIO_Pin_6
	#define LPH8924_PORT_D2		GPIOB
	
	#define LPH8924_PIN_D3		GPIO_Pin_7
	#define LPH8924_PORT_D3		GPIOB
	
	#define LPH8924_PIN_D4		GPIO_Pin_8
	#define LPH8924_PORT_D4		GPIOB
	
	#define LPH8924_PIN_D5		GPIO_Pin_9
	#define LPH8924_PORT_D5		GPIOB
	
	#define LPH8924_PIN_D6		GPIO_Pin_10
	#define LPH8924_PORT_D6		GPIOB
	
	#define LPH8924_PIN_D7		GPIO_Pin_11
	#define LPH8924_PORT_D7		GPIOB
#else
	// Data on one port, pins ordered (fast)
	
	#define LPH8924_PORT_DATA		GPIOB
	#define LPH8924_DATA_SHIFT		4
#endif
