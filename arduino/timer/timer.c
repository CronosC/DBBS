#include "timer.h"
#include <avr/io.h>
#include <math.h>


void ARDUINO_delay_set_registers_timer1(int cmp){
	cli();
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1  = 0;//initialize counter value to 0
	// set compare match register
	OCR1A = cmp;
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);  
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	sei();
}


void ARDUINO_delay_calculate_cmt(double frq){
	//calculate compare match register for frq * 1Hz
	int cmp = (int)((16*pow(10,6)) / (frq*1024) - 1); //(must be <65536)
	if(cmp>256 && cmp<65536){
		ARDUINO_delay_set_registers_timer1(cmp);
	}
	else if(cmp>0 && cmp<=256){
		//TODO: Use Timer0 or Timer2
	}
	else{
		put_c('E');     //ERROR: Calculation for cmp failed
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

	if(interval > 0){
		double frq = 0;
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
   		ARDUINO_delay_calculate_cmt(frq);
	}
	else{
		put_c('E');     // ERROR: Invalid interval
    	put_c('\n');
    	put_c('2');    
    	put_c('\n');
	}
}


ISR(TIMER1_COMPA_vect){
	put_c('t');
	put_c('\n');
}