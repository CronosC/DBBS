#ifndef __timer__ // avoid duplicate definition
#define __timer__

//Sets the registers for Timer1
void ARDUINO_delay_set_registers_timer1(int cmp);

//calculates the value of the compare match register
void ARDUINO_delay_calculate_cmt(double frq);

//turns the disired interval into the disired frequency in Hz
int ARDUINO_delay_calculate_frequency(double interval);

//starts the setup-process. User can input a desired interval in a specific unit
void ARDUINO_delay_setup(double interval, int unit);

#endif
