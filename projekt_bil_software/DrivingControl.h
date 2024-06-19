/*
 * DrivingControl.h
 *
 * Created: 6/14/2024 12:53:05 PM
 *  Author: ottosejrskildsantess
 */ 

#pragma once

class DrivingControl{
	private:
		int previous_speed_;
		bool previous_direction_;
		static const int SPEED_LEVELS[]; 
		static const int MAX_COUNTER;
		int counter_;
	public:

		DrivingControl();

		// Klassemetoderne tager ikke noget input; de henter blot værdien for counter fra klasse instansen
		
		// Returværdi bruges til; conditial statements til main
		int get_counter();
		
		// Bruges til at tælle  counter op gennem ISR 1 og 2
		void increment_counter();

		// Returværdi bruges til; Motor
		int get_speed();
	
		// Returværdi bruges til; Motor
		bool is_forward_direction();
	
		// Returværdi bruges til; Light
		bool get_lights_state();
	
		// Returværdi bruges til; Light
		bool get_brake_state();
	
};