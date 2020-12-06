#ifndef LCD_INTERFACE_h
#define LCD_INTERFACE_h

#include <Encoder.h>
#include <LiquidCrystal.h>
#include <String.h>
#include "Arduino.h"

//class LCD_AND_ROTARY_ENCODER_UI
//{
    //public:

    //LCD_AND_ROTARY_ENCODER_UI(
      //  int LCD_resgisterSelectPin, int LCD_enablePin, int LCD_digitalPin0, int LCD_digitalPin1, int LCD_digitalPin2, int LCD_digitalPin3,
     //   int LCD_displayColoumns, int LCD_displayRows,
      //  int dial_SW_Pin, int dial_CLK_Pin, int dial_DT_Pin);

   
 class LCD_INTERFACE
    {   
        private:

        bool updateLCD = false;

        int prevDialVal, dialVal;
        int row, prevRow;
        int upperRowOcurrences, bottomRowOcurrences;

        String arrow = "->", removeArrow = "  ";

        public:

         int LCD_resgisterSelectPin = 12, LCD_enablePin = 11, LCD_digitalPin0 = 5, LCD_digitalPin1 = 4, LCD_digitalPin2 = 8, LCD_digitalPin3 = 6,
         LCD_displayColoumns = 16, LCD_displayRows = 2,
         dial_SW_Pin = 2, dial_CLK_Pin = A0, dial_DT_Pin = A1, numberOfOptions;            

        private:

        ENCODER dial = ENCODER(dial_CLK_Pin, dial_DT_Pin, dial_SW_Pin);

        LiquidCrystal display = LiquidCrystal(LCD_resgisterSelectPin, LCD_enablePin, LCD_digitalPin0, LCD_digitalPin1, LCD_digitalPin2, LCD_digitalPin3);
        
        int printOptionHorizontalLocation(String tempOption);

        void printOptions(bool topRow, String listOfOptions[]);

        void printArrow();
    
        public:        
    
        LCD_INTERFACE();
        
        void interface(String listOfOptions[]);
    };


//};

#endif