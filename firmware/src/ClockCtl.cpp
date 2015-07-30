#include <Arduino.h>
#include "ClockCtl.h"


ClockCtl::ClockCtl(int pload_pin, int clk_pin, int data_pin) {
	this->pload_pin = pload_pin;
	this->clk_pin = clk_pin;
	this->data_pin = data_pin;

	pinMode(pload_pin, OUTPUT);
	pinMode(clk_pin, OUTPUT);
	pinMode(data_pin, INPUT);

	digitalWrite(clk_pin, LOW);
	digitalWrite(pload_pin, HIGH);
}

void ClockCtl::readCtl() {
	unsigned long ctl = 0;

	digitalWrite(this->pload_pin, LOW);
	delayMicroseconds(5);
	digitalWrite(this->pload_pin, HIGH);

	for (int count = 0; count < 24; ++count) {
		unsigned long bit = digitalRead(this->data_pin);
		ctl |= (bit << (23 - count));
		digitalWrite(this->clk_pin, HIGH);
		delayMicroseconds(5);
		digitalWrite(this->clk_pin, LOW);
	}
	ctl = ~ctl;

	this->alarm_hour = (ctl & 0xfff == 0) ? 0 : log(ctl & 0xfff) / log(2);
	this->alarm_hour += ((ctl >> 18) & 1) ? 12 : 0; // pm/am

	this->alarm_minute = (ctl >> 12) & 0x3f;
	this->alarm_minute = (ctl == 0) ? 0 : log(this->alarm_minute) / log(2);
	this->alarm_minute *= 10;

	this->alarm_enabled = (ctl >> 19) & 1;
}
