/**
 * motor.c
 *
 * Motor control functions for the Monster Moto Shield
 * Borrowed in part from Sparkfun:
 * https://github.com/sparkfun/Monster_Moto_Shield
 *
 * @author Tom Malone
 */

#include <avr/io.h>
#include <math.h>

// VNH2SP30 motor controller IC pin definitions
//		e.g. xxxx[0] controls motor channel 1 output
//			 xxxx[1] controls motor channel 2 output
int inApin[2]	= 		{PD0, PD1};
int inBpin[2]	=		{PD2, PD3};
int pwmPin[2]	=		{PB3, PB4};   // PWM set using OCR0A (PB3) and OCR0B (PB4)
int csPin[2]	=		{PB2, PB3};

void initializePWM(void)
{
	/** Use Phase Correct PWM mode **/

	// Use Inverting mode - output HIGH during count up, LOW during count down
	//		- Bits COM0n1 and COM0n0 in the TCCR0n registers
	//		  (timer/counter control registers) are set to 1
	//		  Note: you have to set COM0A1, COM0A0 and COM0B1, COM0B0
	// 		  in order to use both OC0A and OC0B pwm pins
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0);

	// Use Phase-Correct PWM mode
	// Counter counts up to TOP of 255 (as opposed to user-defined TOP value)
	//		- WGM02 = 0 (in register TCCR0B), WGM01 = 0, WGM00 = 1 (register TCCR0A)
	TCCR0A |= (1 << WGM00);
	TCCR0A &= ~(1 << WGM01); // Not strictly necessary to set to 0 explicitly, but safer
	TCCR0B &= ~(1 << WGM02);

	// Configure clock speed: the Monster Moto shield can only handle
	//		up to 20kHz PWM frequency, so:
	//		use IO clock with no prescaling
	//		F_CPU / Prescaler value / Number of steps
	//		(256 for 8-bit timer, 65536 for 16-bit timer)
	//		1000000 / 1 / 256 = about 3.9kHz
	TCCR0B &= ~(1 << CS02);
	TCCR0B &= ~(1 << CS01);
	TCCR0B |= (1 << CS00);

	// Set PWM pins for output
	MOTOR_PWM_DDR |= (1 << pwmPin[0]) | (1 << pwmPin[2]);
}

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

	// Ensure the motors are braked when initialized
	for (int i=0; i<2; i++) {
		MOTOR_PORT &= ~(1 << inApin[i]);
		MOTOR_PORT &= ~(1 << inBpin[i]);
	}
}


/** High-level motor control **/
	void goForward(speed)
	{
		motorGo(LEFT_MOTOR, CCW, speed);
		motorGo(RIGHT_MOTOR, CW, speed);
	}

	void goReverse(speed)
	{
		motorGo(LEFT_MOTOR, CW, speed);
		motorGo(RIGHT_MOTOR, CCW, speed);
	}

	void goHardLeft(speed)
	{
		motorGo(LEFT_MOTOR, BRAKEGND, 0);
		motorGo(RIGHT_MOTOR, CW, speed);
	}

	void goSoftLeft(speed)
	{
		// Set left motor to half speed so it doesn't turn quite as sharply
		uint8_t leftMotorSpeed = ceil(speed / 2);

		motorGo(LEFT_MOTOR, CCW, leftMotorSpeed);
		motorGo(RIGHT_MOTOR, CW, speed);
	}


	void goHardRight()
	{

	}

	void goSoftRight()
	{

	}


	void goReverseHardLeft(speed)
	{

	}

	void goReverseSoftLeft(speed)
	{

	}


	void goReverseHardRight(speed)
	{

	}

	void goReverseSoftRight(speed)
	{

	}


	void pivoteLeft()
	{

	}


	void pivotRight()
	{

	}


/**
 * Low-level motor control
 * Will power motor in specified direction. Motor will continue to
 * rotate in the specified direction at the specified speed until
 * it's behavior is modified by another function call (to this or
 * another function)
 *
 * @param motor : integer : Should be either 0 or 1. Selects motor to
 * 							 act on.
 * 							 Corresponds to
 * 							 the index of the motor in one of the arrays
 * 							 inApin, inBpin, or pwmPin
 *
 * @param direction : integer : Should be between 0 and 3, with the
 * 								 following meaning:
 * 									0 = Brake to VCC (??? - this was copied from Sparkfun's library. See link in comments at top)
 * 									1 = Clockwise
 * 									2 = CounterClockwise
 * 									3 = Brake to GND (??? - this was copied from Sparkfun's library. See link in comments at top)
 *
 * @param speed : integer : Controls motor PWM duty cycle
 * 						   Should be a value between 0 and 255.
 * 						   Larger numbers = more speed
 * 						   Smaller numbers = less speed
 */
void motorGo(uint8_t motor, uint8_t direction, uint8_t pwm)
{
	if (motor <= 1) {     // Ensure motor ID is valid

		if (direction <= 4) {	// Ensure direction is valid

			// Set inA[motor]
			if (direction <= 1)
				MOTOR_PORT |= (1 << inApin[motor]);
			else
				MOTOR_PORT &= (1 << inApin[motor]);

			// Set inB[motor]
			if ( (direction == 0) || (direction == 2) )
				MOTOR_PORT |= (1 << inBpin[motor]);
			else
				MOTOR_PORT &= ~(1 << inBpin[motor]);


			// Set speed
			if (speed < 0)
				speed = 0;
			if (speed > 255)
				speed = 255;

			if (motor == 0) {
				OCR0A = speed;
			} else {
				OCR0B = speed;
			}
		}
	}
}

// Stop motor motion
void motorStop(uint8_t motor)
{
	int i; // counter

	for (i=0; i<2; i++) {
		MOTOR_PORT &= ~(1 << inApin[i]);
		MOTOR_PORT &= ~(1 << inBpin[i]);
	}

/** TODO **/
	/* Set motor PWM speed to 0 (zero) */
}
