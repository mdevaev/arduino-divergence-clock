#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include "DS1307.h"
#include "Nixie.h"
#include "ClockCtl.h"


#define DEBUG

#define SERIAL_SPEED 115200

#define NIXIE_DIN_PIN 9
#define NIXIE_CLOCK_PIN 10
#define NIXNE_STB_PIN 11

#define SWITCH_PLOAD_PIN 8
#define SWITCH_CLOCK_PIN 7
#define SWITCH_DATA_PIN 6


DS1307 clock;
Nixie nixie(4, NIXIE_DIN_PIN, NIXIE_CLOCK_PIN, NIXNE_STB_PIN);
ClockCtl clock_ctl(SWITCH_PLOAD_PIN, SWITCH_CLOCK_PIN, SWITCH_DATA_PIN);


void setNixieTime(unsigned hour, unsigned minute) {
	static unsigned tick = 1;
	nixie.setSymbol(0, hour / 10, 0);
	nixie.setSymbol(1, hour % 10, (tick ? 1 : 0));
	nixie.setSymbol(2, minute / 10, (!tick ? 10 : 0));
	nixie.setSymbol(3, minute % 10, 0);
	nixie.refresh();
	tick = !tick;
}

void ringer(int count) {
	count /= 20;
	while (count--) {
		analogWrite(A3, 255);
		delay(10);
		analogWrite(A3, 0);
		delay(10);
	}
}

void setup()
{
	Serial.begin(SERIAL_SPEED);
	clock.begin();

	clock.getTime();
	unsigned hour = 99, minute = 99;
	unsigned step = 0, last = 1;
	while (hour >= clock.hour || minute >= clock.minute) {
		setNixieTime(hour, minute);
		delay(10);
		if (step % 3 == 0) {
			analogWrite(A3, last * 255);
			last = !last;
		}
		if (hour >= clock.hour) --hour;
		if (minute >= clock.minute) --minute;
		++step;
	}
	analogWrite(A3, 0);
}

#ifdef DEBUG
void debug_clock(char *tag, unsigned hour, unsigned minute, unsigned second) {
	Serial.print(tag);
	Serial.print(": hour=");
	Serial.print(hour);
	Serial.print("; minute=");
	Serial.print(minute);
	Serial.print("\n");
}
#endif

void loop()
{
	if (Serial.available() >= 8) {
		char buf[8];
		unsigned hour, minute, second;
		Serial.readBytes(buf, 8);
		sscanf(buf, "%u %u %u", &hour, &minute, &second);
		clock.fillByHMS(hour, minute, second);
		clock.setTime();
#ifdef DEBUG
		debug_clock("Clock synchronized", hour, minute, second);
#endif
	}

	clock.getTime();
	clock_ctl.readCtl();

#ifdef DEBUG
	debug_clock("Clock", clock.hour, clock.minute, clock.second);
	debug_clock("Alarm", clock_ctl.alarm_hour, clock_ctl.alarm_minute, 0);
	Serial.print("Alarm.enabled=");
	Serial.print(clock_ctl.alarm_enabled);
	Serial.print("\n");
	Serial.print("\n");
#endif

	setNixieTime(clock.hour, clock.minute);

	if (clock_ctl.alarm_enabled
		&& clock.hour == clock_ctl.alarm_hour
		&& clock.minute == clock_ctl.alarm_minute) {
		ringer(1000);
	} else {
		delay(1000);
	}
	//delay(1000);
}
