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
	//// Bemærk: counter_ svarer til refleksbriksignaltæller, altså hvor mange refleksbriksignaler der er blevet talt op indtil videre
	
	// DrivingControl objektet tæller sin member variabel, counter_, op med én
	control.increment_counter();
	
	// Motor objektet sættet hastigheden, som er bestemt af DrivingControl objektet ud fra hvad counter_ er.
	// control.get_speed() returnerer en integer mellem 0 og 100, som svarer til duty cycle
	motor.set_speed(control.get_speed());
	
	// Motor objektet sætter retningen, som er bestemt af DrivingControl objektet ud fra hvad counter_ er.
	// control.is_forward_direction() returnerer en bool – hvis den er true, så er retningen fremad
	motor.set_forward_direction(control.is_forward_direction());
	
	// Sound objektet spiller en lyd, som er bestemt af DrivingControl objektet ud fra hvad counter_ er.
	// control.get_counter() returnerer en integer, som er counter af refleksbriksignaler.
	sound.play_sound(control.get_counter() == 11 ? 2 : 3);
	
	// Lights objektet bestemmer tilstanden for- og baglysene, som er bestem af DrivingControl objektet ud fra hvad counter_ er.
	// control.get_lights_state() returnerer en bool – hvis den er true, så tændes for- og baglysene og omvendt for false.
	lights.set_lights(control.get_lights_state());
	
	// control.get_brake_state() returnerer en bool – den returnerer true, hvis hastigheden for nuværende refleksbrikcounter er lavere end hastigheden for den tidligere,
	// ELLER hvis der er lavet er retningsskift mellem sidste refleksbrikcounter og den nuværende. Ellers returnerer control.get_brake_state().
	// Hvis control.get_brake_state() returnerer true, og refleksbrikcounter er lavere end 11 (som er den sidste refleksbrik), så aktiveres bremselyset i 0.5 sekunder.
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

DEPRECATED – vi bruger nu SW7 på Arduinoen til at sætte bilen i gang.

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

// Interrupt service rutine for refleksbrik 1 – sættes i gang, når den modtager et signal
// Se ovenstående definition for funktionen handle_interrupt(), for hvad der sker ved interruptet
ISR(INT1_vect){
	handle_interrupt();
}

// Interrupt service rutine for refleksbrik 2 – sættes i gang, når den modtager et signal
// Se ovenstående definition for funktionen handle_interrupt(), for hvad der sker ved interruptet
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
	
	// Vi venter på, at der bliver trykket på SW7 knappen på arduinoen, før at bilen sætter i gang
	while (PINA & 0b10000000)
	{
		// Vi laver et lille delay, for at være helt sikre på, at der ikke er noget kontaktprel.
		_delay_ms(DEBOUNCE_DELAY_MS);
	}
	
	//// Start rutine, som sætter bilen i gang
	// Startlyd afspilles
	sound.play_sound(1);
	
	// Lysene tændes (siden counter_ fra DrivingControl objektet på dettet tidspunkt er counter_ = 0
	lights.set_lights(control.get_lights_state());
	
	// Motorhastigheden sættes, ud fra hvad counter_ er DrivingControl objektet (ved counter_ = 0 er hastigheden 100)
	motor.set_speed(control.get_speed());
	
	// Enable global interrupt flag:
	sei();

	// Her kører vi et loop, som programmet bliver i for evigt – det eneste kode der bliver afviklet, imens programmet køre i loopet, 
	// er koden der er i de forskellige ISR'er
    while (1) 
    {
		
    }
}

