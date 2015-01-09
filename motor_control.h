/**
 * motor_control.h
 *
 * Motor control functions
 *
 * @author Tom Malone
 */

#ifndef MonsterMotoShield_h
#define MonsterMotoShield_h

#include <stdint.h>

// Motor variables
#define RIGHT_MOTOR_CONTROL_A		PD0
#define RIGHT_MOTOR_CONTROL_B		PD1
#define RIGHT_MOTOR_PWM				PB3
#define RIGHT_MOTOR_CURRENT_SENSE	PB2
#define LEFT_MOTOR_CONTROL_A		PD2
#define LEFT_MOTOR_CONTROL_B		PD3
#define LEFT_MOTOR_PWM				PB4
#define LEFT_MOTOR_CURRENT_SENSE	PB3
#define MOTOR_DDR					DDRD
#define MOTOR_PORT					PORTD
#define MOTOR_PWM_DDR				DDRB
#define MOTOR_PWM_PORT				PORTB
#define MOTOR_CURRENT_SENSE_DDR		DDRB
#define MOTOR_CURRENT_SENSE_PORT	PORTB
// Direction of rotation variables
#define BRAKEVCC					0
#define CW							1
#define CCW							2
#define BRAKEGND					3
#define CS_THRESHOLD				17

/** Initialization functions **/
void initializePWM(void);
void initializeMotors(void);

/** High-level motor control **/
void goForward(uint8_t speed);
void goReverse(uint8_t speed);
void goHardLeft(uint8_t speed);
void goSoftLeft(uint8_t speed);
void goHardRight(uint8_t speed);
void goSoftRight(uint8_t speed);
void goReverseHardLeft(uint8_t speed);
void goReverseSoftLeft(uint8_t speed);
void goReverseHardRight(uint8_t speed);
void goReverseSoftRight(uint8_t speed);
void pivotLeft(uint8_t speed);
void pivotRight(uint8_t speed);
void stopMotors(void);

/** Low-level motor control **/
// Low-level motor movement control
void motorRightGo(uint8_t direction, uint8_t speed);
void motorLeftGo(uint8_t direction, uint8_t speed);

// Stop motor motion
void motorRightStop(void);
void motorLeftStop(void);

#endif
