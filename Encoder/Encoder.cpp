#include "Arduino.h"
#include "Encoder.h"

    ENCODER::ENCODER(int tempPinA, int tempPinB, int tempDigitalPin)
  {
    pinA = tempPinA;
    pinB = tempPinB;
    digitalPin = tempDigitalPin;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(digitalPin, INPUT_PULLUP);

    valueA = analogRead(pinA);
    valueB = analogRead(pinB);

    encoderVal = 0;
  }

    int ENCODER::encoderValue()
  {
    valueA = analogRead(pinA);
    valueB = analogRead(pinB);

    if (valueA == 0 && valueB != 0 && execute)
    {
      encoderVal--;
      //Counter clockwise *DOWN*

      execute = false;
    }
    else if (valueB == 0 && valueA != 0 && execute)
    {
      encoderVal++;
       //Clockwise *UP*

      execute = false;
    }
    else if (valueA != 0 && valueB != 0 && !execute)
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
