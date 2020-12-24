#ifndef LCD_INTERFACE_h
#define LCD_INTERFACE_h

#include <Encoder.h>
#include <LiquidCrystal.h>
#include <String.h>
#include "Arduino.h" 

    struct folder
    {
        bool mainFolder = false; 

        String folderName, parentFolder;
        
        String* listOfFoldersPTR = nullptr;
        int* sizeOfListOfFoldersPTR = nullptr;

        void createFolder(String tempListOfFolders[], int tempSizeOfListOfFolders);

        ~folder();
    };

 class LCD_INTERFACE
    {   
        private:// FOR LCD FUNCTIONALITY & MENU FUNCTIONALITY (allows menus to be navigated on the LCD with a dial encoder)

        int row, prevRow;
        
        bool foldersBeforeFunctions = true; 
        bool buttonPressed;
        int const buttonDelayInMillis = 200; 

        String arrow = "->", removeArrow = "  ";
        String BACK = "< BACK >";

        public: // PUBLIC VARIABLES FOR LCD SETUP AND MENU FUNCTIONALITY

         String interfaceTitle = "**Main Menu**"; 
        
         int prevDialVal, dialVal;  
        
         int LCD_resgisterSelectPin = 12, LCD_enablePin = 11, LCD_digitalPin0 = 5, LCD_digitalPin1 = 4, LCD_digitalPin2 = 8, LCD_digitalPin3 = 6,
         LCD_displayColoumns = 16, LCD_displayRows = 2,
         dial_SW_Pin = 2, dial_CLK_Pin = A0, dial_DT_Pin = A1, numberOfFolders;  

         String* listOfFoldersPTR; 
         int* sizeOfListOfFoldersPTR = new int; // index size   
         folder* currentFolderOpen; 

        String currentFolderSelected;    
             
        private: // PRIVATE METHODS AND OBJECTS FOR LCD & MENU FUNCTIONALITY 

        ENCODER dial = ENCODER(dial_CLK_Pin, dial_DT_Pin, dial_SW_Pin);

        LiquidCrystal display = LiquidCrystal(LCD_resgisterSelectPin, LCD_enablePin, LCD_digitalPin0, LCD_digitalPin1, LCD_digitalPin2, LCD_digitalPin3);
        
        bool interfaceBlockade();
        
        int printOptionHorizontalLocation(String tempFolder);

        void printFolders(bool topRow, String listOfFolders[]);

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

        void currentFolder(String tempListOfFolders[], int tempSize, folder* tempCurrentFolderOpen);
        
        public:// PUBLIC VARIABLES AND METHOD FOR ORGANIZING THE FOLDERS THAT MAKE UP MENUS

        folder** PTR_ofListOfFolderObjPTRs = nullptr;  // array of folder pointers (addresses to folders)

        int* amountOfFolderObjPTRs = new int(1); 
        
        void includeFolder(folder* tempParentFolderPTR, folder* tempChildFolderPTR, bool tempFoldersBeforeFunctions);
    };

#endif