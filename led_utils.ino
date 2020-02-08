// Blinks a passed set of LEDs n times

int c_red = Adafruit_NeoPixel::Color(255, 0, 0);
int c_green = Adafruit_NeoPixel::Color(0, 255, 0);
int c_white = Adafruit_NeoPixel::Color(255, 255, 255);

void blinkLeds(Adafruit_NeoPixel *leds, int n) {
  Serial.println("[INFO] Blinking LEDs");
  int i = 0;
  leds->fill(); leds->show();
  while (i < n) {
    i++;
    Serial.println("[INFO] blink!");
    leds->fill(c_white);
    leds->show(); delay(100);
    leds->fill(); leds->show(); delay(100);
  }
}

void xmas(Adafruit_NeoPixel *leds) {
  Serial.println("[INFO] XMAS MODE!");
  int i = 0;
  while (i < 20) {
    int j = 0;
    // red first
    while (j < 6) {
      if (j % 2 == 0) {
        leds->setPixelColor(j, c_red);
      } else {
        leds->setPixelColor(j, c_green);
      }
      j++;
    }
    leds->show();
    delay(500);
    
    // greeen first
    j = 0;
    while (j < 6) {
      if (j % 2 == 0) {
        leds->setPixelColor(j, c_green);
      } else {
        leds->setPixelColor(j, c_red);
      }
      j++;
    }
    leds->show();
    delay(500);
    i++;
  }
  leds->fill();
  leds->show();
}
