#pragma once
#include "uart.h"

class SoundDriver {
public:
	SoundDriver (unsigned char initVolume);
	void PlaySound(unsigned char soundIndex);
	void SetVolume(unsigned char volume);
};