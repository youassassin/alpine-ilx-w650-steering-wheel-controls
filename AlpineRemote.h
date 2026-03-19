/*
  AlpineRemote.h - Library for interfacing with an Alpine
  car audio unit as a wired remote.
  Created by Mattias Winther, July 31, 2016.
  Released into the public domain.
*/
#ifndef AlpineRemote_h
#define AlpineRemote_h

#include "Arduino.h"

class AlpineRemote
{
public:
  AlpineRemote(uint8_t pin);

  void writeVolumeUp();
  void writeVolumeDown();
  void writeMute();
  void writePresetUp();
  void writePresetDown();
  void writeSourceSelect();
  void writeTrackUp();
  void writeTrackDown();
  void writePower();
  void writePlayPause();
  void writeBandSelect();
  void writeActivateSiri();
  void writeActivateMenu();
  void writeDefault();
  void writeStart(String *_bitstream);
  void writeMid(String *_bitstream, uint16_t command);
  void writeEnd(String *_bitstream, bool isLow);

  void writeCommand(uint16_t command);

private:
  uint8_t _pin;
  String _bitstream;
};

#endif