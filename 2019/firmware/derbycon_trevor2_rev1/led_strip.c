#include <atmel_start.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_strip.h"

#define PIN 4

void __led_write_value(uint8_t value) {
	for (uint8_t i=0 ; i<8; i++) {
		if (value & (1 << i)) {
			PORTB_set_pin_level(PIN, 1);
			_delay_us(0.6);
			PORTB_set_pin_level(PIN, 0);
			_delay_us(0.5);
			} else {
			PORTB_set_pin_level(PIN, 1);
			_delay_us(0.1);
			PORTB_set_pin_level(PIN, 0);
			_delay_us(0.1);
		}
	}
}

void led_strip_write(rgb_color * colors, unsigned int count)
{
	PORTB_set_pin_dir(PIN, PORT_DIR_OUT);
	PORTB_set_pin_level(PIN, 0);
	
	cli();   // Disable interrupts temporarily because we don't want our pulse timing to be messed up.
	for (uint8_t i=0; i<count; i++) {
		__led_write_value(colors[i].green);
		__led_write_value(colors[i].red);
		__led_write_value(colors[i].blue);
	}
	
	PORTB_set_pin_level(PIN, 0);
	_delay_us(80);
	sei();          // Re-enable interrupts now that we are done.
}