#ifndef I2C_EEPROM_H
#define I2C_EEPROM_H

#include <Arduino.h>


class I2C_EEPROM {
public:
    // size: this is the total bytes the eeprom support
    // is_hi_low_addr: does it use high and low bytes for addr
    // page_size: max number of writes in a single call allowed
    I2C_EEPROM(uint16_t size, uint16_t page_size, bool is_hi_low_addr);
    // i2c_addr: this is the i2c address for this device
    // size: this is the total bytes the eeprom support
    // is_hi_low_addr: does it use high and low bytes for addr
    // page_size: max number of writes in a single call allowed
    I2C_EEPROM(int i2c_addr, uint16_t size, uint16_t page_size, bool is_hi_low_addr);

    // is responding via i2c at i2c_addr
    bool isReady();
    // waits until ready for read/writing
    void waitForReady();
    // returns the size of the eeprom in bytes
    uint16_t size();

    // put will write the value to the eeprom
    // it will break it into multiple pages if needed
    bool put(uint16_t index, uint8_t *value, uint16_t length);
    // update will write the value to the eeprom only if different
    // it will break it into multiple pages if needed
    bool update(uint16_t index, uint8_t *value, uint16_t length);
    // get will read the value to the eeprom
    // it will break it into multiple pages if needed
    bool get(uint16_t index, uint8_t *value, uint16_t length);

    // put will write the value to the eeprom
    // it will break it into multiple pages if needed
    bool put(uint16_t index, char *value, uint16_t length);
    // update will write the value to the eeprom only if different
    // it will break it into multiple pages if needed
    bool update(uint16_t index, char *value, uint16_t length);
    // get will read the value to the eeprom
    // it will break it into multiple pages if needed
    bool get(uint16_t index, char *value, uint16_t length);

    bool put(uint16_t index, int &value);
    bool update(uint16_t index, int &value);
    bool get(uint16_t index, int &value);

    bool put(uint16_t index, long &value);
    bool update(uint16_t index, long &value);
    bool get(uint16_t index, long &value);

    bool put(uint16_t index, float &value);
    bool update(uint16_t index, float &value);
    bool get(uint16_t index, float &value);

    // write page up to max page size
    // returns false if all the data could not be written
    bool writePage(uint16_t index, uint8_t *value, uint16_t length);

    // write page up to max page size, but only if different
    // this is important to extend the life of the eeprom 
    // returns false if all the data could not be written
    bool updatePage(uint16_t index, uint8_t *value, uint16_t length);

    // write page up to max page size
    // returns false if all the data could not be written
    bool readPage(uint16_t index, uint8_t *value, uint16_t length);

    // read 1 byte from eeprom
    uint8_t read(uint16_t index);
    // write 1 byte to eeprom
    bool write(uint16_t index, uint8_t value);
    // write 1 byte to eeprom (if different)
    bool update(uint16_t index, uint8_t value);  

private:
	int _i2c_addr;
	bool _is_hilow_addr;
	uint16_t _size;
	uint16_t _page_size;

};




#endif 
