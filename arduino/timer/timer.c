#include "timer.h"
#include <avr/io.h>
#include <math.h>

//Global Timer Variable
int8_t time;
int8_t timer_interval;

void ARDUINO_delay_set_registers_timer1(int cmr){
	cli();
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1  = 0;//initialize counter value to 0
	// set compare match register
	OCR1A = cmr;
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);  
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	sei();
}


void ARDUINO_delay_calculate_cmr(double frq){
	//calculate compare match register for frq * 1Hz
	int cmr = (int)((16*pow(10,6)) / (frq*1024) - 1); //(must be <65536)
	if(cmr>256 && cmr<65536){
		ARDUINO_delay_set_registers_timer1(cmr);
	}
	else if(cmr>0 && cmr<=256){
		//TODO: Use Timer0 or Timer2
	}
	else{
		put_c('E');     //ERROR: Calculation of cmr failed
   		put_c('\n');
    	put_c('3');     
    	put_c('\n');
	}
}


double ARDUINO_delay_calculate_frq(double interval){
	//return desired frequency
	return (1/interval);
}


void ARDUINO_delay_setup(double interval, int unit){
	time = 0;
	if(interval > 0){
		double frq = 0;
		timer_interval = interval;
		switch(unit) {
		//Case for ms
		case 0 :
			frq = ARDUINO_delay_calculate_frq(interval/1000);
			break;
		//Case for seconds
		case 1 :
			frq = ARDUINO_delay_calculate_frq(interval);
			break;
		//Case for minutes
		case 2 :
			frq = ARDUINO_delay_calculate_frq(interval*60);
			break;
		//case for hours
		case 3 :
			frq = ARDUINO_delay_calculate_frq(interval*60*60);
			break;
		default :
			put_c('E');     // ERROR: Wrong Time-Unit
    		put_c('\n');
    		put_c('1');     
    		put_c('\n');
   		}
   		ARDUINO_delay_calculate_cmr(frq);
	}
	else{
		put_c('E');     // ERROR: Invalid interval
    	put_c('\n');
    	put_c('2');    
    	put_c('\n');
	}
}


ISR(TIMER1_COMPA_vect){
	time += timer_interval;
}
