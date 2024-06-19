/*
 * Lights.cpp
 *
 * Created: 6/17/2024 10:47:42 PM
 *  Author: ottosejrskildsantess
 */ 

#include "Lights.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define FRONT_INTENSITY 68
#define BACK_INTENSITY 20
#define BRAKE_INTENSITY 97

ISR(TIMER4_OVF_vect){
	OCR3B = (1024*BACK_INTENSITY)/100;
	
	// Stopping the clock
	TCCR4B = 0x00;
	
	// Making sure that the overflow flag is reset:
	TIFR4 = 0b00000001;
	
	// Clearing the timer 4 overflow interrut:
	TIMSK4 = 0x00;	
}


Lights::Lights():on_(false),brake_(false){
	
	DDRE |= 0b00011000;
		
// Light intensity PWM //
	// PWM signalet bliver genereret af Timer 3:
	// Bit 7 & 6: Clearing when upcounting, setting when down counting (compare match, 10); output pin is OC3A (PE, ben 3)
	// Bit 5 & 4: Clearing when upcounting, setting when down counting (compare match, 10); output pin is OC3B (PE, ben 4)
	// Bit 1 & 0: PWM, Phase correct, 10-bit (TOP = 1024)
	TCCR3A = 0b10100011;
	// Bit 2, 1 & 0: Prescaler = 8
	TCCR3B = 0b00000010;
	// With the above settings, the frequency will be ~977.5 Hz
	// OBS: These settings have yet to be tested (17/6 2024, Otto)
	
	// Setting OCR initially to 0, so that the duty cycle is zero (lights off)
	
	// Front lights
	OCR3A =  0;
	
	// Back lights
	OCR3B = 0;
}

void Lights::set_lights(bool on){
	if(on){
		OCR3A = static_cast<int>((1024L * FRONT_INTENSITY) / 100);
		OCR3B = static_cast<int>((1024L * BACK_INTENSITY) / 100);
	} else {
		OCR3A = 0;
		OCR3B = 0;
	}
}


void Lights::activate_brake_state(){
		// Vi initierer en timer, der sørger for at bremselyset er tændt i 0.5 sekunder, uden at bremse resten af programmet
		// Vi bruger timer 4 til dette:
		
		// Sætter overflow interrupt for Timer 4
		TIMSK4 = 0b00000001;
		
		// Beregnet værdi for at få et delay på (næsten) 0.5 sekunder
		TCNT4 = 57724; // Old value: 57724
		
		// Normal mode
		TCCR4A = 0b00000000;
		
		// Bit 2, 1 & 0: Prescaler = 1024 - dette starter timeren
		TCCR4B = 0b00000101;
		
		// Setting the back light intensity to brake state:
		OCR3B = static_cast<int>((1024L * BRAKE_INTENSITY) / 100);
		
		// The intensity will be set back to normal in the ISR for timer 4 overflow		
}
