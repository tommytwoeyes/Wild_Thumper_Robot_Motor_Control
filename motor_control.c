/**
 * motor.c
 *
 * Motor control functions
 *
 * @author Tom Malone
 */

// VNH2SP30 motor controller IC pin definitions
//		e.g. xxxx[0] controls motor channel 1 output
//			 xxxx[1] controls motor channel 2 output
int inApin[2]	= 		{PD0, PD1};
int inBpin[2]	=		{PD2, PD3};
int pwmPin[2]	=		{PD4, PD5};
int csPin[2]	=		{PB2, PB3};

// Initialize motor control pins
void initializeMotors(void)
{
	int outputPin, inputPin;

	// Configure motor control pins for OUTPUT
	for (outputPin=0; outputPin<2; outputPin++) {
		MOTOR_DDR |= (1 << inApin[outputPin]);
		MOTOR_DDR |= (1 << inBpin[outputPin]);
		MOTOR_DDR |= (1 << pwmPin[outputPin]);
	}

	// Configure motor current-sense pins for INPUT
	// (this is not strictly necessary, since all ATmega
	// pins default to the INPUT state, but explicitly
	// setting them as INPUT may help to avoid difficult-
	// to-debug problems later).
	for (inputPin=0; inputPin<2; inputPin++) {
		MOTOR_CURRENT_SENSE_DDR &= ~(1 << csPin[inputPin]);
	}
}

// Move motor forward
void motorGoForward(uint8_t motor, uint8_t pwm)
{

}

// Move motor in reverse
void motorGoReverse(uint8_t motor, uint8_t pwm)
{

}

// Stop motor motion
void motorStop(uint8_t motor)
{

}
