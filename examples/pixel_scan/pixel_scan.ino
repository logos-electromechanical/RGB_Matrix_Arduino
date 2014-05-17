#include <Wire.h>
#include <RGB_Matrix_Arduino.h>

uint8_t buf[192];
RGB_Matrix matrix = RGB_Matrix((uint8_t)0x7e, buf);

void setup (void) {
  Serial.begin(57600);
  Serial.println("I live!");
  matrix.begin();
}

void loop (void) {
  static uint32_t color = 0xff;
  uint8_t i, j, k;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      matrix.setPixel(i,j,color);
      Serial.print("Setting pixel: "); Serial.print(i);
      Serial.print(", "); Serial.print(j); Serial.print(" color: ");
      Serial.println(color, HEX);
      matrix.printBuf();
      matrix.transmitBuf();
      // while(!Serial.available()); Serial.read();
      delay(100);
    }
  }
  color = color << 4;
  if (color > 0x0fffffff) color = 0xff;
}


