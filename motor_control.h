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
#define MOTOR_DDR					DDRD
#define MOTOR_PORT					PORTD
#define MOTOR_CURRENT_SENSE_DDR		DDRB
#define MOTOR_CURRENT_SENSE_PORT	PORTB
#define BRAKEVCC					0
#define CW							1
#define CCW
#define BRAKEGND					3
#define CS_THRESHOLD				17


/** High-level motor control **/
	void goForward();
	void goReverse();
	void goForwardLeft();
	void goForwardRight();
	void goReverseLeft();
	void goReverseRight();
	void turnInPlaceLeft();
	void turnInPlaceRight();


/** Low-level motor control **/
	// Low-level motor movement control
	void motorGo(uint8_t motor, uint8_t pwm);

	// Stop motor motion
	void motorStop(uint8_t motor);

#endif
