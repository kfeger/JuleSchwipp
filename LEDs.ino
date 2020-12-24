/*
   Steuerung der LED-Strips
*/
void FillLEDsBlack(void) {
  FastLED.clear();
  FastLED.show();
}

void FillLedsHSV(byte hue, byte sat, byte val) {
  for (int i; i < NUM_LEDS; i++) {
    leds[i].setHSV( hue, sat, val);
  }
  FastLED.show();
}

void FillLedsColor(void) {
  MessBeginn = millis();
  for (int i; i < NUM_LEDS; i++) {
    leds[i] = CRGB(rot, gruen, blau);
    leds[i].nscale8(hell);
  }
  FastLED.show();
}

void FillLedsWiFi(byte r, byte g, byte b) {
  FastLED.clear();
  leds[0] = CRGB(r, g, b);
  leds[NUM_LEDS - 1] = CRGB(r, g, b);
  FastLED.show();
}

void Einfarbig (void) {
  if (newColor) {
    MessBeginn = millis();
    FillLedsColor();
    SaveToEE();
    newColor = false;
  }
  // Software-Fix. Nach einiger Zeit gehen ein paar LEDs an.
  // Daher wird alle ca. 10 sec. der LED-String neu gesetzt.
  else {
    ShowColorCounter++;
    if (ShowColorCounter >= SHOW_COLOR_AFTER) {
      FillLedsColor();
      ShowColorCounter = 0;
    }
    else
      ShowColorCounter++;
  }
}
