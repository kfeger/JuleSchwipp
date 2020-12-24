/*
   Die eigentliche Alexa-Steuerung
*/
void bogenChanged(EspalexaDevice* bogen)
{
  if (bogen == nullptr) return;

  Serial.print("bogen geaendert nach Helligkeit ");
  hell = bogen->getValue();
  Serial.print(hell);
  rot = bogen->getR();
  Serial.print(", Farben Rot ");
  Serial.print(rot);
  gruen =  bogen->getG();
  Serial.print(", Grün ");
  Serial.print(gruen);
  blau =  bogen->getB();
  Serial.print(", Blau");
  Serial.println(blau);
  SaveToEE();
  SetStripFunction();
  Serial.println("Daten ins EEPROM gesichert");
  newColor = true;
}

void SetStripFunction() {
  if ((rot == 255) && (gruen == 212) && (blau == 0)) {
    StripFunction = GOLD;
  }
  else if ((rot == 255) && (gruen == 160) && (blau == 122)) {
    StripFunction = LACHS;
  }
  else if ((rot == 171) && (gruen == 35) && (blau = 255)) {
    StripFunction = PURPUR;
  }
  else {
    StripFunction = NORMAL;
  }  
}
