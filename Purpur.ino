/*
   Diese Funktion wird bei Auswahl
   von "Rose" gerufen
*/

void Regenbogen(void) {
  byte *c;
  uint16_t i = 0;
  static uint16_t j = 0;


  if ((millis() - RainbowSwitch) > 20) {
    if (j < 256 * 5) { // 5 cycles of all colors on wheel
      for (i = 0; i < NUM_LEDS; i++) {
        c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
        setPixel(i, *c, *(c + 1), *(c + 2));
        leds[i].nscale8(hell);
      }
      FastLED.show();
      j++;
    }
    else {
      j = 0;
    }
    RainbowSwitch = millis();
  }
}

// used by rainbowCycle and theaterChaseRainbow
byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}
