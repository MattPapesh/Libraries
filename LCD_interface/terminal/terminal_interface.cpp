#include <String.h>
#include <iostream>
#include <conio.h>

using namespace std; 

    struct ENCODER
    {
        int valueA, valueB, encoderVal, prevButtonPressed;
        bool execute, buttonPressed;
        public:
        int pinA,  pinB, digitalPin;
       // ENCODER(int tempPinA, int tempPinB, int tempDigitalPin){}
        int encoderValue(){return 0; }
        bool buttonPress(){ return false; }
    };

    struct folder
    {
        bool mainFolder = false; 
        char* folderName;
        char* parentFolder;
        char** listOfFoldersPTR = nullptr;
        int* sizeOfListOfFoldersPTR = nullptr;

        void createFolder(char* tempListOfFolders[], int tempSizeOfListOfFolders)
        {
            if (listOfFoldersPTR != nullptr){delete[] listOfFoldersPTR; }
            if (sizeOfListOfFoldersPTR != nullptr){delete sizeOfListOfFoldersPTR; }
            //Sense memory is constantly allocated on the heap, anything already
            //allocated needs to be freed before more memory is allocated. 

            listOfFoldersPTR = new char*[tempSizeOfListOfFolders];
            sizeOfListOfFoldersPTR = new int; 
            //pointers allocate memory inside of the method in case the std::char* array's size changes.
            *sizeOfListOfFoldersPTR = tempSizeOfListOfFolders;
            
            for (int i = 0; i < tempSizeOfListOfFolders; i++)
            {
                *(listOfFoldersPTR + i) = tempListOfFolders[i];
            }
        }

        ~folder()
        {
            delete[] listOfFoldersPTR;
            delete sizeOfListOfFoldersPTR; //any remaining allocated memory on the heap is freed. 
            listOfFoldersPTR = nullptr;
            sizeOfListOfFoldersPTR = nullptr;
        }
    };

    class LCD_INTERFACE
    {   
        private:// FOR LCD FUNCTIONALITY & MENU FUNCTIONALITY (allows menus to be navigated on the LCD with a dial encoder)

        int row, prevRow;
        bool foldersBeforeFunctions = true; 
        int const buttonDelayInMillis = 200; 
        char* arrow = "->";
        char* removeArrow = "  ";

        public: // PUBLIC VARIABLES FOR LCD SETUP AND MENU FUNCTIONALITY

         bool buttonPressed;
         char* BACK = "< BACK >";
         char* interfaceTitle = "**Main Menu**"; 
         int prevDialVal, dialVal;  
        // int LCD_resgisterSelectPin = A5, LCD_enablePin = A4, LCD_digitalPin0 = A2, LCD_digitalPin1 = 7, 
         //LCD_digitalPin2 = 8, LCD_digitalPin3 = A3, LCD_displayColoumns = 16, LCD_displayRows = 2,
         //dial_SW_Pin = 2, dial_CLK_Pin = A0, dial_DT_Pin = A1, numberOfFolders;  //A0 then A1
         char** listOfFoldersPTR; 
         int* sizeOfListOfFoldersPTR = new int(0); // index size   
         folder* currentFolderOpen; 
         char* currentFolderSelected;    
             
        private: // PRIVATE METHODS AND OBJECTS FOR LCD & MENU FUNCTIONALITY 

        //ENCODER dial = ENCODER(dial_CLK_Pin, dial_DT_Pin, dial_SW_Pin);
        ENCODER dial = ENCODER();
        //LiquidCrystal display = LiquidCrystal(LCD_resgisterSelectPin, LCD_enablePin, 
        //LCD_digitalPin0, LCD_digitalPin1, LCD_digitalPin2, LCD_digitalPin3);

      //  void display(char* output){printf("%s \n", output); }
        
        bool interfaceBlockade()
        {
            bool updateInterface;

            if (dialVal < 0){updateInterface = false; /*dial.encoderVal = 0;*/ }
            else if (dialVal == *sizeOfListOfFoldersPTR){updateInterface = false; /*dial.encoderVal = *sizeOfListOfFoldersPTR - 1;*/ }
            else{updateInterface = true; }

            return updateInterface;
        }

        int printOptionHorizontalLocation(char* tempFolder)
        {
            string tempFolderSTR = tempFolder;
            int tempHorizontalCursurPos;
             // finds the horizontal position of the cursur to center the printed text
            tempHorizontalCursurPos = (9 - (tempFolderSTR.length() /2) ); // equation centers option text

            return tempHorizontalCursurPos;
        }

        void printFolders(bool topRow, char* listOfFolders[])
        {
            if (interfaceBlockade() )
             {
                  int primaryOption, peripheralOption; 

                  if (topRow)
                  {
                    if (dialVal == 0){peripheralOption = 0; }
                    else{peripheralOption = dialVal - 1; }
                    primaryOption = dialVal;
                  }
                  else if (!topRow)
                  {
                    peripheralOption = dialVal + 1;
                    primaryOption = dialVal;
                  }

                //display.setCursor(printOptionHorizontalLocation(listOfFolders[primaryOption] ), bottomRow); 
                //display.print(listOfFolders[primaryOption]); 
                //display.setCursor(printOptionHorizontalLocation(listOfFolders[peripheralOption]), (!bottomRow) );
                //if (primaryOption != peripheralOption){display.print(listOfFolders[peripheralOption]); }
            }
        }

        void printArrow()
        {
            prevRow = row;

            if (dialVal > prevDialVal){row = 0; /*top row */ }
            else if (prevDialVal > dialVal){row = 1; /* bottom row */ }
    
            if (dialVal != prevDialVal)
            {
                //display.clear();
                //display.setCursor(0, prevRow);
                //display.print(removeArrow);
                //display.setCursor(0, row);
                //display.print(arrow);
            }
        }

        void updateDisplay(bool openFolder)
        {
            if (openFolder)
            { 
                for (int i = 0; i < *amountOfFolderObjPTRs; i++)
                {
                    if ( (*(PTR_ofListOfFolderObjPTRs + i) )->folderName == currentFolderSelected){currentFolderOpen = *(PTR_ofListOfFolderObjPTRs + i);
                    currentFolder(currentFolderOpen->listOfFoldersPTR, *(currentFolderOpen->sizeOfListOfFoldersPTR), currentFolderOpen); }
                }
            }
            else if (!openFolder)
            {
                for (int i = 0; i < *amountOfFolderObjPTRs; i++)
                {
                    if ( (*(PTR_ofListOfFolderObjPTRs + i) )->folderName ==  currentFolderOpen->parentFolder){currentFolderOpen = *(PTR_ofListOfFolderObjPTRs + i);
                    currentFolder(currentFolderOpen->listOfFoldersPTR, *(currentFolderOpen->sizeOfListOfFoldersPTR), currentFolderOpen); }
                }
            }
        }

        public:// PUBLIC METHODS FOR MENU FUNCTIONALITY       
    
        //LCD_INTERFACE(){display.begin(LCD_displayColoumns, LCD_displayRows); }

        ~LCD_INTERFACE()
        {
            delete[] PTR_ofListOfFolderObjPTRs;
            delete amountOfFolderObjPTRs; 
            //Freeing any remaining memory that was allocated on the heap

            PTR_ofListOfFolderObjPTRs = nullptr;
            amountOfFolderObjPTRs = nullptr;
        }   

        void begin(folder* tempMenuPTR)
        {
            currentFolderOpen = tempMenuPTR;
            tempMenuPTR->mainFolder = true; 
            *PTR_ofListOfFolderObjPTRs = tempMenuPTR; 
            
           currentFolder(tempMenuPTR->listOfFoldersPTR, *(tempMenuPTR->sizeOfListOfFoldersPTR), tempMenuPTR );
        }

        void openFolder()
        {
            if (buttonPressed)
            {   
                bool enter;
                if (currentFolderSelected == BACK){enter = false; }
                else{enter = true; }
                
               // delay(buttonDelayInMillis); 
                updateDisplay(enter);
            } 
            
            buttonPressed = false;
        }

        void interface()
        {
            prevDialVal = dialVal;
            dialVal = dial.encoderValue();
            currentFolderSelected = *(listOfFoldersPTR + dialVal); 
            buttonPressed = dial.buttonPress(); 

            openFolder();
            printArrow();
            printFolders(row, listOfFoldersPTR + dialVal);
            //delay(25);
        }

        private:// PRIVATE METHODS FOR ORGANIZING THE FOLDERS THAT MAKE UP MENUS

        void updateFolders(folder* tempMainFolderPTR, folder* tempNewFolderPTR)
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

        void currentFolder(char* tempListOfFolders[], int tempSize, folder* tempCurrentFolderOpen)
        {
            delete[] listOfFoldersPTR; 
            tempSize = tempSize + 1;
            listOfFoldersPTR = new char*[tempSize];

            for (int i = 1; i < tempSize; i++)
            {
                *(listOfFoldersPTR + i) = tempListOfFolders[i-1]; 
            }
            
            *sizeOfListOfFoldersPTR = tempSize;
            dial.encoderVal = tempSize; 
            
            if (currentFolderOpen->mainFolder){*(listOfFoldersPTR) = interfaceTitle; }
            else{*(listOfFoldersPTR) = BACK; }
        }
        
        public:// PUBLIC VARIABLES AND METHOD FOR ORGANIZING THE FOLDERS THAT MAKE UP MENUS

        folder** PTR_ofListOfFolderObjPTRs = nullptr;  // array of folder pointers (addresses to folders)
        int* amountOfFolderObjPTRs = new int(1); 

        void includeFolder(folder* tempParentFolderPTR, folder* tempChildFolderPTR, bool tempFoldersBeforeFunctions)
        {
            foldersBeforeFunctions = tempFoldersBeforeFunctions; 
            updateFolders(nullptr, tempChildFolderPTR);
            char* tempListOfFolders[*(tempParentFolderPTR->sizeOfListOfFoldersPTR)];// creates clone of parent folder's options on the stack

            for (int i = 0; i < *(tempParentFolderPTR->sizeOfListOfFoldersPTR); i++)
            {
                tempListOfFolders[i] = *(tempParentFolderPTR->listOfFoldersPTR + i);//makes clone identical to parent folder's list of options
            }

            *(tempParentFolderPTR->sizeOfListOfFoldersPTR) = *(tempParentFolderPTR->sizeOfListOfFoldersPTR) + 1; // size of list increments by 1
            delete[] tempParentFolderPTR->listOfFoldersPTR;  // original parent folder's options are deleted (array deleted) // issue
            tempParentFolderPTR->listOfFoldersPTR = new char*[*(tempParentFolderPTR->sizeOfListOfFoldersPTR)]; // new list of options
            //is made for the parent folder based on the new size

            for (int i = 0; i < *(tempParentFolderPTR->sizeOfListOfFoldersPTR); i++)
            {
                if(foldersBeforeFunctions)
                {
                    if(i == 0){*(tempParentFolderPTR->listOfFoldersPTR ) = tempChildFolderPTR->folderName; }
                    else if(i < *(tempParentFolderPTR->sizeOfListOfFoldersPTR)){*(tempParentFolderPTR->listOfFoldersPTR + i) = tempListOfFolders[i-1]; }
                }
                else if(!foldersBeforeFunctions)
                {
                    if (i <= *(tempParentFolderPTR->sizeOfListOfFoldersPTR) - 2){*(tempParentFolderPTR->listOfFoldersPTR + i) = tempListOfFolders[i]; 
                    /* original list is copied to the new list, leaving//the new index empty.*/ }
                    else if (i == *(tempParentFolderPTR->sizeOfListOfFoldersPTR) - 1){*(tempParentFolderPTR->listOfFoldersPTR + i ) = tempChildFolderPTR->folderName; 
                    /* issue the new index is assigned in option with the name of the child folder. */ }
                }
            }
            
            tempChildFolderPTR->parentFolder = tempParentFolderPTR->folderName; // the parent folder name of the child folder is named after the parent
           // we are not interested in the parent's parent, or the child's child, but strictly the relationship between the current child and the current parent. 
        }
    };

    int main()
    {
        std::cout << "hello world!";
        return 0; 
    }
