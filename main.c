#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define BAUD 57600    // baud rate for serial connection
#define MPU_6050 0xd0 // I2C address of chip for writing

//
// simple serial communication
//

void USART_init(void) {
    // set baud rate
#include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // enable TxD pin output
    DDRD |= 2;
}

// output character on serial line
void put_c( unsigned char data ) {
    while ( !( UCSR0A & (1<<UDRE0)) ) {}
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

// output decimal number on serial line
void putDec(int16_t x) {
    unsigned char buf[8];
    
    if (x<0) {
        put_c('-');
        x = -x;
    }
    if (x==0) {
        put_c('0');
    } else {
        int i=0;
        while (i<8 && x>0) {
            buf[i++] = '0' + (x%10);
            x = x/10;
        }
        i=i-1;
        while (i>= 0) put_c(buf[i--]);
    }
}

// simple test program to print Sensor data
//SETUP:
int __attribute__((OS_main)) main(void) {
    USART_init();   // initialize serial
    put_c('R');     // signal reset
    put_c('\n');

//LOOP:
    
    while (1) {
        
        
    }
}