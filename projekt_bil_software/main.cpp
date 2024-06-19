/*
 * projekt_bil_software.cpp
 *
 * Created: 6/14/2024 11:33:03 AM
 * Author : ottosejrskildsantess
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "DrivingControl.h"
#include "SoundDriver.h"
#include "Motor.h"
#include "Lights.h"

#define DEBOUNCE_DELAY_MS 300

// Start variable sættes til at være false til at starte med; skiftes til true i ISR for INT0, 
// som aktiveres ved et tryk på en eksterne knap på bilen
volatile bool start = false;

DrivingControl control;
SoundDriver sound(15);
Motor motor;
Lights lights;

void handle_interrupt() {
	control.increment_counter();
	motor.set_speed(control.get_speed());
	motor.set_forward_direction(control.is_forward_direction());
	sound.play_sound(control.get_counter() == 11 ? 2 : 3);
	lights.set_lights(control.get_lights_state());
	if(control.get_brake_state() && control.get_counter() < 11) lights.activate_brake_state();
	// Vi dissabler de to ISR for refleksbrikkerne kortvarigt, for at være sikker på,
	//at der kun bliver talt op én gang per reflekspar på banen:
	EIMSK &= 0b11111001;
	_delay_ms(DEBOUNCE_DELAY_MS); // OBS; denne værdi skal findes gennem tests. Prøvede værdier: 150
	
	// For at være på den sikre side, så nulstiller vi interruptsflagene for INT1 & INT2, i tilfælde af, at de var blevet sat.
	// Man nulstiller dem ved at skrive 1 til de tilsvarende bit pladser i flag registret:
	if(EIFR != 0){
		EIFR |= 0b00000111;
	}
	
	// Vi enabler de to interrupts for INT1 og INT2 igen
	EIMSK |= 0b00000110;
}

/*

// Interrupt rutine for start af bil:
ISR(INT0_vect){
	start = true;
	sound.PlaySound(1);
	lights.set_lights(control.get_lights_state());
	motor.set_speed(control.get_speed());
	if(EIFR != 0){
		EIFR |= 0b00000111;
	}
	EIMSK &= 0b11111110;
	reti();
}

*/

ISR(INT1_vect){
	handle_interrupt();
}


ISR(INT2_vect){
	handle_interrupt();
}

int main(void)
{
	// Setting the PA pin 7 to be input (is connected with SW7)
	DDRA &= 0b01111111;
	
	// Interrupts input
	DDRD &= 0b11111001;
	
	// Opsætning af ISR'er; alle sættes til at aktivere korresponderende ISR ved falling edge
	EICRA = 0b00101000;
		
	// Vi enabler de tre interrupts
	EIMSK |= 0b00000110;
	// Enable global interrupt flag:
	
	while (PINA & 0b10000000)
	{
		_delay_ms(DEBOUNCE_DELAY_MS);
	}
	
	sound.play_sound(1);
	lights.set_lights(control.get_lights_state());
	motor.set_speed(control.get_speed());
	sei();


	
    // Her skal vi konstant køre
    while (1) 
    {
		
    }
}

