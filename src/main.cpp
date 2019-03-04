#include <Arduino.h>
#include <Wire.h>

#define SCL_PIN 5
#define SDA_PIN 4
#define I2C_NUM 0x5A    // Slave Address = 0x5A
#define TA      0x06
#define TO1     0x07
#define TO2     0x08

uint16_t readTemp(uint8_t address);

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  float temp;
  Serial.println("-----Start-----");

  temp = (0.02 * readTemp(TA));
  temp = temp - 273.15;
  Serial.println((String)"" + F("DEBUG--Ambient Temp: ") + (String)temp);

  delay(25);  // Added delay to give sensor time to run next reading (otherwise fails)
  Serial.println("--");

  temp = (0.02 * readTemp(TO1));
  temp = temp - 273.15;
  temp = temp * 9/5 + 32;
  Serial.println((String)"" + F("DEBUG--Object Temp: ") + (String)temp);

  delay(2000);
}

uint16_t readTemp(uint8_t address) {
  uint16_t shiftedVal;
  Wire.beginTransmission(I2C_NUM);
  Wire.write(address);     // Write code to ask for which ever measure ()
  uint16_t retVal = Wire.endTransmission(false);
  Serial.println((String)"" + F("DEBUG--Transmit Return Val: ") + (String)retVal);

  Wire.requestFrom(I2C_NUM, 3);     // Request 3 bytes from I2C address providing LSB, MSB, PEC

  int numVal = Wire.available();
  Serial.println((String)"" + F("DEBUG--Num Vals: ") + (String)numVal);

  if(numVal >= 3) {
    uint8_t LSB = Wire.read();   // Sensor provides LSB before MSB
    uint8_t MSB = Wire.read();   // Sensor provides LSB before MSB
    shiftedVal = (MSB << 8) | LSB;
    uint8_t PEC = Wire.read();    // Sensor provides a PEC after MSB 

    Serial.println((String)"" + F("DEBUG--Sensor MSB: ") + (String)MSB);
    Serial.println((String)"" + F("DEBUG--Sensor LSB: ") + (String)LSB);
    Serial.println((String)"" + F("DEBUG--Sensor Val: ") + (String)shiftedVal);
    Serial.println((String)"" + F("DEBUG--Sensor Pec: ") + (String)PEC);
  }

  return shiftedVal;
}