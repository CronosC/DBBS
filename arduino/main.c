#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/sleep.h>

#include "arduino_sercom/ard_serial_com.c"
#include "arduino_sercom/simple_buffer.c"
#include "timer/timer.c"
#include "timer/clock.c"
#include "sensors/moisture.c"

extern int8_t time;
extern volatile int8_t newline_received;
extern clock uptime;

// simple test program to print Sensor data
//SETUP:
int __attribute__((OS_main)) main(void) {
    cli();
    USART_init();   // initialize serial
    MOISTURE_SENSOR_init();
    ADC_init();
    ARDUINO_delay_setup(1,1);
    set_sleep_mode(SLEEP_MODE_STANDBY);  // Set sleep mode to STANDBY mode
    sei();
    
    put_str_nl("Setup done..");

//MAIN LOOP:
    while (1) {
    	check_buffer();
       if(time > 1){
            put_str_nl("a");
            increment_clock(&uptime, 0, 0, time);
            time = 0;
        } 
    }
    sleep_enable();
}
