#ifndef __simple_buffer__
#define __simple_buffer__

#define _int int8_t

struct Buffer; 
typedef struct Buffer buffer;

// empties the buffer
void reset_buffer(buffer *buf);

// writes a char into the buffer
_int write_buffer(buffer *buf, char chr);

// returns the next char in the buffer
char read_buffer(buffer *buf);

// checks if the next char in the buffer is 0x00/the buffer is full
_int buffer_empty(buffer *buf);
_int buffer_full(buffer *buf);

//returns a string from the buffer contents
void get_buffered_string(buffer *buf, char *str, int8_t size);

// for debugging
void print_buffer(buffer *buf);

#endif
