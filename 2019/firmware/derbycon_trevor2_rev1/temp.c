#include <atmel_start.h>
#include "temp.h"

static volatile uint8_t m_temp_reading = 0;

//Handle ADC Ready
ISR(ADC1_RESRDY_vect)
{
	adc_result_t res = ADC_0_get_conversion_result();
	m_temp_reading = res;
	//The interrupt flag has to be cleared manually
	ADC0.INTFLAGS = ADC_RESRDY_bm;
}

uint8_t temp_get() {
	return m_temp_reading;
}
