#ifndef LCD_Interface_h
#define LCD_Interface_h

#include "Arduino.h"
#include <Encoder.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>

    struct parameter // A file parameter that has a name and some kind of data associated with it
    {
        private:

        char* parameter_name = "";
        double data = 0;

        public:

        parameter();
        parameter(char* parameter_name, double data);
        char* getParameterName();
        double getData();
        void setParameterName(char* parameter_name);
        void setData(double data);
    };

    struct file // A "file", where it has a name, and an address to an array of dynamically allocated parameters; a file can have multiple parameters
    {
        private:

        char* file_name = "";
        parameter* parameter_ptrs = nullptr;
        int num_of_parameter_ptrs = 0;
        bool is_active = false;

        public:

        file();
        file(char* file_name);
        ~file();
        void addParameter(char* parameter_name, double data);
        char* getFileName();
        void setFileActive(bool is_active);
        bool getFileActive(); 
        parameter* getParameterPTRs();
        int getNumOfParameters();
    };

    struct folder // A "folder", where it has a name, and can store other folders and files that are both dynamically allocated.
    {
        private:

        char* folder_name = "";
        char* parent_folder_name = "";
        folder* parent_folder_ptr = nullptr;
        file** file_ptr_ptrs = nullptr;
        int num_of_file_ptrs = 0;
        folder** folder_ptr_ptrs = nullptr;
        int num_of_folder_ptrs = 0;

        public:

        folder();
        folder(char* folder_name);
        ~folder();
        void addFolders(folder* folders[], int num_of_folders);
        void addFiles(file* files[], int num_of_files);
        folder* getFolderPTR(int index);
        char* getFolderName();
        int getNumOfFiles();
        int getNumOfFolders();
        void setParentFolderName(char* parent_folder_name);
        void setParentFolderPTR(folder* parent_folder_ptr);
        folder* getParentFolderPTR();
        file* getFilePTR(int index);
        char** getContentNamePTRs(bool folders_before_files);
    };

    class LCD_Interface // An interface to navigate the "folder-file" data structure -> will be prepared to eventually interface older code!
    {
        private:

        folder* base_folder_ptr = nullptr;
        folder* current_folder_open_ptr = nullptr;
        file* current_file_open_ptr = nullptr;
        char* current_content_selected = "";

        folder* getFolderPTR(folder* current_folder_ptr, char* folder_name);
        folder* getFolderPTR(char* folder_name); // set equal to the current_folder_selected

        int row, prev_row;
        bool folders_before_functions = true; 
        int const PERIODIC_DELAY = 25, BUTTON_COOL_DOWN = 200; 
        char* arrow = "->";
        char* remove_arrow = "  ";
        bool refresh_arrow = true; 
        bool button_pressed_once;
        bool nav_interface = true; 
        char* interface_title = "";
        char* BACK = "< BACK >";
        int prev_dial_val, dial_val;
        double parameter_resolution = 1;  
        
        int select_pin = A5, enable_pin = A4, digital_pin_0 = A2, digital_pin_1 = 7, digital_pin_2 = 8, digital_pin_3 = A3,
        display_coloumns = 16, display_rows = 2;

        int SW_pin = 2, CLK_pin = A0, DT_pin = A1;

        ENCODER dial = ENCODER(CLK_pin, DT_pin, SW_pin);
        LiquidCrystal display = LiquidCrystal(select_pin, enable_pin, digital_pin_0, digital_pin_1, digital_pin_2, digital_pin_3);
        
        bool interfaceBlockade();
        int centerTextDisplacement(char* text);
        int centerTextDisplacement(String text);
        void printPrimaryContent(bool top_row, char* current_content[], int num_of_contents);
        void preparePrimaryContent();
        void printSecondaryContent(parameter current_parameter);
        void prepareSecondaryContent();
        void printArrow();
        void openFolder();
        void openFile();
        void updateDisplay(bool open_folder);
        void primaryInterface(bool active);
        void secondaryInterface(bool active);

        public:

        LCD_Interface();
        void interface();
        void begin(folder* base_folder_ptr);
        void setParameterResolution(double parameter_resolution);
        bool getFileActive(char* folder_name, char* file_name);
        double getFileParameterData(char* folder_name, char* file_name, char* parameter_name);
    };

#endif