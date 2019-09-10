#include <atmel_start.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "led_strip.h"
#include "state.h"
#include "temp.h"
#include "term.h"
#include "trevor_touch.h"
#include "simon.h"
#include "led.h"

static char prompt_norml[] = "AND!XOR# ";
#define TEMP_SKIN_LOW_THRESHOLD 125
#define TEMP_SKIN_HIGH_THRESHOLD 145

#define BLING_TICKS_STATE_1 250
#define BLING_TICKS_STATE_2 100
#define BLING_TICKS_STATE_3 15

#define SIMON_TICKS 300
#define ADC_TICKS (SIMON_TICKS * 3)

uint32_t m_ticks = 0;
uint8_t m_bling_state = 0;
uint32_t m_bling_ticks = BLING_TICKS_STATE_1;
#define BLING_MAX_STATE 3

int main(void)
{
	atmel_start_init();				// Initializes MCU, drivers and middleware
	sei();							// Enable interrupts
	SREG |= 0x80;
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	

	printf("\r\n\n\n\n\n");
	printf("=================================\r\n");
	printf("BEGIN TREVOR 2.0\r\n");
	printf("=================================\r\n\n");
	
	led_init();
	
	//Load puzzle state
	state_load();
	
	//Generate a "unique" simon sequence for this player
	simon_generate_sequence();

	while (1)
	{
		term_handler();
		trevor_touch_handler();
		
		//Periodically run the ADC on thermistor
		if ((m_ticks % ADC_TICKS) == 0) {
			ADC_0_start_conversion(2);
		}
		
		//State 0 -> 1 Puzzle : Win A Game of Simon && State 0 Bling
		if (state_get() == 0) {
			if ((m_ticks % SIMON_TICKS) == 0) {
				simon_handler();
			}
			simon_touch_handler();
		}
		//States 1...3 Bling
		else{
			//State 2 -> 3 Puzzle : Coddle Trevor Close
			if (state_get() == 2 && trevor_touch_eye0_get() && trevor_touch_eye1_get() && temp_get() > TEMP_SKIN_LOW_THRESHOLD && temp_get() < TEMP_SKIN_HIGH_THRESHOLD && trevor_touch_hug_get()==false) {
				trevor_touch_hug_set(true);
				state_set(3);
				state_save();
				printf("\r\n ZOMG BEST HUG EVER!\r\n\n");
				printf(prompt_norml);
			}
			
			//Cycle Bling
			if ((m_ticks % m_bling_ticks) == 0) {
				switch(m_bling_state) {
					case 0:
					//LEDS RED
					led_left_set(255,0,0);
					led_right_set(255,0,0);
					break;
					case 1:
					//LEDS GREEN
					led_left_set(0,255,0);
					led_right_set(0,255,0);
					break;
					case 2:
					//LEDS YELLOW
					led_left_set(255,255,0);
					led_right_set(255,255,0);
					break;
					case 3:
					//TODO WHAT GOES HERE?
					break;
				}
				
				//Cycle through LEDs
				m_bling_state = (m_bling_state +1) % BLING_MAX_STATE;
				
				//Determine next update to the LEDs
				if (state_get() == 1)
				m_bling_ticks = BLING_TICKS_STATE_1;
				else if (state_get() == 2)
				m_bling_ticks = BLING_TICKS_STATE_2;
				else if (state_get() == 3)
				m_bling_ticks = BLING_TICKS_STATE_3;
			}
		}
		
		m_ticks++;
		sleep_cpu();
	}
}