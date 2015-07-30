#include <Arduino.h>
#include "Nixie.h"

unsigned short Nixie::DIGITS[10] = {SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6, SEG_7, SEG_8, SEG_9};

Nixie::Nixie(int modules, int din_pin, int clk_pin, int stb_pin) {
	this->modules = modules;
	this->din_pin = din_pin;
	this->clk_pin = clk_pin;
	this->stb_pin = stb_pin;

	pinMode(din_pin, OUTPUT);
	pinMode(clk_pin, OUTPUT);
	pinMode(stb_pin, OUTPUT);
}

void Nixie::refresh() {
	unsigned char *ptr = (unsigned char *) this->buf;
	digitalWrite(this->stb_pin, LOW);
	for (int count = 0; count < this->modules * 2; ++count) {
		shiftOut(this->din_pin, this->clk_pin, LSBFIRST, ptr[count]);
	}
	digitalWrite(this->stb_pin, HIGH);
}

void Nixie::setSymbol(unsigned char index, int ch, int dots) {
	if (index >= this->modules) {
		return;
	}

	this->buf[index] |= LEDS_MASK; // set all the LEDs bit to 1,turn off

	this->buf[index] &= ~this->SEGS_MASK; // clear all nixie part
	this->buf[index] = this->DIGITS[ch];

	switch (dots) {
		case 11: this->buf[index] |= this->SEG_RIGHT|this->SEG_LEFT; break;
		case 10: this->buf[index] |= this->SEG_LEFT; break;
		case 1:  this->buf[index] |= this->SEG_RIGHT; break;
	}
}
