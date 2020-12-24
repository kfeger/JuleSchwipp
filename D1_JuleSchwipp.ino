/*
  Alternative Steuerung des Pavilion durch Amazon's Alexa
  Läuft auf WEMOS D1-Mini
  Die Umschaltung zwischen bisheriger und neuer Steuerung erfolgt mit
  einem 74HCT153 Multiplexer. Die Umschaltung wird durch "Gitta an" (bisherige)
  oder "Gitta aus" (neue Steuerung) durchgeführt.
  Der Pavilion selbst wird in Farbe ("Garten blau") und Helligkeit ("Garten 30%")
  realisiert. Es wird eine Philps Hue emuliert.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <Espalexa.h>         // https://github.com/Aircoookie/Espalexa
#include <FastLED.h>          // https://github.com/FastLED/FastLED
#include <EEPROM.h>

#define NUM_LEDS 27
#define DATA_PIN D6
#define SHOW_COLOR_AFTER 10000

CRGB leds[NUM_LEDS];
//CRGBArray<NUM_LEDS> leds;
// Farben
byte rot = 0, gruen = 0, blau = 0, hell = 0;
bool newColor = false;
enum LED_MODE {NORMAL, GOLD, LACHS, PURPUR};
LED_MODE StripFunction = NORMAL;
CRGBPalette16 currentPalette(PartyColors_p);
//CRGBPalette16 currentPalette(HeatColors_p);
uint8_t i;
uint8_t startPoint = 0;
uint8_t paletteQuantityLength = 128 / NUM_LEDS; //value between 0 and 10, how much of the lights should be lit up at once, 0 being all
TBlendType blendingType; //tBlendType is a type of value like int/char/uint8_t etc., use to choose LINERBLEND and NOBLEND
unsigned long RainbowSwitch = 0;

// Messen
unsigned long MessBeginn;

// EEPROM
// defines für EEPROM
#define EECHK0 0  //Adresse erstes Chek-Byte
#define EECHK1 1  //Adresse zwweites Check-Byte
#define EERED  2   //Stunden öffnen
#define EEGREEN  3   //Minuten öffnen
#define EEBLUE  4   //Stunden schließen
#define EEBRIGHT  5   //Minuten schließen



bool wifiConnected = false;
int ShowColorCounter = 0;

// prototypes
bool connectWifi();

//callback functions
//new callback type, contains device pointer
void bogenChanged(EspalexaDevice* dev);

Espalexa espalexa;

void setup()
{
  Serial.begin(115200);
  //FastLED init
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  rot = 64;
  gruen = 64;
  blau = 0;
  hell = 32;
  FillLedsWiFi(128, 0, 0);
  delay(2000);
  ShowColorCounter = 0;

  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(ConfigModeDisplay);
  wifiManager.autoConnect("Bogen-AP");

  FillLEDsBlack();
  // Alexa init.
  espalexa.addDevice("Bogen", bogenChanged, EspalexaDeviceType::color, 127); //Dimmable device, optional 4th parameter is beginning state (here half on)
  EspalexaDevice* bogen = espalexa.getDevice(0); //this will get "bogen", the index is zero-based
  // EEPROM check und u.U. init.
  EEPROM.begin(128);
  Check_EE();
  SetFromEE();
  SetStripFunction();
  if (StripFunction == NORMAL)
    FillLedsColor();
  espalexa.begin();
  blendingType = LINEARBLEND; // options are LINEARBLEND or NOBLEND - linear is 'cleaner'
}

void loop() {
  espalexa.loop();
  delay(1);
  switch (StripFunction) {
    case NORMAL:
      Einfarbig();
      //Serial.println("StripFunction null, NORMAL");
      break;
    case GOLD:
      //LaufendePalette();
      BogenLoop();
      break;
    case LACHS:
      SnowSparkle();
      //Serial.println("StripFunction zwei, LACHS");
      break;
    case PURPUR:
      Regenbogen();
      //Serial.println("StripFunction drei, ROSE");
      break;
    default:
      Einfarbig();
      //Serial.println("StripFunction default, NORMAL");
      break;
  }
}

void ConfigModeDisplay (WiFiManager *wiFiManager) {
  FillLedsWiFi(0, 0, 128);
}
