#include "config.h"
#include "AlpineRemote.h"

const uint8_t _cmdStart[3] = {
    0xd7, 0xdb, 0xab};

const uint8_t _cmdEnd[2] = {
    0x55, 0xD5};

AlpineRemote::AlpineRemote(uint8_t pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void AlpineRemote::writeVolumeUp()
{
  AlpineRemote::writeCommand(0xDBD6);
}

void AlpineRemote::writeVolumeDown()
{
  AlpineRemote::writeCommand(0x6DF6);
}

void AlpineRemote::writeMute()
{
  AlpineRemote::writeCommand(0xADEE);
}

void AlpineRemote::writePresetUp()
{
  AlpineRemote::writeCommand(0xABEF);
}

void AlpineRemote::writePresetDown()
{
  AlpineRemote::writeCommand(0x55FF);
}

void AlpineRemote::writeSourceSelect()
{
  AlpineRemote::writeCommand(0xB7DB);
}

void AlpineRemote::writeTrackUp()
{
  AlpineRemote::writeCommand(0xBBDA);
}

void AlpineRemote::writeTrackDown()
{
  AlpineRemote::writeCommand(0x5DFA);
}

void AlpineRemote::writePower()
{
  AlpineRemote::writeCommand(0x77EB);
}

void AlpineRemote::writePlayPause()
{
  AlpineRemote::writeCommand(0x57FD);
}

void AlpineRemote::writeBandSelect()
{
  AlpineRemote::writeCommand(0x6BF7);
}

void AlpineRemote::writeActivateSiri()
{
  AlpineRemote::writeCommand(0x7B6E);
}

void AlpineRemote::writeDefault()
{
  delayMicroseconds(500);
  digitalWrite(_pin, LOW);
  delayMicroseconds(500);
}

void AlpineRemote::writeStart(char *_bitstream, uint8_t &pos)
{
  for (byte i = 0; i < 3; i++)
  {
    for (int j = 7; j >= 0; j--)
    {
      bool bit = _cmdStart[i] & (1 << j);
      digitalWrite(_pin, bit ? HIGH : LOW);
      _bitstream[pos++] = bit ? '1' : '0';
      AlpineRemote::writeDefault();
    }
  }
}

void AlpineRemote::writeMid(char *_bitstream, uint8_t &pos, uint16_t command)
{
  for (int j = 15; j >= 0; j--)
  {
    bool bit = command & (1 << j);
    digitalWrite(_pin, bit ? HIGH : LOW);
    _bitstream[pos++] = bit ? '1' : '0';
    AlpineRemote::writeDefault();
  }
}

void AlpineRemote::writeEnd(char *_bitstream, uint8_t &pos, bool isLow)
{
  for (int j = 7; j >= 0; j--)
  {
    bool bit = _cmdEnd[isLow] & (1 << j);
    digitalWrite(_pin, bit ? HIGH : LOW);
    _bitstream[pos++] = bit ? '1' : '0';
    AlpineRemote::writeDefault();
  }
}

void AlpineRemote::writeCommand(uint16_t command)
{
  char buf[33];
  uint8_t pos = 0;

  digitalWrite(_pin, HIGH);
  delay(8);
  digitalWrite(_pin, LOW);
  delayMicroseconds(4500);

  AlpineRemote::writeStart(buf, pos);
  AlpineRemote::writeMid(buf, pos, command);
  AlpineRemote::writeEnd(buf, pos, !(command & 1));
  buf[pos] = '\0';

#ifdef DEBUG
  Serial.print(buf);
  Serial.print(F(" ("));
  Serial.print(strtol(buf, NULL, 2), HEX);
  Serial.println(F(")"));
#endif
}