#include "led_panel.h"

int slaveSelectPin = 10;
LedPanel panel;
uint32_t iteration;

void setup() {
	panel.init(slaveSelectPin);
	panel.setWrap(false, true);
	iteration = 0;
}


void loop() {
	// put your main code here, to run repeatedly:

	panel.clear();

	panel.setOffset(0, 0);
	panel.drawChar53('4');

	panel.setOffset(4, 0);
	panel.drawChar53('2');

	panel.setOffset(8, 0);
	panel.drawChar53('%');

	panel.setOffset(15, 0);
	panel.drawChar53('2');

	panel.setOffset(19, 0);
	panel.drawChar53('2');

	panel.setOffset(23, 0);
	panel.drawChar53(',');

	panel.setOffset(26, 0);
	panel.drawChar53('5');

	panel.setOffset(30, 0);
	panel.drawChar53('^');

	panel.commit();
	iteration++;
	delay(100);
}
