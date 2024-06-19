#include "SoundDriver.h"

SoundDriver::SoundDriver(unsigned char init_volume) {
	init_UART(9600, 8);
	if (init_volume < 0) init_volume = 0; // Limits to min volume of 0
	if (init_volume > 30) init_volume = 30; // Limits to max volume of 30
	set_volume(init_volume);
}

void SoundDriver::play_sound(unsigned char sound_index) {
	send_char(0x7E);
	send_char(0x03);
	send_char(0x00);
	send_char(0x00);
	send_char(sound_index);
	
	// We are calculating checksum, which is used for detecting errors in the code.
	unsigned int checksum = 0xFFFF - (0x03+sound_index) + 1;
	unsigned char ck_sum1 = checksum>>8;
	unsigned char ck_sum2 = checksum & 0xFF;
	send_char(ck_sum1);
	send_char(ck_sum2);
	send_char(0xEF);
}

void SoundDriver::set_volume(unsigned char volume) {
	send_char(0x7E);
	send_char(0x06);
	send_char(0x00);
	send_char(0x00);
	send_char(volume);
	
	unsigned int checksum = 0xFFFF - (0x06+volume) + 1;
	unsigned char ck_sum1 = checksum>>8;
	unsigned char ck_sum2 = checksum & 0xFF;
	send_char(ck_sum1);
	send_char(ck_sum2);
	send_char(0xF5);
	send_char(0xEF);
}