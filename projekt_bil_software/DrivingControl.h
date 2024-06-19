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

		// Klassemetoderne tager ikke noget input; de henter blot v�rdien for counter fra klasse instansen
		
		// Returv�rdi bruges til; conditial statements til main
		int get_counter();
		
		// Bruges til at t�lle  counter op gennem ISR 1 og 2
		void increment_counter();

		// Returv�rdi bruges til; Motor
		int get_speed();
	
		// Returv�rdi bruges til; Motor
		bool is_forward_direction();
	
		// Returv�rdi bruges til; Light
		bool get_lights_state();
	
		// Returv�rdi bruges til; Light
		bool get_brake_state();
	
};