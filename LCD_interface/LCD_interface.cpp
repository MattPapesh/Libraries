#include <Encoder.h>
#include <LiquidCrystal.h>
#include <String.h>
#include <LCD_interface.h>
#include "Arduino.h"
 
          LCD_INTERFACE::LCD_INTERFACE()
          {     
            display.begin(LCD_displayColoumns, LCD_displayRows);  
          }

          LCD_INTERFACE::~LCD_INTERFACE()
          {
            delete[] PTR_ofListOfFolderObjPTRs;
            delete amountOfFolderObjPTRs; 
            //Freeing any remaining memory that was allocated on the heap

            PTR_ofListOfFolderObjPTRs = nullptr;
            amountOfFolderObjPTRs = nullptr; 
          }

          void LCD_INTERFACE::currentFolder(String tempListOfFolders[], int tempSize, folder* tempCurrentFolderOpen)
          {
               delete[] listOfFoldersPTR; 

               tempSize = tempSize + 1;

               listOfFoldersPTR = new String[tempSize];

            for (int i = 1; i < tempSize; i++)
            {
                *(listOfFoldersPTR + i) = tempListOfFolders[i-1]; 
            }
            
            *sizeOfListOfFoldersPTR = tempSize;

            dial.encoderVal = tempSize; 
            
            if (currentFolderOpen->mainFolder)
            {
                 *(listOfFoldersPTR) = interfaceTitle; 
            }
            else
            {
                *(listOfFoldersPTR) = BACK;
            }

            
          }

        void LCD_INTERFACE::begin(folder* tempMenuPTR)
        {
            currentFolderOpen = tempMenuPTR;

            tempMenuPTR->mainFolder = true; 

            *PTR_ofListOfFolderObjPTRs = tempMenuPTR; 
            
           currentFolder(tempMenuPTR->listOfFoldersPTR, *(tempMenuPTR->sizeOfListOfFoldersPTR), tempMenuPTR );      
        }

        bool LCD_INTERFACE::interfaceBlockade()
        {
            bool updateInterface;

            if (dialVal < 0)
            {
                updateInterface = false;

                dial.encoderVal = 0;
            }
            else if (dialVal == *sizeOfListOfFoldersPTR)
            {
                updateInterface = false;

                dial.encoderVal = *sizeOfListOfFoldersPTR - 1; 
            }
            else
            {
                updateInterface = true; 
            }

            return updateInterface;    
        }

        int LCD_INTERFACE::printOptionHorizontalLocation(String tempFolder)
        {
            int tempHorizontalCursurPos;
             // finds the horizontal position of the cursur to center the printed text
    
            tempHorizontalCursurPos = (9 - (tempFolder.length() /2) ); // equation centers option text

            return tempHorizontalCursurPos;
        }

        void LCD_INTERFACE::printFolders(bool bottomRow, String listOfFolders[])
        { 
             if (interfaceBlockade() )
             {
                  int primaryOption, peripheralOption; 

                  if (!bottomRow)
                  {
                      if (dialVal == 0)
                      {
                          peripheralOption = 0;
                      }
                      else
                      {
                          peripheralOption = dialVal - 1; 
                      }
                
                      primaryOption = dialVal;
                  }
                  else if (bottomRow)
                  {
                     peripheralOption = dialVal + 1;
                     primaryOption = dialVal;
                  }

                  display.setCursor(printOptionHorizontalLocation(listOfFolders[primaryOption] ), bottomRow); 

                  display.print(listOfFolders[primaryOption]); 

                  display.setCursor(printOptionHorizontalLocation(listOfFolders[peripheralOption]), (!bottomRow) );
    
                 if (primaryOption != peripheralOption)
                 {
                     display.print(listOfFolders[peripheralOption]);
                 }
            }
        }

        void LCD_INTERFACE::printArrow()
        {
            prevRow = row;

            if (dialVal > prevDialVal)
            {
                  row = 0; //top row
            }
            else if (prevDialVal > dialVal)
            {
                 row = 1;  // bottom row
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

        void LCD_INTERFACE::interface()// runs the current menu (displays current parent folder)
        { 
            String listOfFolders[*sizeOfListOfFoldersPTR];

            for (int i = 0; i < *sizeOfListOfFoldersPTR; i++)
            {
                listOfFolders[i] = *(listOfFoldersPTR + *sizeOfListOfFoldersPTR - i - 1);
            }

            prevDialVal = dialVal;
            dialVal = dial.encoderValue();

            currentFolderSelected = listOfFolders[dialVal]; 
            
            buttonPressed = dial.buttonPress(); 

            openFolder();
        
            printArrow();
            
            printFolders(row, listOfFolders);
        }

        void LCD_INTERFACE::updateFolders(folder* tempMainFolderPTR, folder* tempNewFolderPTR)
        {
           folder** tempPTR_ofListOfFolderObjPTRs = new folder*[*amountOfFolderObjPTRs];
          
           if (PTR_ofListOfFolderObjPTRs != nullptr)
            {
                 for (int i = 0; i < *amountOfFolderObjPTRs; i++)
                {
                    *(tempPTR_ofListOfFolderObjPTRs + i) = *(PTR_ofListOfFolderObjPTRs + i);
                }

                delete[] PTR_ofListOfFolderObjPTRs;
            }
           
           *amountOfFolderObjPTRs = *amountOfFolderObjPTRs + 1;

           PTR_ofListOfFolderObjPTRs = new folder*[*amountOfFolderObjPTRs]; 

           for (int i = 0; i < *amountOfFolderObjPTRs - 1; i++)
           {
               *(PTR_ofListOfFolderObjPTRs + i) = *(tempPTR_ofListOfFolderObjPTRs + i);
           }

           delete[]  tempPTR_ofListOfFolderObjPTRs;

           *(PTR_ofListOfFolderObjPTRs + *amountOfFolderObjPTRs - 1) = tempNewFolderPTR; 
        }

        void LCD_INTERFACE::updateDisplay(bool openFolder)
        {
            if (openFolder)
            { 
                for (int i = 0; i < *amountOfFolderObjPTRs; i++)
                {
                    if ( (*(PTR_ofListOfFolderObjPTRs + i) )->folderName == currentFolderSelected)
                    { 
                        currentFolderOpen =  *(PTR_ofListOfFolderObjPTRs + i) ;
                        
                        currentFolder(currentFolderOpen->listOfFoldersPTR, *(currentFolderOpen->sizeOfListOfFoldersPTR), currentFolderOpen);
                    }
                }
            }
            else if (!openFolder)
            {
                for (int i = 0; i < *amountOfFolderObjPTRs; i++)
                {
                    if ( (*(PTR_ofListOfFolderObjPTRs + i) )->folderName ==  currentFolderOpen->parentFolder)
                    {
                        currentFolderOpen = *(PTR_ofListOfFolderObjPTRs + i);
                        
                        currentFolder(currentFolderOpen->listOfFoldersPTR, *(currentFolderOpen->sizeOfListOfFoldersPTR), currentFolderOpen);
                    }
                }
            }
        }

        void LCD_INTERFACE::openFolder()
        {
            bool enter;

            if (buttonPressed)
            {
                delay(buttonDelayInMillis); 
                
                  if (currentFolderSelected == BACK)
                  {  
                     enter = false;
                  }
                  else
                  {
                    enter = true; 
                 }
                
                updateDisplay(enter);
               
            } buttonPressed = false; 
        }

        void LCD_INTERFACE::includeFolder(folder* tempParentFolderPTR, folder* tempChildFolderPTR, bool tempFoldersBeforeFunctions)
        {
            foldersBeforeFunctions = tempFoldersBeforeFunctions; 

            updateFolders(nullptr, tempChildFolderPTR);
            
            String tempListOfFolders[*(tempParentFolderPTR->sizeOfListOfFoldersPTR)];// creates clone of parent folder's options on the stack

            for (int i = 0; i < *(tempParentFolderPTR->sizeOfListOfFoldersPTR); i++)
            {
                tempListOfFolders[i] = *(tempParentFolderPTR->listOfFoldersPTR + i);//makes clone identical to parent folder's list of options
            }

            *(tempParentFolderPTR->sizeOfListOfFoldersPTR) = *(tempParentFolderPTR->sizeOfListOfFoldersPTR) + 1; // size of list increments by 1
            
            delete[] tempParentFolderPTR->listOfFoldersPTR;  // original parent folder's options are deleted (array deleted) // issue

            tempParentFolderPTR->listOfFoldersPTR = new String[*(tempParentFolderPTR->sizeOfListOfFoldersPTR)]; // new list of options
            //is made for the parent folder based on the new size

            for (int i = 0; i < *(tempParentFolderPTR->sizeOfListOfFoldersPTR); i++)
            {
                if(foldersBeforeFunctions)
                {
                    if(i == 0)
                     {
                            *(tempParentFolderPTR->listOfFoldersPTR ) = tempChildFolderPTR->folderName;
                     }
                     else if(i < *(tempParentFolderPTR->sizeOfListOfFoldersPTR))
                     {
                            *(tempParentFolderPTR->listOfFoldersPTR + i) = tempListOfFolders[i-1];
                     }
                }
                else if(!foldersBeforeFunctions)
                {
                    if (i <= *(tempParentFolderPTR->sizeOfListOfFoldersPTR) - 2)
                    {
                        *(tempParentFolderPTR->listOfFoldersPTR + i) = tempListOfFolders[i]; // original list is copied to the new list, leaving
                         //the new index empty.
                    }
                    else if (i == *(tempParentFolderPTR->sizeOfListOfFoldersPTR) - 1)
                    {        
                        *(tempParentFolderPTR->listOfFoldersPTR + i ) = tempChildFolderPTR->folderName; //issue
                        //the new index is assigned in option with the name of the child folder. 
                    }
                }
            }
            
             tempChildFolderPTR->parentFolder = tempParentFolderPTR->folderName; // the parent folder name of the child folder is named after the parent
           // we are not interested in the parent's parent, or the child's child, but strictly the relationship between the current child and the current parent. 
        }

        folder::~folder()
        {
            delete[] listOfFoldersPTR;
            delete sizeOfListOfFoldersPTR; //any remaining allocated memory on the heap is freed. 

            listOfFoldersPTR = nullptr;
            sizeOfListOfFoldersPTR = nullptr; 
        }

        void folder::createFolder(String tempListOfFolders[], int tempSizeOfListOfFolders)
        {
            if (listOfFoldersPTR != nullptr)
            {
                delete[] listOfFoldersPTR;
            }

            if (sizeOfListOfFoldersPTR != nullptr)
            {
                delete sizeOfListOfFoldersPTR;
            }
            // Sense memory is constantly allocated on the heap, anything already
            //allocated needs to be freed before more memory is allocated. 

            listOfFoldersPTR = new String[tempSizeOfListOfFolders];
            sizeOfListOfFoldersPTR = new int; // pointers allocate memory inside of the method in case the std::string array's size changes.

            *sizeOfListOfFoldersPTR = tempSizeOfListOfFolders;
            
            for (int i = 0; i < tempSizeOfListOfFolders; i++)
            {
                *(listOfFoldersPTR + i) = tempListOfFolders[i];
            }
        }
