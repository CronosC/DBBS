#include "../arduino/simple_buffer.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

buffer test;

#define w write_buffer(&test, 'x');
#define r read_buffer(&test);
#define e printf("empty: %i\n", buffer_empty(&test));
#define p print_buffer(&test);
#define o reset_buffer(&test);
#define g get_buffered_string(&test, str, strlen(str)); printf("Getting str: '%s'\n", str);


char str[16];

int main(int argc, char **argv){

e p 
w e p
w w w p
r r r p
w w p g
}
