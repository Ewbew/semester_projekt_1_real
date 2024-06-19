#include "uart.h"

void init_UART(unsigned long baud_rate, unsigned char data_bits) {
	if ((baud_rate >= 300) && (baud_rate <= 115200) && (data_bits >=5) && (data_bits <= 8)) {
		UCSR0B = 0b00011000;
		UCSR0C = (data_bits - 5) << 1;
		UBRR0 = (XTAL / (16 * baud_rate)) - 1;
	}
}

unsigned char char_ready() {
	if (UCSR0A & 0b10000000) return 1;
	else return 0;
}

char read_char() {
	while (!char_ready()) {}
	return UDR0; 
}

void send_char(char character) {
	while ((UCSR0A & 0b00100000) == 0) {} 
	UDR0 = character;
}

void send_string(char* string) {
	while (*string != 0) {
		send_char(*string);
		string++;
	}
}

void send_integer(int number) {
	char string[32];
	itoa(number, string, 10);
	send_string(string);
}