#include "AlpineRemote.h"

// Thresholds
static const uint8_t  SW1_MIN = 2;
static const uint8_t  SW2_MIN = 3;
static const uint16_t SW1_B1  = 213, SW1_B2 = 198, SW1_B3 = 171, SW1_B4 = 121;
static const uint16_t SW2_B1  = 219, SW2_B2 = 219, SW2_B3 = 124, SW2_B4 = 175;
static const uint16_t SW3_B1  = 222;

// Pins
static const uint8_t REF1 = A6, REF2 = A4, REF3 = A1;

// State
uint16_t sw1, sw2, sw3;
bool wasPressed = false;

AlpineRemote remote(5);

#ifdef DEBUG
  unsigned long lastPrintTime = 0;
#endif

void setup()
{
  analogReference(EXTERNAL);
  DDRD = B11111111;
  Serial.begin(115200);
  Serial.println(F("STC Module Ready!"));
}

inline void readInput()
{
  sw1 = analogRead(REF1);
  sw2 = analogRead(REF2);
  sw3 = analogRead(REF3);
}

void handleSw1()
{
  if      (sw1 > SW1_B1) { remote.writeTrackUp();     Serial.print(F("seek+")); }
  else if (sw1 > SW1_B2) { remote.writeTrackDown();   Serial.print(F("seek-")); }
  else if (sw1 > SW1_B3) { remote.writeVolumeUp();    Serial.print(F("vol+"));  }
  else if (sw1 > SW1_B4) { remote.writeVolumeDown();  Serial.print(F("vol-"));  }
}

void handleSw2()
{
  if      (sw2 > SW2_B1) { remote.writeSourceSelect();  Serial.print(F("mode"));     }
  else if (sw2 > SW2_B2) {                               Serial.print(F("off call")); }
  else if (sw2 > SW2_B3) {                               Serial.print(F("on call"));  }
  else if (sw2 > SW2_B4) { remote.writeActivateSiri();  Serial.print(F("voice"));    }
}

void handleSw3()
{
  if (sw3 > SW3_B1) Serial.print(F("display"));
}

void buttonPressed()
{
  bool s1 = sw1 > SW1_MIN, s2 = sw2 > SW2_MIN, s3 = sw3 > 0;

  if (s1 && s2 && s3) {
    Serial.print(F("sw1&sw2&sw3"));
  } else if (s1 && s2) {
    Serial.print(F("sw1&sw2"));
  } else if (s1 && s3) {
    Serial.print(F("sw1&sw3"));
  } else if (s2 && s3) {
    Serial.print(F("sw2&sw3"));
  } else if (s1) {
    handleSw1();
  } else if (s2) {
    handleSw2();
  } else if (s3) {
    handleSw3();
  }
  Serial.println();
}

void loop()
{
  readInput();
  bool pressed = sw1 > SW1_MIN || sw2 > SW2_MIN || sw3 > 0;

  if (pressed && !wasPressed) {
    buttonPressed();
  }
  wasPressed = pressed;

#ifdef DEBUG
  if (!pressed && millis() - lastPrintTime > 1000) {
    Serial.print(millis());
    Serial.print(F(": sw1="));  Serial.print(sw1);
    Serial.print(F(", sw2="));  Serial.print(sw2);
    Serial.print(F(", sw3="));  Serial.println(sw3);
    lastPrintTime = millis();
  }
#endif

  delay(20);
}
