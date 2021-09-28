#include <String.h>
#include <iostream>
#include <conio.h>

    struct param
    {
        char* paramName = "";
        int parameter = 0;

        param(char* tempParamName, int tempInitialParam)
            : paramName(tempParamName), parameter(tempInitialParam) {}
        param(char* tempParamName)
            : paramName(tempParamName), parameter(0) {}
        param() {}
    };

    struct file
    {
        bool redundant = false;

        char* fileName = "";
        param* paramsPTR = nullptr;
        int numOfParams = 0;

        file(char* tempFileName, param* tempParams, int tempNumOfParams)
            : redundant(false), fileName(tempFileName), paramsPTR(tempParams), numOfParams(tempNumOfParams)
        {
            for (int i = 0; i < tempNumOfParams; i++)
            {
                paramsPTR = tempParams; 
            }
        }

        file(char* tempFileName)
            : redundant(true), fileName(tempFileName), paramsPTR(nullptr), numOfParams(0) {}
        file() {}

        ~file()
        {
            delete[] paramsPTR;
            paramsPTR = nullptr; 
        }
    };

    struct folder
    {
        bool mainFolder = false; 

        char* folderName;
        char* parentFolder;
        
        char** listOfFFsPTR = nullptr;
        int sizeOfListOfFFs = 0;
        file* listOfFilesPTR = nullptr;
        int sizeOfListOfFiles = 0; 

        folder(char* tempFolderName, file tempListOfFiles[], int tempSizeOfListOfFiles)
            : folderName(tempFolderName), listOfFFsPTR(new char*[tempSizeOfListOfFiles]), sizeOfListOfFFs(tempSizeOfListOfFiles)              
        {            
            int filesToDelete = 0; listOfFilesPTR = new file[tempSizeOfListOfFiles]; sizeOfListOfFiles = tempSizeOfListOfFiles;

            for (int i = 0; i < tempSizeOfListOfFiles; i++)
            {
                *(listOfFFsPTR + i) = tempListOfFiles[i].fileName; *(listOfFilesPTR + i) = tempListOfFiles[i]; 
                 
                if (tempListOfFiles[i].redundant)
                {
                    filesToDelete++; 
                }
                
            }/*

            if (filesToDelete < tempSizeOfListOfFiles)
            {
                 sizeOfListOfFiles = tempSizeOfListOfFiles - filesToDelete; 
                 listOfFilesPTR = new file[tempSizeOfListOfFiles - filesToDelete];

                for (int i = 0, tempListOffset = 0; i + tempListOffset < tempSizeOfListOfFiles; i++)
                {
                    while (tempListOfFiles[i + tempListOffset].redundant)
                    {
                        tempListOffset++;
                    }

                    *(listOfFilesPTR + i) = tempListOfFiles[i + tempListOffset]; 
                } 
            }

            Serial.print("Folder: "); Serial.println(folderName); 
            for (int i = 0; i < sizeOfListOfFiles; i++){Serial.print("FileObj: "); Serial.println((listOfFilesPTR + i)->fileName);
            for (int I = 0; I < (listOfFilesPTR + i)->numOfParams; I++){Serial.print("Param: "); Serial.print(((listOfFilesPTR+i)->paramsPTR + I)->paramName); Serial.print(" = "); Serial.println(((listOfFilesPTR+i)->paramsPTR + I)->param);} }
            Serial.println("");  */    
        }

        ~folder()
        {
            delete[] listOfFFsPTR; //any remaining allocated memory on the heap is freed. 
            delete[] listOfFilesPTR;
            listOfFilesPTR = nullptr; 
            listOfFFsPTR = nullptr; 
        }
    };

    class LCD_INTERFACE
    {
        private:// FOR LCD FUNCTIONALITY & MENU FUNCTIONALITY (allows menus to be navigated on the LCD with a dial encoder)

        short int row, prevRow;
        
        bool foldersBeforeFunctions = true;
        bool switchInterface = false;
        bool enableInterface = false; 
        
        short int const buttonDelayInMillis = 125; 

        char* arrow = "->";
        char* const primaryArrow = "->";
        char* const secondaryArrow = ">>";
        char* const removeArrow = "  ";

        public: // PUBLIC VARIABLES FOR LCD SETUP AND MENU FUNCTIONALITY

         bool buttonPressed;
         bool buttonPressedTwice; 
         bool navInterface = true; // If navigating the interface then the primaryInterface() must be getting called

         char* BACK = "< BACK >";
         char* interfaceTitle = "**Main Menu**"; 
        
         int prevDialVal, dialVal;  
        
        // int LCD_resgisterSelectPin = A5, LCD_enablePin = A4, LCD_digitalPin0 = A2, LCD_digitalPin1 = 7, LCD_digitalPin2 = 8, LCD_digitalPin3 = A3,
        // LCD_displayColoumns = 16, LCD_displayRows = 2,
        // dial_SW_Pin = 2, dial_CLK_Pin = A0, dial_DT_Pin = A1, numberOfFolders;  //A0 then A1

         char** listOfFFsPTR = nullptr; 
         int sizeOfListOfFFs = 0; // index size   
         
         folder* currentFolderOpen; 
         char* currentFFSelected;

         folder** PTR_ofListOfFolderObjPTRs = nullptr;// dynamically allocated array of folder pointers (addresses to folders)
         int amountOfFolderObjPTRs = 1;

         public:

          LCD_INTERFACE()
          {     
               //display.begin(LCD_displayColoumns, LCD_displayRows);  
          }

          ~LCD_INTERFACE()
          {
            delete[] PTR_ofListOfFolderObjPTRs;
            //Freeing any remaining memory that was allocated on the heap
            PTR_ofListOfFolderObjPTRs = nullptr;
          }

          void currentFolder(char* tempListOfFFs[], int tempSize, folder* tempCurrentFolderOpen)
          {
               delete[] listOfFFsPTR; 
               tempSize = tempSize + 1;
               listOfFFsPTR = new char*[tempSize];

            for (int i = 1; i < tempSize; i++)
            {
                *(listOfFFsPTR + i) = tempListOfFFs[i-1]; 
            }
            
            sizeOfListOfFFs = tempSize;
            //dial.encoderVal = tempSize; 
            
            if (currentFolderOpen->mainFolder)
            {
                 *(listOfFFsPTR) = interfaceTitle; 
            }
            else
            {
                *(listOfFFsPTR) = BACK;
            }
          }

        void begin(folder* tempMenuPTR)
        {
            currentFolderOpen = tempMenuPTR;
            tempMenuPTR->mainFolder = true; 
            *PTR_ofListOfFolderObjPTRs = tempMenuPTR; 
            
           currentFolder(tempMenuPTR->listOfFFsPTR, tempMenuPTR->sizeOfListOfFFs, tempMenuPTR );      
        }

        bool interfaceBlockade()
        {
            bool updateInterface;

            if (dialVal < 0)
            {
                updateInterface = false;
                //dial.encoderVal = 0;
            }
            else if (dialVal == sizeOfListOfFFs)
            {
                updateInterface = false;
                //dial.encoderVal = sizeOfListOfFFs - 1; 
            }
            else
            {
                updateInterface = true; 
            }

            return updateInterface;    
        }

        short int printOptionHorizontalLocation(char* tempFolder)
        {
            //String tempFolderSTR = tempFolder;
            short int tempHorizontalCursurPos;
             // finds the horizontal position of the cursur to center the printed text
    
            //tempHorizontalCursurPos = (9 - (tempFolderSTR.length() /2) ); // equation centers option text

            return 0;//tempHorizontalCursurPos;
        }

        void printFolders(bool bottomRow, char* listOfFolders[])
        { 
             if (interfaceBlockade() )
             {
                  short int primaryOption, peripheralOption; 

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

                  //display.setCursor(printOptionHorizontalLocation(listOfFolders[primaryOption] ), bottomRow); 
                  //display.print(listOfFolders[primaryOption]); 
                  //display.setCursor(printOptionHorizontalLocation(listOfFolders[peripheralOption]), (!bottomRow) );
    
                 if (primaryOption != peripheralOption)
                 {
                    //display.print(listOfFolders[peripheralOption]);
                 }
            }
        }

        void printArrow()
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
                //display.clear();
                //display.setCursor(0, prevRow);
                //display.print(removeArrow);
                //display.setCursor(0, row);
                //display.print(arrow);
            }
        }

        void primaryInterface()
        {
            arrow = primaryArrow;
         
            char* listOfFolders[sizeOfListOfFFs];

            for (int i = 0; i < sizeOfListOfFFs; i++)
            {
                listOfFolders[i] = *(listOfFFsPTR + sizeOfListOfFFs - i - 1);
            }

             currentFFSelected = listOfFolders[dialVal];

            if (enableInterface && !buttonPressed)
            {
                enableInterface = false;

                //display.setCursor(0, row);
                //display.print(arrow);
            }

            if (!enableInterface)
            {
                openFolder(); 
                printArrow();
                printFolders(row, listOfFolders);
            }   
        }

        void secondaryInterface() 
        {
            arrow = secondaryArrow;
            dialVal = 0; 

            if (buttonPressed || navInterface)
            {
                //display.clear(); 
                
            }

           for (int i = 0; i < amountOfFolderObjPTRs; i++)
           {
               //Serial.print("FolderObj: "); Serial.println((*(PTR_ofListOfFolderObjPTRs + i))->folderName);

                for (int iter = 0; iter < (*(PTR_ofListOfFolderObjPTRs+i))->sizeOfListOfFiles; iter++)
                {
                    //Serial.print("FileObj: "); 
                    //Serial.println(((*(PTR_ofListOfFolderObjPTRs+i))->listOfFilesPTR + iter)->fileName);

                    for (int I = 0; I < ((*(PTR_ofListOfFolderObjPTRs+i))->listOfFilesPTR + iter)->numOfParams; I++)
                    {
                        //Serial.print("Param: "); 
                        //Serial.print((((*(PTR_ofListOfFolderObjPTRs + i))->listOfFilesPTR + iter)->paramsPTR + I)->paramName); Serial.print(" = "); 
                        //Serial.println((((*(PTR_ofListOfFolderObjPTRs+i))->listOfFilesPTR+iter)->paramsPTR + I)->parameter);
                    } 
                } //Serial.println("");  
           }
        }   

        void interface()// runs the current menu (displays current parent folder)
        { 
            getButtonStatus(&buttonPressed, &buttonPressedTwice, buttonDelayInMillis);

            prevDialVal = dialVal;
            //dialVal = dial.encoderValue();

            if (buttonPressedTwice)
            {
                switchInterface = (!switchInterface);
                enableInterface = true;
                //display.clear();  
            }
            if (!switchInterface)
            {
                navInterface = true;
                primaryInterface();
            }
            else
            {   
                secondaryInterface(); 
                navInterface = false;
            }
        }

        void getButtonStatus(bool* pressedOnce, bool* pressedTwice, int delayInMillis)
        {
            *pressedOnce = false;
            *pressedTwice = false;
            //*pressedOnce = dial.buttonPress();
            
            if (*pressedOnce)
            {
                for (int i = 0; i < delayInMillis; i++)
                {
                    //delay(1);

                    if (false/*!dial.buttonPress()*/ )
                    {
                        for(int I = 0; I < delayInMillis; I++)
                        {   
                            //delay(1);

                            //if(dial.buttonPress() )
                            {
                                *pressedTwice = true;
                                *pressedOnce = false;

                                //delay(delayInMillis);

                                break;
                            }
                        }

                        break;
                    }
                }
            }
        }

        void updateFolders(folder* tempMainFolderPTR, folder* tempNewFolderPTR)
        {
           folder** tempPTR_ofListOfFolderObjPTRs = new folder*[amountOfFolderObjPTRs];
          
           if (PTR_ofListOfFolderObjPTRs != nullptr)
            {
                 for (short int i = 0; i < amountOfFolderObjPTRs; i++)
                {
                    *(tempPTR_ofListOfFolderObjPTRs + i) = *(PTR_ofListOfFolderObjPTRs + i);
                }

                delete[] PTR_ofListOfFolderObjPTRs;
            }
           
           amountOfFolderObjPTRs = amountOfFolderObjPTRs + 1;
           PTR_ofListOfFolderObjPTRs = new folder*[amountOfFolderObjPTRs]; 

           for (short int i = 0; i < amountOfFolderObjPTRs - 1; i++)
           {
               *(PTR_ofListOfFolderObjPTRs + i) = *(tempPTR_ofListOfFolderObjPTRs + i);
           }

           delete[]  tempPTR_ofListOfFolderObjPTRs;
           *(PTR_ofListOfFolderObjPTRs + amountOfFolderObjPTRs - 1) = tempNewFolderPTR; 
        }

        void updateDisplay(bool openFolder)
        {
            if (openFolder)
            { 
                for (short int i = 0; i < amountOfFolderObjPTRs; i++)
                {
                    if ( (*(PTR_ofListOfFolderObjPTRs + i) )->folderName == currentFFSelected)
                    { 
                        currentFolderOpen =  *(PTR_ofListOfFolderObjPTRs + i) ;
                        currentFolder(currentFolderOpen->listOfFFsPTR, currentFolderOpen->sizeOfListOfFFs, currentFolderOpen);
                    }
                }
            }
            else if (!openFolder)
            {
                for (short int i = 0; i < amountOfFolderObjPTRs; i++)
                {
                    if ( (*(PTR_ofListOfFolderObjPTRs + i) )->folderName ==  currentFolderOpen->parentFolder)
                    {
                        currentFolderOpen = *(PTR_ofListOfFolderObjPTRs + i);
                        currentFolder(currentFolderOpen->listOfFFsPTR, currentFolderOpen->sizeOfListOfFFs, currentFolderOpen);
                    }
                }
            }
        }

        void openFolder()
        {
            bool enter;

            if (buttonPressed)
            {   
                  if (currentFFSelected == BACK)
                  {  
                     enter = false;
                  }
                  else
                  {
                     enter = true; 
                  }
                
                updateDisplay(enter);
            }  
        }
/*
        int getParam(char* fileName, char* paramName)
        {   
            for (int currentFolder = 0; currentFolder < amountOfFolderObjPTRs; currentFolder++)
            {
                folder* currentFolderPTR = (*(PTR_ofListOfFolderObjPTRs + currentFolder));
                
                for (int currentFile = 0; currentFile < currentFolderPTR->sizeOfListOfFiles; currentFile++)
                {
                    file* currentFilePTR = (currentFolderPTR->listOfFilesPTR + currentFile);

                    if (currentFilePTR->fileName == fileName)
                    {
                        for (int currentParam = 0; currentParam < currentFilePTR->numOfParams; currentParam++)
                        {
                            param* currentParamPTR = (currentFilePTR->paramsPTR + currentParam);

                            if (currentParamPTR->paramName == paramName)
                            {
                                return currentParamPTR->parameter; 
                            }
                        }
                    } 
                }
            }

            return 0; 
        }*/

        bool getButtonPressed()
        {
            return buttonPressed; 
        }

        int getDialValue()
        {
            return dialVal;
        }

        void includeFolder(folder* tempParentFolderPTR, folder* tempChildFolderPTR, bool tempFoldersBeforeFunctions)
        {
            foldersBeforeFunctions = tempFoldersBeforeFunctions; 

            updateFolders(nullptr, tempChildFolderPTR);
            
            char* tempListOfFolders[(tempParentFolderPTR->sizeOfListOfFFs)];// creates clone of parent folder's options on the stack

            for (int i = 0; i < (tempParentFolderPTR->sizeOfListOfFFs); i++)
            {
                tempListOfFolders[i] = *(tempParentFolderPTR->listOfFFsPTR + i);//makes clone identical to parent folder's list of options
            }

            tempParentFolderPTR->sizeOfListOfFFs = tempParentFolderPTR->sizeOfListOfFFs + 1; // size of list increments by 1
            
            delete[] tempParentFolderPTR->listOfFFsPTR;  // original parent folder's options are deleted (array deleted)

            tempParentFolderPTR->listOfFFsPTR = new char*[tempParentFolderPTR->sizeOfListOfFFs]; // new list of options
            //is made for the parent folder based on the new size

            for (int i = 0; i < tempParentFolderPTR->sizeOfListOfFFs; i++)
            {
                if(foldersBeforeFunctions)
                {
                    if(i == 0)
                     {
                            *(tempParentFolderPTR->listOfFFsPTR ) = tempChildFolderPTR->folderName;
                     }
                     else if(i < tempParentFolderPTR->sizeOfListOfFFs)
                     {
                            *(tempParentFolderPTR->listOfFFsPTR + i) = tempListOfFolders[i-1];
                     }
                }
                else if(!foldersBeforeFunctions)
                {
                    if (i <= tempParentFolderPTR->sizeOfListOfFFs - 2)
                    {
                        *(tempParentFolderPTR->listOfFFsPTR + i) = tempListOfFolders[i]; // original list is copied to the new list, leaving
                         //the new index empty.
                    }
                    else if (i == tempParentFolderPTR->sizeOfListOfFFs - 1)
                    {        
                        *(tempParentFolderPTR->listOfFFsPTR + i ) = tempChildFolderPTR->folderName; //issue
                        //the new index is assigned in option with the name of the child folder. 
                    }
                }
            }
            
             tempChildFolderPTR->parentFolder = tempParentFolderPTR->folderName; // the parent folder name of the child folder is named after the parent
           // we are not interested in the parent's parent, or the child's child, but strictly the relationship between the current child and the current parent. 
        }
    };

    LCD_INTERFACE interface = LCD_INTERFACE();

    const int no_files_1 = 3; 
    file files_1[no_files_1] = {file("file_1", nullptr, 0), file("file_2", nullptr, 0), file("file_3", nullptr, 0)};
    folder folder_1 = folder("myFirstFolder", files_1, no_files_1);

    void setup()
    {
        interface.begin(&folder_1);
    }

    void print()
    {
        for(int i = 0; i < interface.amountOfFolderObjPTRs; i++)
        {
            folder current_folder = *(*(interface.PTR_ofListOfFolderObjPTRs) + i);

            printf("folder name:  %s", current_folder.folderName);
        }
    }

    int main()
    {  
        setup(); 
        print(); 
        return 0; 
    }
