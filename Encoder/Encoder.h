#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class ENCODER
{
  private:

  int valueA, valueB, encoderVal, prevButtonPressed;

  bool execute, buttonPressed;

  public:

  int pinA,  pinB, digitalPin;

  ENCODER(int tempPinA, int tempPinB, int tempDigitalPin);

  int encoderValue();

  bool buttonPress();

};

#endif
