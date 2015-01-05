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

int main(void)
{
	// Initialization
	initializeMotors();

	// Main loop
	while (1) {

	}
}
