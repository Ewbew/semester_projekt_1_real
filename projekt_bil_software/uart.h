#pragma once
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

// Target CPU frequency
#define XTAL 16000000

void init_UART(unsigned long baud_rate, unsigned char data_bits);
unsigned char char_ready();
char read_char();
void send_char(char character);
void send_string(char* string);
void send_integer(int number);