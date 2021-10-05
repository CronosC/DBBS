#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "arduino_sercom/ard_serial_com.c"
#include "arduino_sercom/simple_buffer.c"
#include "timer/timer.c"

extern int8_t time;

void ADC_init(void){
    //Einschalten des Enable bits für den ADC
    ADCSRA |= (1<<ADEN) | 7;
    //Voltage Reference Selection
    //AVCC with external capacitor at AREF pin
    ADMUX |= (1<<REFS0);
    ADMUX |= (1<<REFS1);
    ADMUX |= (0<<MUX0);
    ADMUX |= (1<<MUX1);
    ADMUX |= (0<<MUX2);
    ADMUX |= (0<<MUX3);

    ACSR = (0<<ACD);
}


void ADC_read(void){

    int16_t data;
    
    //put ADC in conversion mode
    ADCSRA |=  (1<<ADSC); 

    //Check for if ADC is in conversion mode
    while(ADCSRA & (1<<ADSC)){}
                                 

    data = ADC;
    put_dec(data);
    put_c('\n');
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
    USART_init();   // initialize serial
    MOISTURE_SENSOR_init();
    ADC_init();
    put_c('R');     // signal reset
    put_c('\n');
    //ARDUINO_delay_setup(1.0,1);
    sei();


//LOOP:
    
    while (1) {
        //if(time > 5){
            put_str_nl("test");

            MOISTURE_SENSOR_toggle();
            _delay_ms(1500);

            ADC_read();

            _delay_ms(1500);
            MOISTURE_SENSOR_toggle();
            time = 0;
        //}
        
        //put_c('\n');
        //SENSOR_toggle();
        
         if(newline_received){
            put_buffer_c('\n');
            char input[33];
            
            get_buffered_string(&receive_buffer, input, strlen(input));

            eval_input(input);



            newline_received = 0;
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