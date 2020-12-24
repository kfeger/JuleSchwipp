/*
   Effekt für "Lachs"
*/

void SnowSparkle(void) {
  static unsigned long SnowDelay = 0;
  if ((millis() - SnowDelay) > 1234) {
    rot = gruen = blau = 4;
    FillSnow();
    int Pixel = random(NUM_LEDS);
    setPixel(Pixel, 0xff, 0xff, 0xff);
    FastLED.show();
    delay(5);
    FillSnow();
    FastLED.show();
    SnowDelay = millis();
  }
}

void FillSnow(void) {
  MessBeginn = millis();
  for (int i; i < NUM_LEDS; i++) {
    leds[i] = CRGB(rot, gruen, blau);
  }
}
