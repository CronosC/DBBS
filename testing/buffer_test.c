#include "simple_buffer.c"
#include <stdio.h>
#include <stdlib.h>

buffer test;

#define w write_buffer(&test, 'x');
#define r read_buffer(&test);
#define e printf("empty: %i\n", buffer_empty(&test));
#define p print_buffer(&test);
#define o reset_buffer(&test);
#define g get_buffered_string(&test, str); printf("Getting str: '%s'\n", str);


char str[16];

int main(int argc, char **argv){

p e p
p w w p e r r p e
p w w w w w w w w w w w w w w w p e g p e p
}
