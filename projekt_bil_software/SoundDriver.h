#pragma once
#include "uart.h"

class SoundDriver {
public:
	SoundDriver (unsigned char init_volume);
	void play_sound(unsigned char sound_index);
	void set_volume(unsigned char volume);
};