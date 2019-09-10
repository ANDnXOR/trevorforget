#include <atmel_start.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "state.h"
#include "term.h"

#define DATA_STORE_ADDRESS 0
static uint8_t m_state;

inline uint8_t state_get() {
	return m_state;
}

inline void state_set(uint8_t state) {
	m_state = state;
}

void state_load() {
	m_state = FLASH_0_read_eeprom_byte(DATA_STORE_ADDRESS);
	
	if ((m_state < 0)||(m_state > 3)) { //Check if the data isn't a proper value or never been initialized
		printf(" FUCK: Invalid state value in EEPROM...\r\n\n");
		state_set(0); //Fix the bad state with default value
		state_save(); //Update EEPROM with valid state value
	}
	else {
		//printf(" SUCCESS: Loaded state %d\r\n\n", m_state);
	}
}

void state_save() {
	nvmctrl_status_t status;
	status = FLASH_0_write_eeprom_byte(DATA_STORE_ADDRESS, m_state);
	_delay_ms(500);
	
	if (status != NVM_OK) {
		printf(" FUCK: Error writing to EEPROM...\r\n\n");
	}
}