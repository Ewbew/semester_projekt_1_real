#include "uart.h"

void InitUART(unsigned long baudRate, unsigned char dataBits) {
	if ((baudRate >= 300) && (baudRate <= 115200) && (dataBits >=5) && (dataBits <= 8)) {
		UCSR0B = 0b00011000;
		UCSR0C = (dataBits - 5) << 1;
		UBRR0 = (XTAL / (16 * baudRate)) - 1;
	}
}

unsigned char CharReady() {
	if (UCSR0A & 0b10000000) return 1;
	else return 0;
}

char ReadChar() {
	while (!CharReady()) {}
	return UDR0; 
}

void SendChar(char character) {
	while ((UCSR0A & 0b00100000) == 0) {} 
	UDR0 = character;
}

void SendString(char* string) {
	while (*string != 0) {
		SendChar(*string);
		string++;
	}
}

void SendInteger(int number) {
	char string[32];
	itoa(number, string, 10);
	SendString(string);
}