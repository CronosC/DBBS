#include "simple_buffer.h"
#include <stdio.h>

#define buf_size 32
#define _int int8_t

struct Buffer{
	char memory[buf_size]; // where the data is saved
	_int current; // the next free position
	_int start; // where we read from last
}; 

// resets the counters in the buffer and makes the next char 0x00
void reset_buffer(buffer *buf){
	buf->current = 0;
	buf->start = 0;
	
	for(_int i = 0; i < buf_size; i++) {
		buf->memory[i] = 0x00;
	}
}

// writes a char into the buffer. If the buffer is full the char is lost. Returns true if the char is a new line symbol and the buffer can be read
_int write_buffer(buffer *buf, char chr){
	if (!buffer_full(buf)){
		buf->memory[buf->current] = chr;
		buf->current = (buf->current + 1) % buf_size;
	}
		
	return ((chr == '\n') || buffer_full(buf));
}

// returns a single char from the buffer and increments the start
char read_buffer(buffer *buf){
	if (buffer_empty(buf)) {
		return 'E';
	} else {
		char data;
		data = buf->memory[buf->start]; // read next char
		buf->memory[buf->start] = 0x00; // delete char
		buf->start = (buf->start + 1) % buf_size; // refer to next start
		return data;
	}
}

// checks whether the buffer is empty
_int buffer_empty(buffer *buf){
	return (buf->memory[buf->start] == 0x00);
}
/*
_int buffer_full(buffer *buf){
	_int current_points_to_last_cell = (buf->start == ((buf->current + 1) % buf_size));
	_int last_cell_has_contents = (buf->memory[buf->current] != 0x00);
	return (current_points_to_last_cell && last_cell_has_contents);
}
*/

_int buffer_full(buffer *buf){
	return ((buf->memory[buf->current]) != 0x00);
}

// returns a string from the buffer. The string ends either at end of buffered chars, the next linebreak or the end of buffer (if it is full)
void get_buffered_string(buffer *buf, char str[], _int size){
	_int i = 0;
	//cli();
	
	// first delete data trash from the string
	for(_int j = 0; j < size; j++) {
		str[j] = 0x00;
	}
	
	// then read the buffer into the string
	while(1) {
		if(!buffer_empty(buf)) {
			str[i] = read_buffer(buf);
		} else {
			break;
		}
		i++;
	}
	//sei();
	
	// fix the string such that it terminates with \n 0x00

	if(str[i - 1] != '\n'){
		str[i] = '\n';
	}
	
} 

// for debugging
void print_buffer(buffer *buf){
	for(_int i = 0; i < buf_size; i++) {
		if (buf->memory[i] != 0x00) {
			printf("| %c ", buf->memory[i]);
		} else {
			printf("|   ");
		}
	}
	
	printf("| ");
	
	if (buffer_empty(buf)) {
		printf("   empty");
	} else if(buffer_full(buf)){
		printf("   full");	
	} else {
		printf("   partially filled");
	}
	
	printf("\n");
	for(int i = 0; i < buf_size; i++) {
		if (i == buf->start && i == buf->current) {
			printf(" s^c");
		}
		else if (i == buf->start) {
			printf(" s^ ");
		}
		else if (i == buf->current) {
			printf(" c^ ");
		}
		else {
			printf("    ");
		}
	}

	printf("\n\n");
}

