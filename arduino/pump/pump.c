#include "pump.h"

void PUMP_init(void){
    DDRD |= (1<<DDD3);
}


void PUMP_toggle(void){
    PORTD ^= (1<<PORTD3);
}

void PUMP_on(void){
    PORTD |= (1<<PORTD3);
}
void PUMP_off(void){
    PORTD &= ~(1<<PORTD3);
}
