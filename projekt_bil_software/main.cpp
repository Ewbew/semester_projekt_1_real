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

// Start variable s�ttes til at v�re false til at starte med; skiftes til true i ISR for INT0, 
// som aktiveres ved et tryk p� en eksterne knap p� bilen
volatile bool start = false;

DrivingControl control;
SoundDriver sound(15);
Motor motor;
Lights lights;

void handle_interrupt() {
	//// Bem�rk: counter_ svarer til refleksbriksignalt�ller, alts� hvor mange refleksbriksignaler der er blevet talt op indtil videre
	
	// DrivingControl objektet t�ller sin member variabel, counter_, op med �n
	control.increment_counter();
	
	// Motor objektet s�ttet hastigheden, som er bestemt af DrivingControl objektet ud fra hvad counter_ er.
	// control.get_speed() returnerer en integer mellem 0 og 100, som svarer til duty cycle
	motor.set_speed(control.get_speed());
	
	// Motor objektet s�tter retningen, som er bestemt af DrivingControl objektet ud fra hvad counter_ er.
	// control.is_forward_direction() returnerer en bool � hvis den er true, s� er retningen fremad
	motor.set_forward_direction(control.is_forward_direction());
	
	// Sound objektet spiller en lyd, som er bestemt af DrivingControl objektet ud fra hvad counter_ er.
	// control.get_counter() returnerer en integer, som er counter af refleksbriksignaler.
	sound.play_sound(control.get_counter() == 11 ? 2 : 3);
	
	// Lights objektet bestemmer tilstanden for- og baglysene, som er bestem af DrivingControl objektet ud fra hvad counter_ er.
	// control.get_lights_state() returnerer en bool � hvis den er true, s� t�ndes for- og baglysene og omvendt for false.
	lights.set_lights(control.get_lights_state());
	
	// control.get_brake_state() returnerer en bool � den returnerer true, hvis hastigheden for nuv�rende refleksbrikcounter er lavere end hastigheden for den tidligere,
	// ELLER hvis der er lavet er retningsskift mellem sidste refleksbrikcounter og den nuv�rende. Ellers returnerer control.get_brake_state().
	// Hvis control.get_brake_state() returnerer true, og refleksbrikcounter er lavere end 11 (som er den sidste refleksbrik), s� aktiveres bremselyset i 0.5 sekunder.
	if(control.get_brake_state() && control.get_counter() < 11) lights.activate_brake_state();
	
	// Vi dissabler de to ISR for refleksbrikkerne kortvarigt, for at v�re sikker p�,
	//at der kun bliver talt op �n gang per reflekspar p� banen:
	EIMSK &= 0b11111001;
	_delay_ms(DEBOUNCE_DELAY_MS); // OBS; denne v�rdi skal findes gennem tests. Pr�vede v�rdier: 150
	
	// For at v�re p� den sikre side, s� nulstiller vi interruptsflagene for INT1 & INT2, i tilf�lde af, at de var blevet sat.
	// Man nulstiller dem ved at skrive 1 til de tilsvarende bit pladser i flag registret:
	if(EIFR != 0){
		EIFR |= 0b00000111;
	}
	
	// Vi enabler de to interrupts for INT1 og INT2 igen
	EIMSK |= 0b00000110;
}

/*

DEPRECATED � vi bruger nu SW7 p� Arduinoen til at s�tte bilen i gang.

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

// Interrupt service rutine for refleksbrik 1 � s�ttes i gang, n�r den modtager et signal
// Se ovenst�ende definition for funktionen handle_interrupt(), for hvad der sker ved interruptet
ISR(INT1_vect){
	handle_interrupt();
}

// Interrupt service rutine for refleksbrik 2 � s�ttes i gang, n�r den modtager et signal
// Se ovenst�ende definition for funktionen handle_interrupt(), for hvad der sker ved interruptet
ISR(INT2_vect){
	handle_interrupt();
}

int main(void)
{
	// Setting the PA pin 7 to be input (is connected with SW7)
	DDRA &= 0b01111111;
	
	// Interrupts input
	DDRD &= 0b11111001;
	
	// Ops�tning af ISR'er; alle s�ttes til at aktivere korresponderende ISR ved falling edge
	EICRA = 0b00101000;
		
	// Vi enabler de tre interrupts
	EIMSK |= 0b00000110;
	
	// Vi venter p�, at der bliver trykket p� SW7 knappen p� arduinoen, f�r at bilen s�tter i gang
	while (PINA & 0b10000000)
	{
		// Vi laver et lille delay, for at v�re helt sikre p�, at der ikke er noget kontaktprel.
		_delay_ms(DEBOUNCE_DELAY_MS);
	}
	
	//// Start rutine, som s�tter bilen i gang
	// Startlyd afspilles
	sound.play_sound(1);
	
	// Lysene t�ndes (siden counter_ fra DrivingControl objektet p� dettet tidspunkt er counter_ = 0
	lights.set_lights(control.get_lights_state());
	
	// Motorhastigheden s�ttes, ud fra hvad counter_ er DrivingControl objektet (ved counter_ = 0 er hastigheden 100)
	motor.set_speed(control.get_speed());
	
	// Enable global interrupt flag:
	sei();

	// Her k�rer vi et loop, som programmet bliver i for evigt � det eneste kode der bliver afviklet, imens programmet k�re i loopet, 
	// er koden der er i de forskellige ISR'er
    while (1) 
    {
		
    }
}

