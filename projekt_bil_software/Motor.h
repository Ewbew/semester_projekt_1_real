/*
 * Motor.h
 *
 * Created: 6/14/2024 3:48:41 PM
 *  Author: ottosejrskildsantess
 */ 

// Nice-to-haves:
// - Do gradual acceleration, and not just instantly set the speed to current speed (e.g. going directly from 50 to 100)

#pragma once
class Motor{
	private:
		int speed_;
		bool forward_direction_;
	public:
		// Should just initialize with base values (speed = 0 and forward_direction = true)
		Motor();
		void set_speed(int speed);
		void set_forward_direction(bool forward_direction);
	};