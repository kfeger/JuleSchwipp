/*
   Effekt für "Gold"
*/

void LaufendePalette (void) {
  static unsigned long PaletteDelay = 0;
  if ((millis() - PaletteDelay) > 30) {
    startPoint = startPoint + 1;   //if it is 0, then it will all stay the same
    fill_palette(leds, NUM_LEDS, startPoint, paletteQuantityLength, currentPalette, 10, blendingType);
    FastLED.show();
    Serial.println(" StripFunction eins, GOLD");
    PaletteDelay = millis();
  }
}

void BogenLoop (void) {
  #define MAX_BRIGHT 64
  static int j = 0, k = 0;
  static unsigned long BogenLoopDelay = 0;
  static bool DownCount = false, FullCycle = false;
  if ((millis() - BogenLoopDelay) > 30) {
    if (j < 3) {
      // Fade IN
      if (!DownCount) {
        if (k <= MAX_BRIGHT) {
          switch (j) {
            case 0:
              setAll(k, 0, 0);
              if (k == MAX_BRIGHT) {
                DownCount = true;
                Serial.println(k);
              }
              break;
            case 1:
              setAll(0, k, 0);
              if (k == MAX_BRIGHT)
                DownCount = true;
              break;
            case 2:
              setAll(0, 0, k);
              if (k == MAX_BRIGHT)
                DownCount = true;
              break;
          }
          k++;
        }
      }
      else {
        // Fade OUT
        if (k >= 0) {
          switch (j) {
            case 0:
              setAll(k, 0, 0);
              if (k == 0) {
                DownCount = false;
                FullCycle = true;
              }
              break;
            case 1:
              setAll(0, k, 0);
              if (k == 0) {
                DownCount = false;
                FullCycle = true;
              }
              break;
            case 2:
              setAll(0, 0, k);
              if (k == 0) {
                DownCount = false;
                FullCycle = true;
              }
              break;
          }
          k--;
        }
      }
    }

    if (FullCycle) {
      setAll(0,0,0);
      j++;
      if (j == 3)
        j = 0;
      FullCycle  = false;
      k = 0;
    }
    BogenLoopDelay = millis();
  }
}

void setAll(byte r, byte g, byte b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(r, g, b);
    //leds[i].nscale8(MAX_BRIGHT);
  }
  FastLED.show();
}
