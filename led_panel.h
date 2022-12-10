#ifndef INCLUDED_LED_PANEL_H
#define INCLUDED_LED_PANEL_H

class LedPanel {
public:
	void init(int slaveSelectPin);

	void commit();
	void clear();

	void setWrap(bool horizontal, bool vertical);
	void setOffset(int8_t horizontal, uint8_t vertical);

	void set(uint8_t col, uint8_t row, bool value);
	void setColumn(uint8_t column, uint8_t value);
	void setRow(uint8_t row, uint32_t value);

	void drawChar74(char c);
	void drawChar53(char c);

private:
	void setReg(uint8_t addr, uint8_t data);
	void setRegForAll(uint8_t addr, uint8_t data);

	int slaveSelectPin;
	uint8_t values[8][4];
	int8_t hor_offset;
	int8_t ver_offset;
	bool hor_wrap;
	bool ver_wrap;
};

#endif // included
