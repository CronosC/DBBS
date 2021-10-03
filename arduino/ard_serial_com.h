#ifndef __sercom_helper__ // avoid duplicate definition
#define __sercom_helper__


// Initializes the arduino for serial communication, baud is the BAUD rate (data transfer rate).
void USART_init(uint16_t baud_rate);

// sets the BAUD rate. serial communication must then be restarted to match the new rate.
void set_BAUD(uint16_t baud_rate);

// evaluates received input chars
void eval_input(char input[]);

// sends strings over the serial
void put_str_nl(char str[]);
void put_str(char str[]);

// sends a hex number over the serial
void put_dec(int16_t x);

// sends a char over the serial
void put_c(unsigned char data );

// receives a single char
unsigned char get_c(void);

// compares to str for equality
int8_t str_equal(char a[], char b[]);

#endif
