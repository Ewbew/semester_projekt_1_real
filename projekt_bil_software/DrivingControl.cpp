/*
 * DrivingControl.cpp
 *
 * Created: 6/14/2024 1:45:37 PM
 *  Author: ottosejrskildsantess
 */ 

#include "DrivingControl.h"

// Her bestemmes de forskellige hastigeheder, for det forskellige værdier for counter_ (refleksbriktællerværdier) 
// Der er i alt 11 forskellige værdier, én for hver refleksbrik
const int DrivingControl::SPEED_LEVELS[] = {100, 100, 100, 100, 90, 100, 100, 100, 100, 100, 70, 0};
const int DrivingControl::MAX_COUNTER = sizeof(SPEED_LEVELS) / sizeof(SPEED_LEVELS[0]);


DrivingControl::DrivingControl():counter_(0), previous_speed_(0), previous_direction_(true) {}

int DrivingControl::get_counter(){
	return counter_;
}


// OBS: Disse værdier skal ændres indtil at bilen kører igennem uden for mange fejl; de bestemmes gennem testkørlser.
// OBS: Det kan være, at bilen bliver nødt til at få sat speed til 0, i stedet for bare at skifte retning.
int DrivingControl::get_speed() {
	if (counter_ >= 0 && counter_ < MAX_COUNTER) {
		return SPEED_LEVELS[counter_];
		} else {
		return 0;
	}
}

void DrivingControl::increment_counter(){
	counter_++;
}

bool DrivingControl::is_forward_direction() {
	return counter_ < 6 || counter_ >= 8;
}


// if-statements følger samme logik som get_speed()
bool DrivingControl::get_lights_state() { // They should be on at all times
	return counter_ < 11; //counter_ < 6 || (counter_ >= 7 && counter_ < 11);
}


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