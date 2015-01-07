/**
 * motor_control.c
 *
 * Motor control functions for the Monster Moto Shield
 * Borrowed in part from Sparkfun:
 * https://github.com/sparkfun/Monster_Moto_Shield
 *
 * @author Tom Malone
 */

#include <math.h>
#include <avr/io.h>
#include "motor_control.h"

void initializePWM(void)
{
	// Use Inverting mode - output HIGH during count up, LOW during count down
	//		- Bits COM0n1 and COM0n0 in the TCCR0n registers
	//		  (timer/counter control registers) are set to 1
	//		  Note: you have to set COM0A1, COM0A0 and COM0B1, COM0B0
	// 		  in order to use both OC0A and OC0B pwm pins
	// TCCR0A = 0b11110001
	// TCCR0B = 0b00000010
	TCCR0A |= (1 << COM0A1);
	TCCR0A |= (1 << COM0A0);
	TCCR0A |= (1 << COM0B1);
	TCCR0A |= (1 << COM0B0);

	// Use Phase-Correct PWM mode
	// Counter counts up to TOP, then counts back down to 0 (as opposed
	// to overflowing and restarting at 0, as in Fast PWM mode)
	// TOP is set to 255
	// - WGM02 = 0 (register TCCR0B), WGM01 = 0, WGM00 = 1 (register TCCR0A)
	TCCR0A |= (1 << WGM00);
	TCCR0A &= ~(1 << WGM01); // Not strictly necessary to set to 0 explicitly, but safer
	TCCR0B &= ~(1 << WGM02);

	// According to the datasheet, the Force Output Compare bits in TCCR0B
	// must be set to zero when operating in any PWM mode, to ensure
	// compatibility with future devices.
	// Explicitly set FOC0A and FOC0B to zero, just in case
	TCCR0B &= ~(1 << FOC0A);
	TCCR0B &= ~(1 << FOC0B);

	// Configure clock speed: the Monster Moto shield can only handle
	//		up to 20kHz PWM frequency, so:
	//		use IO clock with no prescaling
	//		F_CPU / Prescaler value / Number of steps
	//		(510 for 8-bit timer in Phase Correct mode (count up/down)
	//		65536 for 16-bit timer)
	//		16000000 / 8 / 510 = about 3.9kHz
	TCCR0B &= ~(1 << CS02);  // Prescaler val of 8:
	TCCR0B |= (1 << CS01);   // CS02 = 0, CS01 = 1
	TCCR0B &= ~(1 << CS00);	 // CS00 = 0
}

// Initialize motor control pins
void initializeMotors(void)
{
	// Configure motor control pins for OUTPUT
	MOTOR_DDR |= (1 << RIGHT_MOTOR_CONTROL_A) | (1 << RIGHT_MOTOR_CONTROL_B);
	MOTOR_DDR |= (1 << LEFT_MOTOR_CONTROL_A) | (1 << LEFT_MOTOR_CONTROL_B);
	MOTOR_PWM_DDR |= (1 << RIGHT_MOTOR_PWM) | (1 << LEFT_MOTOR_PWM);

	// Configure motor current-sense pins for INPUT
	// (this is not strictly necessary, since all ATmega
	// pins default to the INPUT state, but explicitly
	// setting them as INPUT may help to avoid difficult-
	// to-debug problems later).
	MOTOR_CURRENT_SENSE_DDR &= ~(1 << RIGHT_MOTOR_CURRENT_SENSE);
	MOTOR_CURRENT_SENSE_DDR &= ~(1 << LEFT_MOTOR_CURRENT_SENSE);

	// Ensure the motors are braked when initialized
	OCR0A = OCR0B = 0;
	MOTOR_PORT &= ~(1 << RIGHT_MOTOR_CONTROL_A);
	MOTOR_PORT &= ~(1 << RIGHT_MOTOR_CONTROL_B);
	MOTOR_PORT &= ~(1 << LEFT_MOTOR_CONTROL_A);
	MOTOR_PORT &= ~(1 << LEFT_MOTOR_CONTROL_B);
}


/** High-level motor control **/
void goForward(uint8_t speed)
{
	motorLeftGo(CCW, speed);
	motorRightGo(CW, speed);
}

void goReverse(uint8_t speed)
{
	motorLeftGo(CW, speed);
	motorRightGo(CCW, speed);
}

void goHardLeft(uint8_t speed)
{
	motorLeftStop();
	motorRightGo(CW, speed);
}

void goSoftLeft(uint8_t speed)
{
	// Set left motor to half specified speed so it doesn't
	// turn quite as sharply
	uint8_t leftMotorSpeed = ceil(speed / 2);

	motorLeftGo(CCW, leftMotorSpeed);
	motorRightGo(CW, speed);
}


void goHardRight(uint8_t speed)
{
	motorLeftGo(CCW, speed);
	motorRightStop();
}

void goSoftRight(uint8_t speed)
{
	// Set right motor to half specified speed so it doesn't
	// turn quite as sharply
	uint8_t rightMotorSpeed = ceil(speed / 2);

	motorLeftGo(CCW, speed);
	motorRightGo(CW, rightMotorSpeed);
}


void goReverseHardLeft(uint8_t speed)
{
	motorLeftStop();
	motorRightGo(CCW, speed);
}

void goReverseSoftLeft(uint8_t speed)
{
	// Set right motor to half specified speed so it doesn't
	// turn quite as sharply
	uint8_t leftMotorSpeed = ceil(speed / 2);

	motorLeftGo(CW, leftMotorSpeed);
	motorRightGo(CCW, speed);
}


void goReverseHardRight(uint8_t speed)
{
	motorLeftGo(CW, speed);
	motorRightStop();
}

void goReverseSoftRight(uint8_t speed)
{
	uint8_t rightMotorSpeed = ceil(speed / 2);

	motorLeftGo(CCW, speed);
	motorRightGo(CW, rightMotorSpeed);
}


void pivotLeft(uint8_t speed)
{
	motorLeftGo(CW, speed);
	motorRightGo(CW, speed);
}


void pivotRight(uint8_t speed)
{
	motorLeftGo(CCW, speed);
	motorRightGo(CCW, speed);
}

void stopMotors()
{
	motorRightStop();
	motorLeftStop();
}


/** Low-level motor control **/

/**
 * Will power motor in specified direction. Motor will continue to
 * rotate in the specified direction at the specified speed until
 * it's behavior is modified by another function call (to this or
 * another function)
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
void motorRightGo(uint8_t direction, uint8_t speed)
{
	if (direction < 4) {	// Ensure direction is valid

		if (direction <= 1)
			MOTOR_PORT |= (1 << RIGHT_MOTOR_CONTROL_A);
		else
			MOTOR_PORT &= ~(1 << RIGHT_MOTOR_CONTROL_A);

		if ( (direction == 0) || (direction == 2) )
			MOTOR_PORT |= (1 << RIGHT_MOTOR_CONTROL_B);
		else
			MOTOR_PORT &= ~(1 << RIGHT_MOTOR_CONTROL_B);


		// Ensure speed is within bounds
		if (speed < 0)
			speed = 0;
		if (speed > 255)
			speed = 255;

		// Set speed
		OCR0A = speed;
	}
}

/**
 * Will power motor in specified direction. Motor will continue to
 * rotate in the specified direction at the specified speed until
 * it's behavior is modified by another function call (to this or
 * another function)
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
void motorLeftGo(uint8_t direction, uint8_t speed)
{
	if (direction < 4) {	// Ensure direction is valid

		if (direction <= 1)
			MOTOR_PORT |= (1 << LEFT_MOTOR_CONTROL_A);
		else
			MOTOR_PORT &= ~(1 << LEFT_MOTOR_CONTROL_A);

		if ( (direction == 0) || (direction == 2) )
			MOTOR_PORT |= (1 << LEFT_MOTOR_CONTROL_B);
		else
			MOTOR_PORT &= ~(1 << LEFT_MOTOR_CONTROL_B);

		// Ensure speed is within bounds
		if (speed < 0)
			speed = 0;
		if (speed > 255)
			speed = 255;

		// Set speed
		OCR0A = speed;
	}
}

// Stop right motor
void motorRightStop()
{
	MOTOR_PORT &= ~(1 << RIGHT_MOTOR_CONTROL_A);
	MOTOR_PORT &= ~(1 << RIGHT_MOTOR_CONTROL_B);

	// 0% duty cycle == stopped
	OCR0A = 0;
}

// Stop left motor
void motorLeftStop()
{
	MOTOR_PORT &= ~(1 << LEFT_MOTOR_CONTROL_A);
	MOTOR_PORT &= ~(1 << LEFT_MOTOR_CONTROL_B);

	// 0% duty cycle == stopped
	OCR0B = 0;
}
