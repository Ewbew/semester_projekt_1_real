/*
 * Motor.cpp
 *
 * Created: 6/17/2024 12:23:36 PM
 *  Author: ottosejrskildsantess
 */ 
#include "Motor.h"
#include <avr/io.h>


// Every class object starts with having its member values set to the following initial values:

// Motor klasse objektet initialiseres med en hastighed på 0 og forward_direction som true
Motor::Motor():speed_(0), forward_direction_(true) {
	
	// Motor PWM (pin 11); sættes som udgang
	DDRB = 0b00100000;
	
	// Setting PINC0 to be output, to control the motor direction (pin #37, marked on the Mega Shield 2560)
	DDRC |= 0x01;
	
// SPEED //
	// Motor PWM signalet bliver genereret af Timer 1:
	// Bit 7 & 6: Clearing when upcounting, setting when downcounting (compare match); output pin is OC1A (PB, ben 5
	// Bit 1 & 0: PWM, Phase correct, 10-bit (TOP = 1024)
	TCCR1A = 0b10000011;
	// Bit 2, 1 & 0: Prescaler = 64 (is subject to change – e.g. if set to 8, the frequency will be about 977 Hz)
	TCCR1B = 0b00000011;
	
	// When setting OCR1A to be equal TOP (2^10), then duty cycle is:
	// Duty cycle = 1 - (OCRn/TOP) = 0 (1 - 1 = 0)
	OCR1A = static_cast<int>((1024L/100L)*speed_);	
	

// DIRECTION //	
	// Setting the bit to be 1 (forward_direction = true = 0x01), meaning that the output from PC0 will be 5 V:
	PORTC |= forward_direction_;
}

// Klassemetoden/funktionen sætter hastigheden ved at skrive en værdi til OCR1A, som styrer signalet fra timer 1
void Motor::set_speed(int speed){
	speed_ = speed;
	OCR1A = static_cast<int>((1024L/100L)*speed);
}

// Klassemetoden/funktionen sætter retningen ved at sætte PIN
void Motor::set_forward_direction(bool forward_direction){
	if(forward_direction){ // LSB is being set (the output pin)
		PORTC |= 0b00000001;
	} else { // This just keep all of the other bits in PORTC the same, while the LSB is being cleared (the output pin)
		PORTC &= 0b11111110;
	}
}