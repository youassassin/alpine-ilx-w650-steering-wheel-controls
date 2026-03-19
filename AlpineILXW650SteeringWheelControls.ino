// #define DEBUG
#include "config.h"
#include "AlpineRemote.h"

// Thresholds
static const uint8_t SW1_IDLE = 2;
static const uint8_t SW2_IDLE = 3;
static const uint8_t MARGIN = 2;
static const uint16_t SW1_TRACKUP_MIN = 213, SW1_TRACKDN_MIN = 198, SW1_VOLUP_MIN = 171, SW1_VOLDN_MIN = 121;
static const uint16_t SW2_SOURCE_MIN = 219, SW2_ENDCALL_MIN = 203, SW2_VOICE_MIN = 175, SW2_ANSWER_MIN = 124;
static const uint16_t SW3_DISPLAY_MIN = 222;

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
  if (sw1 > SW1_TRACKUP_MIN - MARGIN)
  {
    remote.writeTrackUp();
    Serial.print(F("seek+"));
  }
  else if (sw1 > SW1_TRACKDN_MIN - MARGIN)
  {
    remote.writeTrackDown();
    Serial.print(F("seek-"));
  }
  else if (sw1 > SW1_VOLUP_MIN - MARGIN)
  {
    remote.writeVolumeUp();
    Serial.print(F("vol+"));
  }
  else if (sw1 > SW1_VOLDN_MIN - MARGIN)
  {
    remote.writeVolumeDown();
    Serial.print(F("vol-"));
  }
}

void handleSw2()
{
  if (sw2 > SW2_SOURCE_MIN - MARGIN)
  {
    remote.writeSourceSelect();
    Serial.print(F("mode"));
  }
  else if (sw2 > SW2_ENDCALL_MIN - MARGIN)
  {
    Serial.print(F("end call"));
  }
  else if (sw2 > SW2_VOICE_MIN - MARGIN)
  {
    remote.writeActivateSiri();
    Serial.print(F("voice"));
  }
  else if (sw2 > SW2_ANSWER_MIN - MARGIN)
  {
    Serial.print(F("answer call"));
  }
}

void handleSw3()
{
  if (sw3 > SW3_DISPLAY_MIN - MARGIN)
    Serial.print(F("display"));
}

void buttonPressed()
{
  bool s1 = sw1 > SW1_IDLE, s2 = sw2 > SW2_IDLE, s3 = sw3 > 0;

  if (s1 && s2 && s3)
  {
    Serial.print(F("sw1&sw2&sw3"));
  }
  else if (s1 && s2)
  {
    Serial.print(F("sw1&sw2"));
  }
  else if (s1 && s3)
  {
    Serial.print(F("sw1&sw3"));
  }
  else if (s2 && s3)
  {
    Serial.print(F("sw2&sw3"));
  }
  else if (s1)
  {
    handleSw1();
  }
  else if (s2)
  {
    handleSw2();
  }
  else if (s3)
  {
    handleSw3();
  }
  Serial.println();
}

void loop()
{
  readInput();
  bool pressed = sw1 > SW1_IDLE || sw2 > SW2_IDLE || sw3 > 0;

  if (pressed && !wasPressed)
  {
    buttonPressed();
  }
  wasPressed = pressed;

#ifdef DEBUG
  if (!pressed && millis() - lastPrintTime > 1000)
  {
    Serial.print(millis());
    Serial.print(F(": sw1="));
    Serial.print(sw1);
    Serial.print(F(", sw2="));
    Serial.print(sw2);
    Serial.print(F(", sw3="));
    Serial.println(sw3);
    lastPrintTime = millis();
  }
#endif

  delay(20);
}
