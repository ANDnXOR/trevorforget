#include <atmel_start.h>
#include "led.h"

uint8_t gamma8[] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
	115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
	144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
	177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
	215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

void led_init() {
	//Ensure PWM pins are outputs
	PORTB.DIRSET = (1<<0)|(1<<1)|(1<<2);
	PORTC.DIRSET = (1<<3)|(1<<4)|(1<<5);
	
	//Enable alternate waveform pins on PORTC
	PORTMUX.CTRLC |= PORTMUX_TCA03_bm | PORTMUX_TCA04_bm | PORTMUX_TCA05_bm;
	
	// Configure timer A0 to generate waveforms
	// In split mode, the type A timer can be used as 2x 8 bit timers, "high" (H) and "low" (L)
	// Each timer has 3 channels, here we will use Ch0 from both timers
	TCA0.SINGLE.CTRLD = TCA_SINGLE_SPLITM_bm;				// enable split mode
	
	//Set period
	TCA0.SPLIT.HPER = 0xFF;
	TCA0.SPLIT.LPER = 0xFF;
	
	//Set duty cycles to 0
	TCA0.SPLIT.HCMP0 = 0;
	TCA0.SPLIT.HCMP1 = 0;
	TCA0.SPLIT.HCMP2 = 0;
	TCA0.SPLIT.LCMP0 = 0;
	TCA0.SPLIT.LCMP1 = 0;
	TCA0.SPLIT.LCMP2 = 0;
	
	//Counters (phase shifting)
	TCA0.SPLIT.HCNT = 0;							// phase = 90 deg. on H timer
	TCA0.SPLIT.LCNT = 0;							// phase = 0 deg. on L timer
	
	//Enable comparator outputs on upper and lower splits both channels
	TCA0.SPLIT.CTRLB =
	TCA_SPLIT_HCMP0EN_bm
	| TCA_SPLIT_HCMP1EN_bm
	| TCA_SPLIT_HCMP2EN_bm
	| TCA_SPLIT_LCMP0EN_bm
	| TCA_SPLIT_LCMP1EN_bm
	| TCA_SPLIT_LCMP2EN_bm;
	
	TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm;					// start timer
		//| TCA_SPLIT_CLKSEL_DIV2_gc; //system clock
}

void led_left_set(uint8_t red, uint8_t green, uint8_t blue) {
	//Color correction and brightness control
	green = (uint8_t)((float)green*0.60f);
	red =  (uint8_t)((float)red*0.90f);
	
	//Red Left
	TCA0.SPLIT.LCMP0 = gamma8[red];
	//Green Left
	TCA0.SPLIT.LCMP2 = gamma8[green];
	//Blue Left
	TCA0.SPLIT.LCMP1 = gamma8[blue];
}

void led_right_set(uint8_t red, uint8_t green, uint8_t blue) {
	//Color correction and brightness control
	green = (uint8_t)((float)green*0.70f);
	red =  (uint8_t)((float)red*0.90f);
	
	//Red Right
	TCA0.SPLIT.HCMP2 = gamma8[red];
	//Green Right
	TCA0.SPLIT.HCMP0 = gamma8[green];
	//Blue Right
	TCA0.SPLIT.HCMP1 = gamma8[blue];
}