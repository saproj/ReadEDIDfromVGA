/*
Arduino sketch to read EDID structures from VGA monitors.
This is a modified version of edidupdater1_read_edid_and_print_to_term.ino
sketch from http://www.chrisbot.com/i2c-and-monitor-ddc.html
The original sketch could only read the first 32 bytes correctly.

Instruction.
Unplug the monitor. Connect SDA,SCL,+5V,GND pins on Arduino to
the corresponding pins (12,15,9,5) on monitor's VGA connector. Run the sketch,
it will output a HEX dump of 128 bytes of EDID structure. In most cases
it is the only EDID data block available. TODO: enhance the sketch to read additional
128-byte blocks if extensions are present (byte at offset 126 is not zero).
Refer to https://en.wikipedia.org/wiki/Extended_Display_Identification_Data
to interpret EDID data.
There also EDID parsers, here is one online: http://www.edidreader.com/
*/

// SDA (20)  HDB15-12
// SCL (21)  HDB15-15
// +5V       HDB15-9
// GND       HDB15-5

#include <Wire.h>

const int i2c_port = 0x50;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Wire.begin();
  ddcRead();
}

void loop() {
  while (!Serial.available())
    ;
  Serial.parseInt();
  ddcRead();
}

void ddcRead() {
  uint8_t sum = 0;
  
  Wire.beginTransmission(i2c_port);
  Wire.write(0);
  Wire.endTransmission();

  for (int block = 0; block < 128 / BUFFER_LENGTH; block++){
    Wire.requestFrom(i2c_port, BUFFER_LENGTH);
    for (int i = 0; i < BUFFER_LENGTH && Wire.available();) {
      const uint8_t b = Wire.read();
      if (b < 16)
        Serial.print("0");
      Serial.print(b, HEX);
      sum += b;
      i++;
      if (i % 16 == 0)
        Serial.println();
      else if (i % 8 == 0)
        Serial.print("  ");
      else
        Serial.print(" ");
    }
  }
  
  Serial.print("Sum is: ");
  Serial.println(sum, HEX);
}
