#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/sleep.h>
#include <string.h>
#include <avr/interrupt.h>

#include "arduino_sercom/simple_buffer.c"
#include "arduino_sercom/ard_serial_com.c"
#include "timer/timer.c"
#include "timer/clock.c"
#include "sensors/moisture.c"
#include "pump/pump.c"

extern uint16_t time;
extern volatile int8_t newline_received;
extern clock uptime;


//SETUP:
int __attribute__((OS_main)) main(void) {
    cli();
    USART_init(57600UL);   // initialize serial
    MOISTURE_SENSOR_init();
    PUMP_init();
    ADC_init();
    ARDUINO_delay_setup(1,1);
    set_sleep_mode(SLEEP_MODE_STANDBY);  // Set sleep mode to STANDBY mode
    sei();
    
    put_str_nl("Setup done..");

//MAIN LOOP:
    while (1) {
    	check_buffer();
        
       if(time >= 1200){
            increment_clock(&uptime, 0, 0, time);
            
            
            while( (MOISTURE_SENSOR_get_averaged_reading(10) < 150) ){
                if(MOISTURE_SENSOR_get_averaged_reading(10) > 200){
                    PUMP_off();
                    break;
                }

                PUMP_on();
            }
                //lieber mal sicher gehen
                PUMP_off();
                put_str_nl("pump off");
            
            time = 0;  
        } 
    }
    sleep_enable();
}
