#include "led_panel.h"
#include "sht35.h"
#include <Wire.h>

int slaveSelectPin = 10;
LedPanel panel;
uint32_t iteration;

void setup() {
	panel.init(slaveSelectPin);
	panel.setWrap(false, true);
	iteration = 0;
	Wire.begin();
}

void drawHumidity53(LedPanel& panel, uint8_t offset_hor, uint8_t offset_ver, uint8_t value) {
	if (value > 99) {
		value = 99;
	}

	char c1 = '0' + value/10;
	char c2 = '0' + value%10;

	if (c1 != '0') {
		panel.setOffset(offset_hor+0, offset_ver);
		panel.drawChar53(c1);
	}

	panel.setOffset(offset_hor+4, offset_ver);
	panel.drawChar53(c2);

	panel.setOffset(offset_hor+8, offset_ver);
	panel.drawChar53('%');
}

void drawTemperature53(LedPanel& panel, uint8_t offset_hor, uint8_t offset_ver, int16_t value) {
	if (value < 0) {
		value = 0;
	}
	if (value > 999) { // not that it matters, I am probably dead anyway
		value == 999;
	}

	char c1 = '0' + value/100;
	char c2 = '0' + (value%100)/10;
	char c3 = '0' + value%10;

	panel.setOffset(offset_hor+0, offset_ver);
	panel.drawChar53(c1);

	panel.setOffset(offset_hor+4, offset_ver);
	panel.drawChar53(c2);

	panel.setOffset(offset_hor+8, offset_ver);
	panel.drawChar53(',');

	panel.setOffset(offset_hor+11, offset_ver);
	panel.drawChar53(c3);

	panel.setOffset(offset_hor+15, offset_ver);
	panel.drawChar53('^');
}

void drawTime74(LedPanel& panel, uint8_t offset_hor, uint8_t offset_ver, uint8_t hour, uint8_t minute) {
	if (hour > 99) {
		hour = 99;
	}

	if (minute > 99) {
		minute = 99;
	}

	char c1 = '0' + hour/10;
	char c2 = '0' + hour%10;
	char c3 = '0' + minute/10;
	char c4 = '0' + minute%10;

	panel.setOffset(offset_hor+0, offset_ver);
	panel.drawChar74(c1);

	panel.setOffset(offset_hor+5, offset_ver);
	panel.drawChar74(c2);

	panel.setOffset(offset_hor+10, offset_ver);
	panel.drawChar74(':');

	panel.setOffset(offset_hor+12, offset_ver);
	panel.drawChar74(c3);

	panel.setOffset(offset_hor+17, offset_ver);
	panel.drawChar74(c4);
}



void loop() {
	// put your main code here, to run repeatedly:

	uint32_t humidity;
	uint32_t temperature;

	sht35_measure(&humidity, &temperature);

	panel.clear();

	drawHumidity53(panel, 0, 1, humidity);
	drawTemperature53(panel, 15, 1, temperature);

	panel.commit();
	delay(2000);
}
