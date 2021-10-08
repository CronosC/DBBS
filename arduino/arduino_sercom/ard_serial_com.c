#include "simple_buffer.h"
#include "ard_serial_com.h"
#include <string.h>
#include <avr/interrupt.h>
#include "../sensors/moisture.h"
#include "../timer/timer.h"
#include "../timer/clock.h"
#include "../pump/pump.h"


buffer receive_buffer;
buffer send_buffer;

volatile int8_t newline_received;
extern clock uptime;

// SETUP
// Initializes the arduino for serial communication, baud is the BAUD rate (data transfer rate).
void USART_init() {
	
	/*write baud rate*/
	UBRR0H = (unsigned char)(16>>8);
	UBRR0L = (unsigned char)16;
	 	
	/*enable receiver and transmitter*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);
	
	/*frame format 8data, 2stop bit */
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);

	/*Enable read on Pin D2*/
	DDRD |= 2;
	
}

ISR(USART_RX_vect){ // received data interrupt
	if(write_buffer(&receive_buffer, UDR0)){ // write returns true in case of \n or full buffer
		newline_received = 1;
	}
}

ISR(USART_TX_vect){ // send data complete interrupt
//	UCSR0B &= ~(1<<UDRIE0); // turn of empty send register interrupt source
}

ISR(USART_UDRE_vect) {
	UDR0 = read_buffer(&send_buffer); // send next char from buffer
	if(buffer_empty(&send_buffer)){
		UCSR0B &= ~(1<<UDRIE0);
	}
}

void check_buffer(void){
        if(newline_received){
            char input[33];
            
            get_buffered_string(&receive_buffer, input, strlen(input));
            eval_input(input);

            newline_received = 0;
        }
}


// evaluates received input chars
void eval_input(char input[]){
	
	// for some reason strcomp returns 0 if equal

	if(str_equal(input, "ping\n")){ 
		put_str_nl("pong");
	}else if(str_equal(input, "moist\n")){
		put_dec_nl(MOISTURE_SENSOR_get_averaged_reading((int8_t)25));
	}else if(str_equal(input, "timer 1\n")){
		ARDUINO_delay_setup(1,1);
	}else if(str_equal(input, "timer 2\n")){
		ARDUINO_delay_setup(2,1);
	}else if(str_equal(input, "uptime\n")){
		char str[10];
		get_time(&uptime, str);
		put_str_nl(str);
	}else if(str_equal(input, "pump\n")){
		PUMP_toggle();
	}else if(str_equal(input, "s\n")){
		MOISTURE_SENSOR_toggle();
	}else if(str_equal(input, "on\n")){
		MOISTURE_SENSOR_on();
	}else if(str_equal(input, "off\n")){
		MOISTURE_SENSOR_off();
	}else {
		put_str("?: ");
		put_str(input);
	}
}

int8_t str_equal(char a[], char b[]) {
	int8_t i = 0;
	int8_t is_equal = 1;
	while(1) {
		if((a[i] != b[i])) {
			is_equal = 0;
		}
		if((a[i] == '\n') || (a[i] == 0x00)) {
			break;
		}
		i++;
	}
	
	return is_equal;
}

// SEND
// sends strings over the serial
void put_str(char str[]) {
	for (int8_t i = 0; i < strlen(str); i++) {
		put_buffer_c(str[i]);
	}
}

void put_str_nl(char str[]) {
	put_str(str);
	put_buffer_c('\n');
}

// sends a hex number over the serial
void put_dec(int16_t x) {
	unsigned char buf[8];

	if (x<0) { // check sign
		put_buffer_c('-');
		x = -x;
	}
	if (x==0) { // check for null
		put_buffer_c('0');
	} else {
		int i=0;
		while (i<8 && x>0) {
		    buf[i++] = '0' + (x%10);
		    x = x/10;
	}
	i=i-1;
	while (i>= 0) put_buffer_c(buf[i--]);
	}
}

// sends a hex number over the serial
void put_dec_nl(int16_t x) {
	put_dec(x);
	put_buffer_c('\n');
}

// sends a char over the serial
void put_buffer_c(unsigned char data ) {
	if(write_buffer(&send_buffer, data)){
		UCSR0B = UCSR0B | (1<<UDRIE0);
	};
}

// output character on serial line
void put_c( unsigned char data ) {
    while ( !( UCSR0A & (1<<UDRE0)) ) {}
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

// RECEIVE
// read a single char
unsigned char get_c(void){
	return UDR0;
}



