void PUMP_init(void){
    DDRD = (1<<DDD3);
}


void PUMP_toggle(void){
    PORTD ^= (1<<PORTD3);
}