
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
                
            }

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
            PTR_ofListOfFolderObjPTRs = new folder*[3];
            *PTR_ofListOfFolderObjPTRs = tempMenuPTR; 
             
           currentFolder(tempMenuPTR->listOfFFsPTR, tempMenuPTR->sizeOfListOfFFs, tempMenuPTR );      
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

        void includeFolder(folder* tempParentFolderPTR, folder* tempChildFolderPTR, bool tempFoldersBeforeFunctions)
        {
            foldersBeforeFunctions = tempFoldersBeforeFunctions; 
            updateFolders(nullptr, tempChildFolderPTR);
            char* tempListOfFFs[(tempParentFolderPTR->sizeOfListOfFFs)] = {};// creates clone of parent folder's options on the stack

            for (int i = 0; i < (tempParentFolderPTR->sizeOfListOfFFs); i++)
            {
                tempListOfFFs[i] = *(tempParentFolderPTR->listOfFFsPTR + i);//makes clone identical to parent folder's list of options
            }

            tempParentFolderPTR->sizeOfListOfFFs++; // size of list increments by 1
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
                            *(tempParentFolderPTR->listOfFFsPTR + i) = tempListOfFFs[i-1];
                     }
                }
                else if(!foldersBeforeFunctions)
                {
                    if (i <= tempParentFolderPTR->sizeOfListOfFFs - 2)
                    {
                        *(tempParentFolderPTR->listOfFFsPTR + i) = tempListOfFFs[i]; // original list is copied to the new list, leaving
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

    const int no_params = 3; 
    param p[no_params] = {param("p1",0), param("p2",1), param("p3",2)};

    const int no_files_1 = 3; 
    file files_1[no_files_1] = {file("file_1", p, 0), file("file_2", p, 0), file("file_3", p, 0)};
    folder folder_1 = folder("myFirstFolder", files_1, no_files_1);
    folder folder_2 = folder("mySecondFolder", files_1, no_files_1);

    void setup()
    { 
        interface.includeFolder(&folder_1, &folder_2, false); 
        interface.begin(&folder_1);
    }
    
    void print()
    {
        folder current_folder = folder("", NULL, 0); 
        folder folders[9] = {current_folder, current_folder, current_folder, current_folder, current_folder, current_folder, current_folder, current_folder, current_folder};

        for(int i = 0; i < interface.amountOfFolderObjPTRs; i++)
        {   
            folders[i] = *((*interface.PTR_ofListOfFolderObjPTRs) + i);
        }

        for(int i = 0; i < interface.amountOfFolderObjPTRs; i++)
        {
            const int num_of_files = current_folder.sizeOfListOfFiles;
            file files[num_of_files];

            current_folder = folders[i]; 
            printf("folder name:  %s \n", current_folder.folderName);
            printf("child folders: "); 

            for(int ii = 0; ii < num_of_folders; ii++)
            {
                if(ii != 0)
                {
                    printf(", "); 
                }

                if(folders[ii].parentFolder == current_folder.folderName)
                {
                    printf("%s", folders[ii].folderName);
                }
            }
            printf("files: %i", num_of_files);
            printf("\nfiles & parameters below: \n\n"); 

            for(int ii = 0; ii < num_of_files; ii++)
            {   
                files[ii] = *(current_folder.listOfFilesPTR + ii);
            }

            for(int ii = 0; ii < num_of_files; ii++)
            {
                file current_file = files[ii];
                printf("file #%i: %s -> parameters: ", ii, current_file.fileName);

                for(int iii = 0; iii < current_file.numOfParams; iii++)
                {
                    if(iii != 0)
                    {
                        printf(", ");
                    }

                    printf("%s", *(current_file.paramsPTR + iii));
                }

                printf("\n");
            }
            
            printf("\n\n");
        }
    }

    int main()
    {  
        setup(); 
        print(); 
        return 0; 
    }