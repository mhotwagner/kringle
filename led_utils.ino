// Blinks a passed set of LEDs n times

//int c_red = Adafruit_NeoPixel::Color(0, 255, 0);
//int c_green = Adafruit_NeoPixel::Color(255, 0, 0);
//int c_yellow = Adafruit_NeoPixel::Color(255, 255, 0);
//int c_white = Adafruit_NeoPixel::Color(255, 255, 255);

void turnOnLeds(Adafruit_NeoPixel *leds) {
  Serial.println("[Turning on LEDs");
  leds->show();
}

void turnOnLeds(Adafruit_NeoPixel *leds, int c) {
  Serial.print("[Turning on LEDs to color ");
  Serial.println(c);
  leds->fill(c);
  leds->show();
}

void turnOnLeds(Adafruit_NeoPixel *leds, int c, int b) {
  Serial.print("[INFO] Turning on LEDs to color ");
  Serial.print(c);
  Serial.print(" at ");
  Serial.print(b);
  Serial.println("% brightness");
  leds->fill(c);
  leds->setBrightness(b);
  leds->show();
}

void setColor(Adafruit_NeoPixel *leds, int c) {
  turnOnLeds(leds, c);
}

void setBrightness(Adafruit_NeoPixel *leds, int b) {
  Serial.print("[INFO] Setting LED brightness to ");
  Serial.print(b);
  Serial.println("%");
  leds->setBrightness(b);
  leds->show();
}

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

void blinkLeds(Adafruit_NeoPixel *leds, int n, int c) {
  Serial.println("[INFO] Blinking LEDs");
  int i = 0;
  leds->fill(); leds->show();
  while (i < n) {
    i++;
    Serial.println("[INFO] blink!");
    leds->fill(c);
    leds->show(); delay(100);
    leds->fill(); leds->show(); delay(100);
  }
}

void successBlink(Adafruit_NeoPixel *leds) {
  Serial.println("[INFO] Success blink");
  blinkLeds(leds, 2, c_green);
}

void errorBlink(Adafruit_NeoPixel *leds) {
  Serial.println("[INFO] Error blink");
  blinkLeds(leds, 2, c_red);
}

void infoBlink(Adafruit_NeoPixel *leds) {
  Serial.println("[INFO] Info blink");
  blinkLeds(leds, 2, c_yellow);
}

void every_other(Adafruit_NeoPixel *leds, int c_a, int c_b) {
  int i = 0;
  while (i < 10) {
    int j = 0;
    // red first
    while (j < 6) {
      if (j % 2 == 0) {
        leds->setPixelColor(j, c_a);
      } else {
        leds->setPixelColor(j, c_b);
      }
      j++;
    }
    leds->show();
    delay(500);
    
    // greeen first
    j = 0;
    while (j < 6) {
      if (j % 2 == 0) {
        leds->setPixelColor(j, c_b);
      } else {
        leds->setPixelColor(j, c_a);
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

void xmas(Adafruit_NeoPixel *leds) {
  Serial.println("[INFO] XMAS Mode!");
  every_other(leds, c_red, c_green);
}

void jmas(Adafruit_NeoPixel *leds) {
  Serial.println("[INFO] JMAS Mode!");
  every_other(leds, c_white, c_blue);
}
