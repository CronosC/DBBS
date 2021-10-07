#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "arduino_sercom/ard_serial_com.c"
#include "arduino_sercom/simple_buffer.c"
#include "timer/timer.c"

extern int8_t time;
extern volatile int8_t newline_received;

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
	int16_t data;
	data = ADC_read();
	n--;
	
	while(n>0){

        put_str("      ");
        put_dec_nl(ADC_read());
		int16_t tmp;
		tmp = ADC_read();
		data = (data + tmp) / 2;
		n--;
	}
	
	return data;
}


void MOISTURE_SENSOR_init(void){
    DDRD = (1<<DDD2);
}


void MOISTURE_SENSOR_toggle(void){
    PORTD ^= (1<<PORTD2);
}


// simple test program to print Sensor data
//SETUP:
int __attribute__((OS_main)) main(void) {
    cli();
    USART_init();   // initialize serial
    MOISTURE_SENSOR_init();
    ADC_init();
    put_buffer_c('R');     // signal reset
    put_buffer_c('\n');
    ARDUINO_delay_setup(1,1);
    sei();


//LOOP:
    
    while (1) {
        
            
        
        //put_dec_nl(time);

        
        if(newline_received){
            put_buffer_c('\n');
            char input[33];
            
            get_buffered_string(&receive_buffer, input, strlen(input));

            eval_input(input);



            newline_received = 0;
        }
       if(time > 4){
            put_str_nl("a");
            time = 0;
        } 
         

        /*
        put_c('a');
        put_c('\n');
        put_c('b');
        put_c('\n');
        put_c('c');
        put_c('\n');
        put_c('d');
        put_c('\n');
        put_c('e');
        put_c('\n');
        put_c('f');
        put_c('\n');
        */
    }
}
