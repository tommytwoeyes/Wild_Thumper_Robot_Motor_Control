/**
 * motor.h
 *
 * Motor control functions
 *
 * @author Tom Malone
 */

#ifndef MonsterMotoShield_h
#define MonsterMotoShield_h

// Motor variables
#define MOTOR_DDR				DDRD
#define MOTOR_CURRENT_SENSE_DDR	DDRB
#define BRAKEVCC		0
#define CW				1
#define CCW				2
#define BRAKEGND		3
#define CS_THRESHOLD	17

// Initialize motor control pins
void initializeMotors(void);

// Move motor forward
void motorGoForward(uint8_t motor, uint8_t pwm);

// Move motor in reverse
void motorGoReverse(uint8_t motor, uint8_t pwm);

// Stop motor motion
void motorStop(uint8_t motor);

#endif
