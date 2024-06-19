#pragma once
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

// Target CPU frequency
#define XTAL 16000000

void InitUART(unsigned long baudRate, unsigned char dataBits);
unsigned char CharReady();
char ReadChar();
void SendChar(char character);
void SendString(char* string);
void SendInteger(int number);