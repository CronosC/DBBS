#include "simple_buffer.h"
#include "ard_serial_com.h"
#include <string.h>

buffer receive_buffer;
buffer send_buffer;

volatile int8_t newline_received;

// SETUP
// Initializes the arduino for serial communication, baud is the BAUD rate (data transfer rate).
void USART_init(uint16_t baud_rate) {
	
	if (baud_rate == 9600UL) {
		/*write baud rate*/
		UBRR0H = (unsigned char)(103>>8);
		UBRR0L = (unsigned char)103;
	} else if (baud_rate == 57600UL) {
		/*write baud rate*/
		UBRR0H = (unsigned char)(16>>8);
		UBRR0L = (unsigned char)16;
	} else {

	}
	 	
	/*enable receiver and transmitter*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);
	
	/*frame format 8data, 2stop bit */
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
	
}

// sets the BAUD rate. serial communication must then be restarted to match the new rate.
void set_BAUD(uint16_t baud_rate){
	if (baud_rate == 9600UL) {
		/*write baud rate*/
		UBRR0H = (unsigned char)(103>>8);
		UBRR0L = (unsigned char)103;
	} else if (baud_rate == 57600UL) {
		/*write baud rate*/
		UBRR0H = (unsigned char)(16>>8);
		UBRR0L = (unsigned char)16;
	} else {

	}
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


// evaluates received input chars
void eval_input(char input[]){
	
	// for some reason strcomp returns 0 if equal
	if(str_equal(input, "toggle led\n")){ 
		put_str_nl("Toggling LED..");
		toggle_LED();
	}else if(str_equal(input, "shutdown\n")){ 
		put_str_nl("Shutdown..");
		shutdown();
	}else if(str_equal(input, "set baud 57k\n")){ 
		put_str_nl("BAUD: 57.6k..");
		set_BAUD(57600UL);
	}else if(str_equal(input, "set baud 9k\n")){ 
		put_str_nl("BAUD: 9.6k..");
		set_BAUD(9600UL);
	}else if(str_equal(input, "ping\n")){ 
		put_str_nl("pong");
	}else if(str_equal(input, "ping")){ 
		put_str_nl("pong wo");
	}else if(str_equal(input, "dh11\n")){ 
		read_DH11();
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
		put_c(str[i]);
	}
}

void put_str_nl(char str[]) {
	put_str(str);
	put_c('\n');
}

// sends a hex number over the serial
void put_dec(int16_t x) {
	unsigned char buf[8];

	if (x<0) { // check sign
		put_c('-');
		x = -x;
	}
	if (x==0) { // check for null
		put_c('0');
	} else {
		int i=0;
		while (i<8 && x>0) {
		    buf[i++] = '0' + (x%10);
		    x = x/10;
	}
	i=i-1;
	while (i>= 0) put_c(buf[i--]);
	}
}

// sends a char over the serial
void put_c(unsigned char data ) {
	if(write_buffer(&send_buffer, data)){
		UCSR0B = UCSR0B | (1<<UDRIE0);
	};
}

// RECEIVE
// read a single char
unsigned char get_c(void)
{
	return UDR0;
}


