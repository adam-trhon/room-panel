#include <SPI.h>

const int slaveSelectPin = 10;

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

void setReg(uint8_t addr, uint8_t data) {
	if (addr < 16) {
		SPI.transfer(addr);
		SPI.transfer(data);
	}
}

void setRegForAll(uint8_t addr, uint8_t data) {
	digitalWrite(slaveSelectPin, LOW);
	for (int i = 0; i < 4; ++i) {
		setReg(addr, data);
	}
	digitalWrite(slaveSelectPin, HIGH);
	delay(1);
}

uint8_t values[8][4] = {0};
uint8_t translation = 0;

void clearAll() {
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 4; ++j)
			values[i][j] = 0;
}

void commit() {
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

void set(uint8_t col, uint8_t row, bool value) {
	col = (col+translation) % 32;
	row = row % 8;
	
	uint8_t i_panel = col/8;
	uint8_t i_bit = 7-col%8;

	if (value) {
		values[row][i_panel] |= 1<<i_bit;
	} else {
		values[row][i_panel] &= ~(1<<i_bit);
	}
	
}

void setColumn(uint8_t column, uint8_t value) {
	for (int i = 0; i < 8; ++i) {
		set(column, i, value&(1<<(7-i)));
	}
}

void font74(uint8_t *points, uint8_t count) {
	for (int i = 0; i < count; ++i) {
		set(points[i]%4, points[i]/4, 1);
	}
}

void drawChar(char c) {
	switch (c) {
		case 0:
			setColumn(3, B00000000);
			setColumn(2, B00000000);
			setColumn(1, B00000000);
			setColumn(0, B00000000);
			break;

		case '1':
			setColumn(3, B11111110);
			setColumn(2, B01000000);
			setColumn(1, B00100000);
			setColumn(0, B00000000);
			break;

		case '2':
			setColumn(3, B11110010);
			setColumn(2, B10010010);
			setColumn(1, B10010010);
			setColumn(0, B10011110);
			break;

		case '3':
			setColumn(3, B11111110);
			setColumn(2, B10010010);
			setColumn(1, B10010010);
			setColumn(0, B10010010);
			break;

		case '4':
			setColumn(3, B11111110);
			setColumn(2, B00010000);
			setColumn(1, B00010000);
			setColumn(0, B11110000);
			break;

		case '5':
			setColumn(3, B10011110);
			setColumn(2, B10010010);
			setColumn(1, B10010010);
			setColumn(0, B11110010);
			break;

		case '6':
			setColumn(3, B10011110);
			setColumn(2, B10010010);
			setColumn(1, B10010010);
			setColumn(0, B11111110);
			break;

		case '7':
			setColumn(3, B11100000);
			setColumn(2, B10011000);
			setColumn(1, B10000110);
			setColumn(0, B10000000);
			break;

		case '8':
			setColumn(3, B11111110);
			setColumn(2, B10010010);
			setColumn(1, B10010010);
			setColumn(0, B11111110);
			break;

		case '9':
			setColumn(3, B11111110);
			setColumn(2, B10010000);
			setColumn(1, B10010000);
			setColumn(0, B11110000);
			break;

		case '0':
			setColumn(3, B11111110);
			setColumn(2, B10000010);
			setColumn(1, B10000010);
			setColumn(0, B11111110);
			break;

		case '%':
			setColumn(3, B00100110);
			setColumn(2, B00010110);
			setColumn(1, B11001000);
			setColumn(0, B11000100);
			break;

		case '^':
			setColumn(3, B00000000);
			setColumn(2, B11100000);
			setColumn(1, B10100000);
			setColumn(0, B11100000);
			break;
	}
}

void setup() {
	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);
	
	SPI.begin();

	SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	setRegForAll(REG_INTENSITY, 1);
	setRegForAll(REG_SCAN_LIMIT, 7);
	setRegForAll(REG_SHUTDOWN, 1);
	SPI.endTransaction();

	commit();


}

uint32_t iteration;

void loop() {
	// put your main code here, to run repeatedly:
	
	clearAll();

	translation = 0;
	drawChar('4');
	translation = 5;
	drawChar('2');
	translation = 10;
	drawChar('%');
	translation = 18;
	drawChar('2');
	translation = 23;
	drawChar('2');
	translation = 28;
	drawChar('^');
	commit();
	delay(1000);

	while (1) {
	}
	iteration++;
	
}
