#include "moisture.h"


void ADC_init(void){
    //Einschalten des Enable bits für den ADC
    ADCSRA |= (1<<ADEN) | 7;
    //Voltage Reference Selection
    //AVCC with external capacitor at AREF pin
    ADMUX |= (1<<REFS0);
    ADMUX |= (0<<REFS1);
    ADMUX |= (0<<MUX0);
    ADMUX |= (1<<MUX1);
    ADMUX |= (0<<MUX2);
    ADMUX |= (0<<MUX3);

    ACSR = (0<<ACD);
}


int16_t ADC_read(void){
    int16_t data;
    
    //put ADC in conversion mode
    ADCSRA |=  (1<<ADSC); 

    //Check for if ADC is in conversion mode
    while(ADCSRA & (1<<ADSC)){}                              
    data = ADC;
    
    return data;
}


int16_t MOISTURE_SENSOR_get_averaged_reading(int8_t n) {
	MOISTURE_SENSOR_on();
	_delay_ms(1000);
	
	int16_t data;
	data = ADC_read();
	n--;
	
	while(n>0){
		int16_t tmp;
		tmp = ADC_read();
		data = (data + tmp) / 2;
		n--;
	}
	_delay_ms(50);
	MOISTURE_SENSOR_off();
	return data;
}


void MOISTURE_SENSOR_init(void){
    DDRD |= (1<<DDD2);

}


void MOISTURE_SENSOR_toggle(void){
    PORTD ^= (1<<PORTD2);
}

void MOISTURE_SENSOR_on(void){
    PORTD |= (1<<PORTD2);
}
void MOISTURE_SENSOR_off(void){
    PORTD &= ~(1<<PORTD2);
}
