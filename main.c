// Evan Widloski - 2017-12-04
// Quadrature Encoder Divider for ATtiny85

/*
    Pinout
    ------
                       ATTiny85
                        ------
                      --|o   |--
    quad. B in  PB3   --|    |--   PB2  quad. B out
    quad. A in  PB4   --|    |--   PB1  quad. A out
                      --|    |--
                        ------

 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// the divisor must evenly divide 256
#define DIVISOR 8

#define PULSEWIDTH (2 * DIVISOR)
#define PERIODWIDTH (2 * PULSEWIDTH)

#define A_OUTPUT (((counter + DIVISOR) % PERIODWIDTH) >= PULSEWIDTH)
#define B_OUTPUT ((counter % PERIODWIDTH) >= PULSEWIDTH)

uint8_t counter = 0;
uint8_t last_PINB = 0;

int main(){

    DDRB = 1<<PB1 | 1<<PB2;        // enable output on PB1, PB2
    GIMSK = 1<<PCIE;               // turn on pin change interrupts
    PCMSK = 1<<PCINT3 | 1<<PCINT4; // unmask PCINT3/4 interrupts

    sei();

    while(1) {}
}

// interrupt for input pin state change
ISR(PCINT0_vect){

    // if last A input != current B input
    if ((PINB>>1 ^ last_PINB) & 1<<PB3){
        counter++;
    }
    else {
        counter--;
    }
    PORTB = (A_OUTPUT<<PB1) | (B_OUTPUT<<PB2);

    last_PINB = PINB;
}
