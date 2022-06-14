# I2C_EEPROM

Generic Arduino I2C EEPROM Library.

* Supports write by page size to increase performance
* Can check existing values before writing to increase EEPROM life
* Support writing/reading basic types (char*, int, long, float)
* Support writing buffers



##  Examples

```c++
// instantiate instance passing in size in bytes, page size, address mode
I2C_EEPROM eep(8192, 32, true);

// write version string to EEPROM at address index 0
char ver[10] = "123456789";
// update will only write the value if the current data is different
// you can use eep.put(0, ver, 10); to force it
eep.update(0, ver, 10);
// read 10 bytes of data from address index 0
char tmp[10];
eep.get(0, tver, 10);

// force write float to address index 32
float fval = 3.14159265359;
eep.put(32, fval);
float fval2 = 0.0;
eep.get(32, fval2);

// update long to address index 64
long lval = 1147483647;
eep.update(64, lval);
long lval2 = 0;
eep.get(64, lval2);
Serial.print("long: ");
Serial.println(lval2);

// write a string that is bigger then the page size
// this will handle writing across pages
int lsiz = 42;
char lbuf[64] = "cj4Hgu2LaxwiQOKlqty4NbO41PMVNbB34okWQdnWXn";
eep.update(128, lbuf, lsiz);
char tbuf[64];
memset(tbuf, 0, 63);
eep.get(128, tbuf, lsiz);
```





## Support IC2 EEPROMs

| Chip | Size  | Page Size | Address Mode |
| --------- | ----- | --------- | ---------- |
| AT24C01   | 128   | 8         | 1          |
| AT24C02   | 256   | 8         | 1          |
| AT24C04   | 512   | 16        | 1          |
| AT24C08   | 1024  | 16        | 1          |
| AT24C16   | 2048  | 16        | 1          |
| AT24C32   | 4096  | 32        | 2          |
| AT24C64   | 8192  | 64        | 2          |
| AT24C128  | 16384 | 64        | 2          |
| AT24C256  | 32768 | 64        | 2          |
| AT24C512  | 65536 | 128       | 2          |
| M24C01             | 128   | 16        | 1          |
| ST24C01            | 128   | 8         | 1          |
| M24C02             | 256   | 16        | 1          |
| ST24C02            | 256   | 8         | 1          |
| M24C04             | 512   | 16        | 1          |
| ST24C04            | 512   | 8         | 1          |
| M24C08             | 1024  | 16        | 1          |
| ST24C08            | 1024  | 16        | 1          |
| M24C16             | 2048  | 16        | 1          |
| M24C32             | 4096  | 32        | 2          |
| M24C64             | 8192  | 32        | 2          |
| M24128             | 16384 | 64        | 2          |
| M24256             | 32768 | 64        | 2          |
| M24512             | 65536 | 64        | 2          |
| MC24C01C     | 128  | 16        | 1          |
| MC24AA02E48  | 256  | 8         | 1          |
| MC24AA025E48 | 256  | 16        | 1          |

