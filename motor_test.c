/**
 *  Establish & verify motor control using ATmega1284p
 *
 *  Currently uses PORTA for motor control in order to preserve
 *  other functionality (e.g. Serial comms, ISP, output compare,
 *  etc.)
 *
 * @author Tom Malone
 * @date January 5, 2015
 */

#include <avr/io.h>
#include <util/delay.h>
#include "motor_control.h"

#define LED         PB0
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define DELAYTIME   5000

int main(void)
{
	// Initialization
	initializePWM();
	initializeMotors();

    // Configure LED pin for ouput in LED Data Direction Register
    LED_DDR |= (1 << LED);

	while (1) {
		motorRightGo(CW, 150);
		motorLeftGo(CW, 75);

        // Turn on LED
        LED_PORT |= (1 << LED);
        _delay_ms(DELAYTIME);

        // Turn off LED
        LED_PORT &= ~(1 << LED);
        _delay_ms(DELAYTIME);
	}
}
