#include "clock.h"

clock uptime;

struct Clock{
	int8_t hours;
	int8_t minutes; 
	int8_t seconds; 
}; 

void get_time(clock *c, char str[]){
	if(c->seconds > 9 && c->minutes > 9) {
		sprintf(str, "%d:%d:%d", c->hours, c->minutes, c->seconds);
	} else if (c->seconds > 9){
		sprintf(str, "%d:0%d:%d", c->hours, c->minutes, c->seconds);
	} else if (c->minutes > 9){
		sprintf(str, "%d:%d:0%d", c->hours, c->minutes, c->seconds);
	} else {
		sprintf(str, "%d:0%d:0%d", c->hours, c->minutes, c->seconds);
	}
}

void increment_clock(clock *c, int8_t h, int8_t m, int8_t s){
	c->seconds += s;
	c->minutes += m;
	c->hours += h;
	check_clock(c);
}

void check_clock(clock *c){
	c->minutes += c->seconds / 60;
	c->hours += c->minutes / 60;
	
	c->seconds = c->seconds % 60;
	c->minutes = c->minutes % 60;
}


