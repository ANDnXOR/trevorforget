#include <atmel_start.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include "trevor_touch.h"
#include "util.h"

bool m_eye0 = false;
bool m_eye1 = false;
bool m_hug_check = false;

void trevor_touch_hug_set(bool hugged){
	m_hug_check = hugged;
}

bool trevor_touch_hug_get(){
	return m_hug_check;
}

void trevor_touch_handler() {
	touch_process();
	m_eye0 = get_sensor_state(0) & KEY_TOUCHED_MASK;
	m_eye1 = get_sensor_state(1) & KEY_TOUCHED_MASK;


//  	if(m_eye0 && m_eye1) {
//  		printf("B");
//  		} else if(m_eye0) {
//  		printf("L");
//  		} else if (m_eye1) {
//  		printf("R");
//  	}
	//if (m_eye0)printf("EYE0\r\n");
	//if (m_eye1)printf("EYE1\r\n");
}

bool trevor_touch_eye0_get() {
	return m_eye0;
}

bool trevor_touch_eye1_get() {
	return m_eye1;
}

void trevor_touch_clear() {
	m_eye0 = false;
	m_eye1 = false;
}

bool trevor_touched() {
	return (m_eye0 || m_eye1);
}

bool trevor_touch_eye0_wait(double max_ms) {
	printf("%ld: Waiting for eye0\r\n", trevor_millis_get());
	
	uint32_t end_time_ms = trevor_millis_get() + (uint32_t)max_ms;
	if (trevor_touch_eye0_get()) {
		return true;
	}
	
	while (trevor_millis_get() < end_time_ms) {
		trevor_touch_handler();
		_delay_ms(1);
		if(trevor_touch_eye0_get()) {
			return true;
		}
	}
	
	return false;
}

bool trevor_touch_eye1_wait(double max_ms) {
	printf("%ld: Waiting for eye1\r\n", trevor_millis_get());
	
	uint32_t end_time_ms = trevor_millis_get() + (uint32_t)max_ms;
	if (trevor_touch_eye0_get()) {
		return true;
	}
	
	while (trevor_millis_get() < end_time_ms) {
		trevor_touch_handler();
		_delay_ms(1);
		if(trevor_touch_eye0_get()) {
			return true;
		}
	}
	
	return false;
}

/************************************************************************/
/* Wait for touch from any captouch sensor                              */
/************************************************************************/
bool trevor_touch_wait(double max_ms) {
	printf("%ld Waiting for eyes\r\n", trevor_millis_get());
	
	uint32_t end_time_ms = trevor_millis_get() + (uint32_t)max_ms;
	if (trevor_touch_eye0_get() || trevor_touch_eye1_get()) {
		return true;
	}
	
	while (trevor_millis_get() < end_time_ms) {
		trevor_touch_handler();
		_delay_ms(1);
		if(trevor_touch_eye0_get() || trevor_touch_eye1_get()) {
			return true;
		}
	}
	
	return false;
}

/************************************************************************/
/* Wait for touch events to clear                                       */
/************************************************************************/
void trevor_touch_wait_clear() {
	//printf("Waiting for clear...");
	while(trevor_touched()) {
		trevor_touch_handler();
		_delay_ms(1);
	}
	//printf("CLEAR\r\n");
}