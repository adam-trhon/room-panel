
#include "led_panel.h"
#include "SPI.h"

// TODO remove the preprocessor constants
#define REG_NOP 0x0
#define REG_DIG0 0x1
#define REG_DIG1 0x2
#define REG_DIG2 0x3
#define REG_DIG3 0x4
#define REG_DIG4 0x5
#define REG_DIG5 0x6
#define REG_DIG6 0x7
#define REG_DIG7 0x8
#define REG_DIG(n) (0x01+n)
#define REG_DECODE_MODE 0x9
#define REG_INTENSITY 0xA
#define REG_SCAN_LIMIT 0xB
#define REG_SHUTDOWN 0xC
#define REG_DISPLAY_TEST 0xF

void LedPanel::init(int slaveSelectPin) {
	this->slaveSelectPin = slaveSelectPin;
	hor_offset = 0;
	ver_offset = 0;
	hor_wrap = 0;
	ver_wrap = 0;
	clear();

	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);
	
	SPI.begin();

	SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	setRegForAll(REG_INTENSITY, 1);
	setRegForAll(REG_SCAN_LIMIT, 7);
	setRegForAll(REG_DECODE_MODE, 0);
	setRegForAll(REG_SHUTDOWN, 0);
	SPI.endTransaction();

	delay(10);

	SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	setRegForAll(REG_SHUTDOWN, 1);
	SPI.endTransaction();

	commit();
}


void LedPanel::commit() {
	delay(1);
	SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	for (int row = 0; row < 8; ++row) {
		digitalWrite(slaveSelectPin, LOW);
		for (int panel = 0; panel < 4; ++panel) {
			setReg(REG_DIG(row), values[row][panel]);
		}
		digitalWrite(slaveSelectPin, HIGH);
	}
	SPI.endTransaction();
	delay(1);
}

void LedPanel::clear() {
	memset(values, 0, sizeof(values));
}

void LedPanel::setWrap(bool horizontal, bool vertical) {
	hor_wrap = horizontal;
	ver_wrap = vertical;
}

void LedPanel::setOffset(int8_t horizontal, uint8_t vertical) {
	hor_offset = horizontal;
	ver_offset = vertical;
}

void LedPanel::set(uint8_t col, uint8_t row, bool value) {
	col = col+hor_offset;
	row = row+ver_offset;

	if (hor_wrap) {
		col = col % 32;
	} else if (col >= 32) {
		return;
	}

	if (ver_wrap) {
		row = row % 8;
	} else if (row >= 8) {
		return;
	}
	
	uint8_t i_panel = col/8;
	uint8_t i_bit = 7-col%8;

	if (value) {
		values[row][i_panel] |= 1<<i_bit;
	} else {
		values[row][i_panel] &= ~(1<<i_bit);
	}
}
	
void LedPanel::setColumn(uint8_t column, uint8_t value) {
	for (int i = 0; i < 8; ++i) {
		set(column, i, value&(1<<(7-i)));
	}
}

void LedPanel::setRow(uint8_t row, uint32_t value) {
	const uint32_t mask = 1;
	for (int i = 0; i < 32; ++i) {
		set(i, row, value&(mask<<(31-i)));
	}
}

void LedPanel::drawChar74(char c) {
	switch (c) {
		case 0:
			setColumn(3, 0b00000000);
			setColumn(2, 0b00000000);
			setColumn(1, 0b00000000);
			setColumn(0, 0b00000000);
			break;

		case '1':
			setColumn(3, 0b11111110);
			setColumn(2, 0b01000000);
			setColumn(1, 0b00100000);
			setColumn(0, 0b00000000);
			break;

		case '2':
			setColumn(3, 0b11110010);
			setColumn(2, 0b10010010);
			setColumn(1, 0b10010010);
			setColumn(0, 0b10011110);
			break;

		case '3':
			setColumn(3, 0b11111110);
			setColumn(2, 0b10010010);
			setColumn(1, 0b10010010);
			setColumn(0, 0b10010010);
			break;

		case '4':
			setColumn(3, 0b11111110);
			setColumn(2, 0b00010000);
			setColumn(1, 0b00010000);
			setColumn(0, 0b11110000);
			break;

		case '5':
			setColumn(3, 0b10011110);
			setColumn(2, 0b10010010);
			setColumn(1, 0b10010010);
			setColumn(0, 0b11110010);
			break;

		case '6':
			setColumn(3, 0b10011110);
			setColumn(2, 0b10010010);
			setColumn(1, 0b10010010);
			setColumn(0, 0b11111110);
			break;

		case '7':
			setColumn(3, 0b11100000);
			setColumn(2, 0b10011000);
			setColumn(1, 0b10000110);
			setColumn(0, 0b10000000);
			break;

		case '8':
			setColumn(3, 0b11111110);
			setColumn(2, 0b10010010);
			setColumn(1, 0b10010010);
			setColumn(0, 0b11111110);
			break;

		case '9':
			setColumn(3, 0b11111110);
			setColumn(2, 0b10010000);
			setColumn(1, 0b10010000);
			setColumn(0, 0b11110000);
			break;

		case '0':
			setColumn(3, 0b11111110);
			setColumn(2, 0b10000010);
			setColumn(1, 0b10000010);
			setColumn(0, 0b11111110);
			break;

		case '%':
			setColumn(3, 0b00100110);
			setColumn(2, 0b00010110);
			setColumn(1, 0b11001000);
			setColumn(0, 0b11000100);
			break;

		case '^':
			setColumn(3, 0b00000000);
			setColumn(2, 0b11100000);
			setColumn(1, 0b10100000);
			setColumn(0, 0b11100000);
			break;
	}
}

void LedPanel::drawChar53(char c) {
	switch (c) {
		case 0:
			setColumn(2, 0b00000000);
			setColumn(1, 0b00000000);
			setColumn(0, 0b00000000);
			break;
		case '1':
			setColumn(2, 0b11111000);
			setColumn(1, 0b01000000);
			setColumn(0, 0b00100000);
			break;
		case '2':
			setColumn(2, 0b11101000);
			setColumn(1, 0b10101000);
			setColumn(0, 0b10111000);
			break;
		case '3':
			setColumn(2, 0b11111000);
			setColumn(1, 0b10101000);
			setColumn(0, 0b10101000);
			break;
		case '4':
			setColumn(2, 0b11111000);
			setColumn(1, 0b00100000);
			setColumn(0, 0b11100000);
			break;
		case '5':
			setColumn(2, 0b10111000);
			setColumn(1, 0b10101000);
			setColumn(0, 0b11101000);
			break;
		case '6':
			setColumn(2, 0b10111000);
			setColumn(1, 0b10101000);
			setColumn(0, 0b11111000);
			break;
		case '7':
			setColumn(2, 0b11111000);
			setColumn(1, 0b10000000);
			setColumn(0, 0b10000000);
			break;
		case '8':
			setColumn(2, 0b11111000);
			setColumn(1, 0b10101000);
			setColumn(0, 0b11111000);
			break;
		case '9':
			setColumn(2, 0b11111000);
			setColumn(1, 0b10100000);
			setColumn(0, 0b11100000);
			break;
		case '0':
			setColumn(2, 0b11111000);
			setColumn(1, 0b10001000);
			setColumn(0, 0b11111000);
			break;
		case '%':
			setColumn(2, 0b01001000);
			setColumn(1, 0b00100000);
			setColumn(0, 0b10010000);
			break;
		case '^':
			setColumn(1, 0b11000000);
			setColumn(0, 0b11000000);
			break;
		case ',':
			setColumn(2, 0b00000000);
			setColumn(1, 0b00011000);
			setColumn(0, 0b00000100);
			break;
		case '.':
			setColumn(0, 0b00001000);
			break;
	}
}

void LedPanel::setReg(uint8_t addr, uint8_t data) {
	if (addr < 16) {
		SPI.transfer(addr);
		SPI.transfer(data);
	}
}

void LedPanel::setRegForAll(uint8_t addr, uint8_t data) {
	digitalWrite(slaveSelectPin, LOW);
	for (int i = 0; i < 4; ++i) {
		setReg(addr, data);
	}
	digitalWrite(slaveSelectPin, HIGH);
	delay(1);
}
