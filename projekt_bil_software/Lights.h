/*
 * Lights.h
 *
 * Created: 6/17/2024 3:45:41 PM
 *  Author: ottosejrskildsantess
 */ 

#pragma once

class Lights{
	private:
		bool on_;
		bool brake_;
	public:
		Lights();
		void set_lights(bool on);
		void activate_brake_state();
	};