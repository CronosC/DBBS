#ifndef __clock__ // avoid duplicate definition
#define __clock__


struct Clock;
typedef struct Clock clock;

void get_time(clock *c, char str[]);
void increment_clock(clock *c, int8_t h, int8_t m, int8_t s);
void check_clock(clock *c);

#endif
