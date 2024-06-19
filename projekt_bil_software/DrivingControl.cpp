/*
 * DrivingControl.cpp
 *
 * Created: 6/14/2024 1:45:37 PM
 *  Author: ottosejrskildsantess
 */ 

#include "DrivingControl.h"


const int DrivingControl::SPEED_LEVELS[] = {100, 100, 100, 50, 90, 50, 50, 50, 80, 80, 70, 0};
const int DrivingControl::MAX_COUNTER = sizeof(SPEED_LEVELS) / sizeof(SPEED_LEVELS[0]);


DrivingControl::DrivingControl(): previous_speed_(0), previous_direction_(true) {}


// OBS: Disse værdier skal ændres indtil at bilen kører igennem uden for mange fejl; de bestemmes gennem testkørlser.
// OBS: Det kan være, at bilen bliver nødt til at få sat speed til 0, i stedet for bare at skifte retning.
int DrivingControl::get_speed() {
	if (counter >= 0 && counter < MAX_COUNTER) {
		return SPEED_LEVELS[counter];
		} else {
		return 0;
	}
}

void DrivingControl::increment_counter(){
	counter++;
}

bool DrivingControl::is_forward_direction() {
	return counter < 6 || counter >= 8;
}


// if-statements følger samme logik som get_speed()
bool DrivingControl::get_lights_state() { // They should be on at all times
	return counter < 11; //counter < 6 || (counter >= 7 && counter < 11);
}

// const int DrivingControl::SPEED_LEVELS[] = {100, 90, 65, 50, 90, 50, 50, 50, 80, 80, 50, 0};

// TO - DO: Rewrite code, so we don't decrement and then increment counter_pointer – can result in major errors!
bool DrivingControl::get_brake_state(){
	int current_speed = get_speed();
	bool current_direction = is_forward_direction();
	// Hvis nuværende fart end lavere end tidligere fart, skal der returneres true;
	// Derudover, hvis der er skiftet retning siden sidste refleksbrik, må bilen også skulle bremse/standse
	bool should_brake = current_speed < previous_speed_ || current_direction != previous_direction_;

	previous_speed_ = current_speed;
	previous_direction_ = current_direction;

	return should_brake;
}