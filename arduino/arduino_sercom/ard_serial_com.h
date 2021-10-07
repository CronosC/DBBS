#ifndef __sercom_helper__ // avoid duplicate definition
#define __sercom_helper__


// Initializes the arduino for serial communication, baud is the BAUD rate (data transfer rate).
void USART_init(void);

// evaluates received input chars
void eval_input(char input[]);

// sends strings over the serial
void put_str_nl(char str[]);
void put_str(char str[]);

// sends a hex number over the serial
void put_dec(int16_t x);

// sends a char over to stdout
void put_c(unsigned char data );

// sends a char over the serial
void put_buffer_c(unsigned char data );

// receives a single char
unsigned char get_c(void);

// compares to str for equality
int8_t str_equal(char a[], char b[]);

#endif
