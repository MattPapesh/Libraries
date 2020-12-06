#include <Encoder.h>
#include <LiquidCrystal.h>
#include <String.h>
#include <LCD_AndRotaryEncoder_UI.h>
#include "Arduino.h"
 
          LCD_INTERFACE::LCD_INTERFACE()
        {            
            display.begin(LCD_displayColoumns, LCD_displayRows);  
        }

        int LCD_INTERFACE::printOptionHorizontalLocation(String tempOption)
        {
            int tempHorizontalCursurPos;
             // finds the horizontal position of the cursur to center the printed text
    
            tempHorizontalCursurPos = (9 - (tempOption.length() /2) ); // equation centers option text

            return tempHorizontalCursurPos;
        }

        void LCD_INTERFACE::printOptions(bool topRow, String listOfOptions[])
        { 
            int primaryOption, peripheralOption;

            if (!topRow)
            {
                peripheralOption = dialVal - 1;
                primaryOption = dialVal;
            }
            else if (topRow)
            {
                peripheralOption = dialVal + 1;
                primaryOption = dialVal;
            }

            display.setCursor(printOptionHorizontalLocation(listOfOptions[primaryOption] ), topRow); 

            display.print(listOfOptions[primaryOption]); 

            display.setCursor(printOptionHorizontalLocation(listOfOptions[peripheralOption]), (!topRow) );

            if (peripheralOption >= 0)
            {
                 display.print(listOfOptions[peripheralOption]); 
            }
        }

        void LCD_INTERFACE::printArrow()
        {
            prevRow = row;

            if (dialVal > prevDialVal)
            {
                  row = 0; //top row

                  upperRowOcurrences++; 

                  if (upperRowOcurrences == 2)
                  {
                      updateLCD = true; 

                      upperRowOcurrences = 0;
                  }
                  else
                  {
                      updateLCD = false;
                  }
                  
            }
            else if (prevDialVal > dialVal)
            {
                 row = 1;  // bottom row

                 bottomRowOcurrences++; 

                 if (bottomRowOcurrences == 2)
                 {
                     updateLCD = true;

                     bottomRowOcurrences = 0;
                 }
                 else
                 {
                     updateLCD = false;
                 }
                 
            }
    
            if (dialVal != prevDialVal)
            {
                display.clear();

                display.setCursor(0, prevRow);

                display.print(removeArrow);
  
                display.setCursor(0, row);

                display.print(arrow);
            }
        }       
        
        void LCD_INTERFACE::interface(String listOfOptions[])
        {
            prevDialVal = dialVal;
            dialVal = dial.encoderValue();  
        
            printArrow();

            printOptions(row, listOfOptions);
        }
