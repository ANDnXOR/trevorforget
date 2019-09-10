#include <atmel_start.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include "led.h"
#include "state.h"
#include "trevor_touch.h"
#include "util.h"
#include "temp.h"
#include "term.h"

#define START_LEVEL 1
#define MAX_LEVEL 16
//#define DEBUG_SIMON //Uncomment for simon debug statements

static char prompt_norml[] = "AND!XOR# ";
uint8_t m_level = START_LEVEL; //Player progress, determines how long the sequence is
uint8_t sequence[MAX_LEVEL];
uint8_t m_sequence_index = START_LEVEL; //Start the sequence at the end
uint8_t m_player_sequence_index = 0;

void simon_level_reset(){
	m_level = START_LEVEL;
}

static void __correct() {
	m_player_sequence_index++;
	if(term_mode_get() == 1){
		printf("\r\n ...Correct, index now %d of %d\r\n\n", m_player_sequence_index, m_level);
		printf(prompt_norml);
	}
	
	//They beat the sequence advance the level
	if (m_player_sequence_index >= m_level) {
		if (m_level < MAX_LEVEL) {
			m_level++;
			if(term_mode_get() == 1){
				printf("\r\n *Level advanced, new level: %d\r\n\n", m_level);
				printf(prompt_norml);
			}
			m_player_sequence_index = 0;
			m_sequence_index = m_level;
			for (uint8_t i=0; i<16; i++) {
				led_left_set(255,255,255);
				led_right_set(0,0,0);
				_delay_ms(50);
				led_left_set(0,0,0);
				led_right_set(255,255,255);
				_delay_ms(50);
			}
			led_left_set(0,0,0);
			led_right_set(0,0,0);
		}
		//Excelsior they beat the last level
		else {
			state_set(1);
			state_save();
			printf("\r\n All my life's memories, flashed before my eyes...As 1's and 0's!\r\n\n");
			printf(prompt_norml);
			for (uint8_t i=0; i<30; i++) {
				led_left_set(0, 255, 0);
				led_right_set(255, 0, 0);
				_delay_ms(100);
				led_left_set(255, 0, 0);
				led_right_set(0, 255, 0);
				_delay_ms(100);
			}
		}
	}
}

/************************************************************************/
/* Helper function to handle failure by starting the current level over */
/************************************************************************/
static void __fail() {
	m_player_sequence_index = 0;
	m_sequence_index = m_level;
	led_left_set(255, 0, 0);
	led_right_set(255,0,0);
	_delay_ms(1000);
	if(term_mode_get() == 1){
		printf("\r\n ...WRONG! Try again\r\n\n");
		printf(prompt_norml);
	}
}

/************************************************************************/
/* Handler that runs every tick of trevor                               */
/************************************************************************/
void simon_handler() {
	//If we're at the end of the sequence indicate start of sequence
	if (m_sequence_index >= m_level) {
		//printf("%ld: START OF SEQUENCE\r\n", trevor_millis_get());
		
		//Indicate start of sequence
		led_left_set(0,255,0);
		led_right_set(0,255,0);
		
		//Start of sequence, reset the player
		m_player_sequence_index = 0;
		m_sequence_index = 0;
		} else {
		//printf("%d: %d ", m_sequence_index, sequence[m_sequence_index]);
		if (sequence[m_sequence_index] == 0) {
			//indicate right
			led_left_set(0, 0, 0);
			led_right_set(255,0,0);
			_delay_ms(400);
			led_right_set(0,0,0);
			} else {
			//indicate left
			led_left_set(0, 255, 0);
			led_right_set(0,0,0);
			_delay_ms(400);
			led_left_set(0,0,0);
			
		}
		m_sequence_index++;
	}
}

/************************************************************************/
/* Handler for simon touch controls                                     */
/************************************************************************/
void simon_touch_handler() {
	//Read user input and advance if necessary
	if (trevor_touched()) {
		//Right Pressed
		if (trevor_touch_eye0_get()) {
			//led_left_set(0, 0, 0); //Commented out because you cant even see the feedback blink with sequence playing
			//led_right_set(255,0,0);   //Commented out because you cant even see the feedback blink with sequence playing
			#ifdef DEBUG_SIMON
			printf(" LEFT ");
			#endif
			if (sequence[m_player_sequence_index] == 0) {
				__correct();
				} else {
				__fail();
			}
		}
		//Left pressed
		if (trevor_touch_eye1_get()) {
			//led_left_set(0, 255, 0);  //Commented out because you cant even see the feedback blink with sequence playing
			//led_right_set(0,0,0);  //Commented out because you cant even see the feedback blink with sequence playing
			#ifdef DEBUG_SIMON
			printf(" RIGHT ");
			#endif
			if (sequence[m_player_sequence_index] == 1) {
				__correct();
				} else {
				__fail();
			}
		}

		//Give some feedback to user
		_delay_ms(100);
		//Wait for them to release
		trevor_touch_wait_clear();
		//clear
		//led_left_set(0, 0, 0);  //Commented out because you cant even see the feedback blink with sequence playing
		//led_right_set(0,0,0);   //Commented out because you cant even see the feedback blink with sequence playing
	}

}

void simon_generate_sequence() {
	//TODO Uncomment this and provide an ADC seed before production, leave out now for testing
	//Only call this once!
	srand(temp_get());

	#ifdef DEBUG_SIMON
	printf(" GENERATED SEQUENCE: ");
	#endif
	for (int i = 0; i < MAX_LEVEL; i++)	{
		sequence[i] = rand() % 2;
		#ifdef DEBUG_SIMON
		printf("%d ", sequence[i]);
		#endif
	}
	#ifdef DEBUG_SIMON
	printf("\r\n\n");
	#endif
}