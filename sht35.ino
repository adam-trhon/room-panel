#include "sht35.h"

static uint32_t old_humidity = 0;
static uint32_t old_temperature = 0;

void sht35_measure(uint32_t *humidity, uint32_t *temperature) {
	uint8_t measurementRequest[] = {0x2C, 0x06};

	Wire.beginTransmission(0x44);
	Wire.write(measurementRequest, 2);
	Wire.endTransmission(true);

	delay(10); // maybe fixes the random worng values

	uint8_t response[6] = {0};
	int bytes_read = Wire.requestFrom(0x44, 6, true);

	for (int i = 0; i < bytes_read; ++i) {
		response[i] = Wire.read();
	}

	uint32_t temp_off = (((uint32_t) response[0]) << 8) + response[1]; 
	temp_off *= 1750;
	temp_off /= 0xFFFF;

	*temperature = ((int32_t)temp_off) - 450;

	*humidity = (((uint32_t) response[3]) << 8) + response[4];
	*humidity *= 100;
	*humidity /= 0xFFFF;

	if (*humidity == 0 || *humidity > 100 || *temperature == 0 || *temperature > 500) {
		*humidity = old_humidity;
		*temperature = old_temperature;
	} else {
		old_humidity = *humidity;
		old_temperature = *temperature;
	}
}
