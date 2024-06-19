#include "SoundDriver.h"

SoundDriver::SoundDriver(unsigned char initVolume) {
	InitUART(9600, 8);
	if (initVolume < 0) initVolume = 0; // Limits to min volume of 0
	if (initVolume > 30) initVolume = 30; // Limits to max volume of 30
	SetVolume(initVolume);
}

void SoundDriver::PlaySound(unsigned char soundIndex) {
	SendChar(0x7E);
	SendChar(0x03);
	SendChar(0x00);
	SendChar(0x00);
	SendChar(soundIndex);
	
	// We are calculating checksum, which is used for detecting errors in the code.
	unsigned int checksum = 0xFFFF - (0x03+soundIndex) + 1;
	unsigned char ck_sum1 = checksum>>8;
	unsigned char ck_sum2 = checksum & 0xFF;
	SendChar(ck_sum1);
	SendChar(ck_sum2);
	SendChar(0xEF);
}

void SoundDriver::SetVolume(unsigned char volume) {
	SendChar(0x7E);
	SendChar(0x06);
	SendChar(0x00);
	SendChar(0x00);
	SendChar(volume);
	
	unsigned int checksum = 0xFFFF - (0x06+volume) + 1;
	unsigned char ck_sum1 = checksum>>8;
	unsigned char ck_sum2 = checksum & 0xFF;
	SendChar(ck_sum1);
	SendChar(ck_sum2);
	SendChar(0xF5);
	SendChar(0xEF);
}