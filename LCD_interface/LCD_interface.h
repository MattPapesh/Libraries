#ifndef LCD_INTERFACE_h
#define LCD_INTERFACE_h

#include <Encoder.h>
#include <LiquidCrystal.h>
#include <String.h>
#include "Arduino.h" 

    struct folder
    {
        bool mainFolder = false; 

        char* folderName;
        char* parentFolder;
        
        char** listOfFoldersPTR = nullptr;
        short int* sizeOfListOfFoldersPTR = nullptr;

        void createFolder(char* tempListOfFolders[], short int tempSizeOfListOfFolders);

        ~folder();
    };

 class LCD_INTERFACE
    {   
        private:// FOR LCD FUNCTIONALITY & MENU FUNCTIONALITY (allows menus to be navigated on the LCD with a dial encoder)

        short int row, prevRow;
        
        bool foldersBeforeFunctions = true; 
        
        short int const buttonDelayInMillis = 200; 

        char* arrow = "->";
        char* removeArrow = "  ";

        public: // PUBLIC VARIABLES FOR LCD SETUP AND MENU FUNCTIONALITY

         bool buttonPressed;

         char* BACK = "< BACK >";

         char* interfaceTitle = "**Main Menu**"; 
        
         int prevDialVal, dialVal;  
        
         int LCD_resgisterSelectPin = A5, LCD_enablePin = A4, LCD_digitalPin0 = A2, LCD_digitalPin1 = 7, LCD_digitalPin2 = 8, LCD_digitalPin3 = A3,
         LCD_displayColoumns = 16, LCD_displayRows = 2,
         dial_SW_Pin = 2, dial_CLK_Pin = A0, dial_DT_Pin = A1, numberOfFolders;  //A0 then A1

         char** listOfFoldersPTR; 
         short int* sizeOfListOfFoldersPTR = new short int; // index size   
         folder* currentFolderOpen; 

        char* currentFolderSelected;    
             
        private: // PRIVATE METHODS AND OBJECTS FOR LCD & MENU FUNCTIONALITY 

        ENCODER dial = ENCODER(dial_CLK_Pin, dial_DT_Pin, dial_SW_Pin);

        LiquidCrystal display = LiquidCrystal(LCD_resgisterSelectPin, LCD_enablePin, LCD_digitalPin0, LCD_digitalPin1, LCD_digitalPin2, LCD_digitalPin3);
        
        bool interfaceBlockade();
        
        short int printOptionHorizontalLocation(char* tempFolder);

        void printFolders(bool topRow, char* listOfFolders[]);

        void printArrow();

        void updateDisplay(bool openFolder);

        public:// PUBLIC METHODS FOR MENU FUNCTIONALITY       
    
        LCD_INTERFACE();

        ~LCD_INTERFACE();

        void begin(folder* tempMenuPTR);

        void openFolder();

        void interface();

        private:// PRIVATE METHODS FOR ORGANIZING THE FOLDERS THAT MAKE UP MENUS

        void updateFolders(folder* tempMainFolderPTR, folder* tempNewFolderPTR);

        void currentFolder(char* tempListOfFolders[], short int tempSize, folder* tempCurrentFolderOpen);
        
        public:// PUBLIC VARIABLES AND METHOD FOR ORGANIZING THE FOLDERS THAT MAKE UP MENUS

        folder** PTR_ofListOfFolderObjPTRs = nullptr;  // array of folder pointers (addresses to folders)

        short int* amountOfFolderObjPTRs = new short int(1); 
        
        void includeFolder(folder* tempParentFolderPTR, folder* tempChildFolderPTR, bool tempFoldersBeforeFunctions);
    };

#endif
