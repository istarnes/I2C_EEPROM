
#include "I2C_EEPROM.h"
#include <Wire.h>

#define DEFAULT_IC2_ADDR 0x50

I2C_EEPROM::I2C_EEPROM(uint16_t size, uint16_t page_size, bool is_hi_low_addr) {
	this->_i2c_addr = DEFAULT_IC2_ADDR;
	this->_size = size;
	this->_is_hilow_addr = is_hi_low_addr;
	this->_page_size = page_size;
	Wire.begin();
}

I2C_EEPROM::I2C_EEPROM(int i2c_addr, uint16_t size, uint16_t page_size, bool is_hi_low_addr) {
	this->_i2c_addr = i2c_addr;
	this->_size = size;
	this->_is_hilow_addr = is_hi_low_addr;
	this->_page_size = page_size;
	Wire.begin();
}

bool I2C_EEPROM::isReady() {
    Wire.beginTransmission(this->_i2c_addr);
    return Wire.endTransmission() == 0;
}

void I2C_EEPROM::waitForReady() {
	while (!this->isReady()) {
	  delay(10);
	};
}

uint16_t I2C_EEPROM::size() {
	return this->_size;
}

// SMART PUT / GET

bool I2C_EEPROM::put(uint16_t index, char *value, uint16_t length) {
	return this->put(index, (uint8_t*)value, length);
}

bool I2C_EEPROM::update(uint16_t index, char *value, uint16_t length) {
	return this->update(index, (uint8_t*)value, length);
}

bool I2C_EEPROM::get(uint16_t index, char *value, uint16_t length) {
	return this->get(index, (uint8_t*)value, length);
}

// int
bool I2C_EEPROM::put(uint16_t index, int &value) {
	return this->writePage(index, (uint8_t*)&value, sizeof(int));
}

bool I2C_EEPROM::update(uint16_t index, int &value) {
	return this->updatePage(index, (uint8_t*)&value, sizeof(int));
}

bool I2C_EEPROM::get(uint16_t index, int &value) {
	return this->readPage(index, (uint8_t*)&value, sizeof(int));
}

// long
bool I2C_EEPROM::put(uint16_t index, long &value) {
	return this->put(index, (uint8_t*)&value, sizeof(long));
}

bool I2C_EEPROM::update(uint16_t index, long &value) {
	return this->update(index, (uint8_t*)&value, sizeof(long));
}

bool I2C_EEPROM::get(uint16_t index, long &value) {
	return this->get(index, (uint8_t*)&value, sizeof(long));
}

// float
bool I2C_EEPROM::put(uint16_t index, float &value) {
	return this->writePage(index, (uint8_t*)&value, sizeof(float));
}

bool I2C_EEPROM::update(uint16_t index, float &value) {
	return this->updatePage(index, (uint8_t*)&value, sizeof(float));
}

bool I2C_EEPROM::get(uint16_t index, float &value) {
	return this->readPage(index, (uint8_t*)&value, sizeof(float));
}

bool I2C_EEPROM::put(uint16_t index, uint8_t *value, uint16_t length) {
	if (length > this->_page_size) {
		uint16_t remaining = length;
		while (remaining > 0) {
			uint16_t plen = this->_page_size;
			if (remaining < this->_page_size) {
				plen = remaining;
			}
			remaining -= plen;
			this->writePage(index, value, plen);
			value += plen;
			index += plen;
		}
		return true;
	}
	return this->writePage(index, value, length);
}

bool I2C_EEPROM::update(uint16_t index, uint8_t *value, uint16_t length) {
	if (length > this->_page_size) {
		uint16_t remaining = length;
		while (remaining > 0) {
			uint16_t plen = this->_page_size;
			if (remaining < this->_page_size) {
				plen = remaining;
			}
			remaining -= plen;
			this->updatePage(index, value, plen);
			value += plen;
			index += plen;
		}
		return true;
	}
	return this->updatePage(index, value, length);
}

bool I2C_EEPROM::get(uint16_t index, uint8_t *value, uint16_t length) {
	if (length > this->_page_size) {
		uint16_t remaining = length;
		while (remaining > 0) {
			uint16_t plen = this->_page_size;
			if (remaining < this->_page_size) {
				plen = remaining;
			}
			remaining -= plen;
			this->readPage(index, value, plen);
			value += plen;
			index += plen;
		}
		return true;
	}
	return this->readPage(index, value, length);
}

// WRITE / READ PAGE

bool I2C_EEPROM::writePage(uint16_t index, uint8_t *value, uint16_t length) {
	if (length > this->_page_size) {
		return false;
	}
	uint8_t hb = (uint8_t)(index  >> 8);
	uint8_t lb  = (uint8_t)(index & 0x00ff);

	this->waitForReady();
	Wire.beginTransmission(this->_i2c_addr);
	Wire.write(hb);
	Wire.write(lb);
	for (int i=0; i<length; i++) {
		Wire.write(*value++);
	}
	byte error = Wire.endTransmission();
	delay(10);
	return error == 0;
}

bool I2C_EEPROM::updatePage(uint16_t index, uint8_t *value, uint16_t length) {
	uint8_t tmp[64];
	if (!this->readPage(index, tmp, length)) {
		return false;
	}
	if (memcmp(tmp, value, length) == 0) {
		return true;
	}
	return this->writePage(index, value, length);
}

bool I2C_EEPROM::readPage(uint16_t index, uint8_t *value, uint16_t length) {
	if (length > this->_page_size) {
		return false;
	}

	uint8_t hb = (uint8_t)(index  >> 8);
	uint8_t lb  = (uint8_t)(index & 0x00ff);

	this->waitForReady();
	Wire.beginTransmission(this->_i2c_addr);
	if (this->_is_hilow_addr) {
		Wire.write(hb);
	} else {
		index += hb;
		Wire.write(index);
	}
	Wire.write(lb);
	byte error = Wire.endTransmission();
	if (error != 0) {
		return false;
	}
	Wire.requestFrom(this->_i2c_addr, length);
	delay(10);
	while(Wire.available()) {
		*value++ = Wire.read();
	}
	return true;
}


// BASIC READ/WRITES 

uint8_t I2C_EEPROM::read(uint16_t index) {
	uint8_t value = 0;
	this->readPage(index, &value, 1);
	return value;
}

bool I2C_EEPROM::write(uint16_t index, uint8_t value) {
	return this->writePage(index, &value, 1);
}

bool I2C_EEPROM::update(uint16_t index, uint8_t value) {
	return this->updatePage(index, &value, 1);
}

