#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "arduino_sercom/ard_serial_com.c"
#include "arduino_sercom/simple_buffer.c"
#include "timer/timer.c"
#include "sensors/moisture.c"

extern int8_t time;
extern volatile int8_t newline_received;

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


//MAIN LOOP:
    while (1) {
        if(newline_received){
            char input[33];
            
            get_buffered_string(&receive_buffer, input, strlen(input));
            eval_input(input);

            newline_received = 0;
        }
       if(time > 4){
            put_str_nl("a");
            time = 0;
        } 
    }
}
