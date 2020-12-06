#include "Arduino.h"
#include "Encoder.h"

    ENCODER::ENCODER(int analogPinA, int analogPinB, int tempDigitalPin)
  {
    pinA = analogPinA;
    pinB = analogPinB;
    digitalPin = tempDigitalPin;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(digitalPin, INPUT_PULLUP);

    analogValueA = analogRead(pinA);
    analogValueB = analogRead(pinB);

    encoderVal = 0;
  }

    int ENCODER::encoderValue()
  {
    analogValueA = analogRead(pinA);
    analogValueB = analogRead(pinB);

    if (analogValueA == 0 && analogValueB != 0 && execute)
    {
      encoderVal--;
      //Counter clockwise *DOWN*

      execute = false;
    }
    else if (analogValueB == 0 && analogValueA != 0 && execute)
    {
      encoderVal++;
       //Clockwise *UP*

      execute = false;
    }
    else if (analogValueA != 0 && analogValueB != 0 && !execute)
    {
      execute = true;
    }

    return encoderVal;
  }

 bool ENCODER::buttonPress()
  {
      int tempButtonPress = digitalRead(digitalPin);

      if (tempButtonPress == LOW && prevButtonPressed == HIGH)
      {
          buttonPressed = true;
      }
      else if (tempButtonPress == HIGH && prevButtonPressed == LOW)
      {
          buttonPressed = false;
      }

     prevButtonPressed = tempButtonPress;

     return buttonPressed;
  }
