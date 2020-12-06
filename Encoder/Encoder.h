#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class ENCODER
{
  private:

  int analogValueA, analogValueB, encoderVal, prevButtonPressed;

  bool execute, buttonPressed;

  public:

  int pinA,  pinB, digitalPin;

  ENCODER(int analogPinA, int analogPinB, int tempDigitalPin);

  int encoderValue();

  bool buttonPress();

};

#endif
