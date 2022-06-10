#include "I2C_EEPROM.h"


// M24C64
I2C_EEPROM eep(8192, 32, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //give it a second
  delay(1000);
  Serial.println("--- new session ---");

  char ver[10] = "123456789";
  eep.update(0, ver, 10);
  char tver[10];
  eep.get(0, tver, 10);
  Serial.print("version: ");
  Serial.println(tver);


  float fval = 3.14159265359;
  eep.put(32, fval);
  float fval2 = 0.0;
  eep.get(32, fval2);
  Serial.print(sizeof(float));
  Serial.print(" float: ");
  Serial.println(fval2, 10);


  long lval = 1147483647;
  eep.update(64, lval);
  long lval2 = 0;
  eep.get(64, lval2);
  Serial.print("long: ");
  Serial.println(lval2);


  int lsiz = 42;
  char lbuf[64] = "cj4Hgu2LaxwiQOKlqty4NbO41PMVNbB34okWQdnWXn";
  eep.update(128, lbuf, lsiz);
  char tbuf[64];
  memset(tbuf, 0, 63);
  eep.get(128, tbuf, lsiz);
  Serial.print("multiple pages: ");
  Serial.println(tbuf);
}

void loop() {
  // put your main code here, to run repeatedly:

}
