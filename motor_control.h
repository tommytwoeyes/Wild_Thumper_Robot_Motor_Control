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
#define LEFT_MOTOR					0
#define RIGHT_MOTOR					1
#define MOTOR_DDR					DDRD
#define MOTOR_PORT					PORTD
#define MOTOR_PWM_DDR				DDRB
#define MOTOR_PWM_PORT				PORTB
#define MOTOR_CURRENT_SENSE_DDR		DDRB
#define MOTOR_CURRENT_SENSE_PORT	PORTB
#define BRAKEVCC					0
#define CW							1
#define CCW                         2
#define BRAKEGND					3
#define CS_THRESHOLD				17

/** Initialization functions **/
void initializePWM(void);
void initializeMotors(void);

/** High-level motor control **/
	void goForward();
	void goReverse();
	void goHardLeft();
	void goSoftLeft();
	void goHardRight();
	void goSoftRight();
	void goReverseLeft();
	void goReverseRight();
	void pivoteLeft();
	void pivotRight();


/** Low-level motor control **/
	// Low-level motor movement control
	void motorGo(uint8_t motor, uint8_t direction, uint8_t pwm);

	// Stop motor motion
	void motorStop(uint8_t motor);

#endif
