#include "SoundDriver.h"

SoundDriver::SoundDriver(unsigned char init_volume) {
	init_UART(9600, 8);
	if (init_volume < 0) init_volume = 0; // Begr�nser minimumsv�rdien af volumen til 0
	if (init_volume > 30) init_volume = 30; // Begr�nser maksimalv�rdien af volumen til 0
	set_volume(init_volume);
}

void SoundDriver::play_sound(unsigned char sound_index) {
	send_char(0x7E);
	send_char(0x03);
	send_char(0x00);
	send_char(0x00);
	send_char(sound_index);
	
	// Vi beregner checksum, som bruges til at fange eventuelle fejl i koden.
	unsigned int checksum = 0xFFFF - (0x03+sound_index) + 1;
	unsigned char ck_sum1 = checksum>>8;
	unsigned char ck_sum2 = checksum & 0xFF;
	send_char(ck_sum1);
	send_char(ck_sum2);
	send_char(0xEF);
}

void SoundDriver::set_volume(unsigned char volume) {
	
	// Vi s�rger for, at hvis volume skrider gr�nsev�rdierne, at de blive sat til valide v�rdier
	// (samme logik somme i klassekonstruktoren).
	if(volume > 30){
		volume = 30;
	} else if (volume < 0){
		volume = 0;
	}
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