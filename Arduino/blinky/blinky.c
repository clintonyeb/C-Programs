#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRD |= (1 << DDD0);

	for (;;) {
        PORTD |= (1 << PORTD7);
	    _delay_ms(1000);
        PORTD &= (~(1 << PORTD7));
        _delay_ms(100);
	}
	return 0;
}