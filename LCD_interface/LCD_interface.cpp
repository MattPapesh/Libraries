#include "LCD_Interface.h"
#include <stdio.h>
#include <stdlib.h>
    
    parameter::parameter(){}
    parameter::parameter(char* parameter_name, double data)
    :   parameter_name(parameter_name), data(data){}

    char* parameter::getParameterName()
    {
        return parameter_name;
    }

    double parameter::getData()
    {
        return data;
    }

    void parameter::setParameterName(char* parameter_name)
    {
        this->parameter_name = parameter_name;
    }

    void parameter::setData(double data)
    {
        this->data = data; 
    }

    file::file(){}
    file::file(char* file_name)
    :   file_name(file_name){}

    file::~file()
    {
        delete[] parameter_ptrs;
        parameter_ptrs = nullptr;
    }

    void file::addParameter(char* parameter_name, double data)
    {
        parameter* new_parameter_ptrs = new parameter[num_of_parameter_ptrs + 1];

        for(int i = 0; i < num_of_parameter_ptrs; i++)
        {
            *(new_parameter_ptrs + i) = *(parameter_ptrs + i);
        }

        (new_parameter_ptrs + num_of_parameter_ptrs)->setParameterName(parameter_name);
        (new_parameter_ptrs + num_of_parameter_ptrs)->setData(data);

        delete[] parameter_ptrs;
        parameter_ptrs = new_parameter_ptrs;
        num_of_parameter_ptrs++;
    }

    char* file::getFileName()
    {
        return file_name;
    }

    parameter* file::getParameterPTRs()
    {
        return parameter_ptrs;
    }

    int file::getNumOfParameters()
    {
        return num_of_parameter_ptrs;
    }

    folder::folder(){}
    folder::folder(char* folder_name)
    :   folder_name(folder_name){}

    folder::~folder()
    {
        delete[] file_ptr_ptrs, folder_ptr_ptrs;
        file_ptr_ptrs = nullptr, folder_ptr_ptrs = nullptr;
    }

    void folder::addFolders(folder* folders[], int num_of_folders)
    {
        folder** new_folder_ptr_ptrs = new folder*[num_of_folder_ptrs + num_of_folders];

        for(int i = 0; i < num_of_folder_ptrs; i++)
        {
            *(new_folder_ptr_ptrs + i) = *(folder_ptr_ptrs + i);
        }

        for(int i = 0; i < num_of_folders; i++)
        {
            *(new_folder_ptr_ptrs + num_of_folder_ptrs + i) = folders[i];
            (*(new_folder_ptr_ptrs + num_of_folder_ptrs + i))->setParentFolderName(folder_name);
            (*(new_folder_ptr_ptrs + num_of_folder_ptrs + i))->setParentFolderPTR(this);
        }

        delete[] folder_ptr_ptrs;
        folder_ptr_ptrs = new_folder_ptr_ptrs;
        num_of_folder_ptrs = num_of_folder_ptrs + num_of_folders;
    }

    void folder::addFiles(file* files[], int num_of_files)
    {
        file** new_file_ptr_ptrs = new file*[num_of_file_ptrs + num_of_files];

        for(int i = 0; i < num_of_file_ptrs; i++)
        {
            *(new_file_ptr_ptrs + i) = *(file_ptr_ptrs + i);
        }

        for(int i = 0; i < num_of_files; i++)
        {
            *(new_file_ptr_ptrs + num_of_file_ptrs + i) = files[i];
        }

        delete[] file_ptr_ptrs;
        file_ptr_ptrs = new_file_ptr_ptrs;
        num_of_file_ptrs = num_of_file_ptrs + num_of_files;
    }

    folder* folder::getFolderPTR(int index)
    {
        if(index < num_of_folder_ptrs)
        {
            return *(folder_ptr_ptrs + index);
        }

        return nullptr;
    }

    // Methods for any info regarding this folder
    char* folder::getFolderName()
    {
        return folder_name;
    }

    int folder::getNumOfFiles()
    {
        return num_of_file_ptrs;
    } 
        
    int folder::getNumOfFolders()
    {
        return num_of_folder_ptrs;
    }

    // Methods for any info regarding this folder's parent folder
    void folder::setParentFolderName(char* parent_folder_name)
    {
        this->parent_folder_name = parent_folder_name;
    }

    void folder::setParentFolderPTR(folder* parent_folder_ptr)
    {
        this->parent_folder_ptr = parent_folder_ptr;
    }

    folder* folder::getParentFolderPTR()
    {
        return parent_folder_ptr;
    }

    // Method regarding any info regarding this folder's child folders and files
    file* folder::getFilePTR(int index)
    {
        if(index < num_of_file_ptrs)
        {
            return *(file_ptr_ptrs + index);
        }

        return nullptr;
    }

    char** folder::getContentNamePTRs(bool folders_before_files)
    {
        char** content_name_ptrs = new char*[num_of_folder_ptrs + num_of_file_ptrs];

        if(folders_before_files)
        {
            for(int i = 0; i < num_of_folder_ptrs; i++)
            {
                *(content_name_ptrs + i) = (*(folder_ptr_ptrs + i))->getFolderName();
            }

            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                *(content_name_ptrs + num_of_folder_ptrs + i) = (*(file_ptr_ptrs + i))->getFileName();
            }
        }
        else
        {
            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                *(content_name_ptrs + i) = (*(file_ptr_ptrs + i))->getFileName();
            }

            for(int i = 0; i < num_of_folder_ptrs; i++)
            {
                *(content_name_ptrs + num_of_file_ptrs + i) = (*(folder_ptr_ptrs + i))->getFolderName();
            }
        }

        return content_name_ptrs;
    }

    LCD_Interface::LCD_Interface()
    {
        display.begin(display_coloumns, display_rows);  
    }

    void LCD_Interface::begin(folder* base_folder_ptr)
    {
        this->base_folder_ptr = base_folder_ptr;
        current_folder_open_ptr = base_folder_ptr;
        dial.encoderVal = base_folder_ptr->getNumOfFolders() + base_folder_ptr->getNumOfFiles();
        interface_title = base_folder_ptr->getFolderName();
    }

    folder* LCD_Interface::getFolderPTR(folder* current_folder_ptr, char* folder_name)
    {
        int folder_index = 0;
        int num_of_folders = current_folder_ptr->getNumOfFolders();

        for(int i = 0; i < num_of_folders; i++)
        {
            if(current_folder_ptr->getFolderPTR(i)->getFolderName() == folder_name)
            {
                return current_folder_ptr->getFolderPTR(i);
            }

            if(getFolderPTR(current_folder_ptr->getFolderPTR(i), folder_name))
            {
                return getFolderPTR(current_folder_ptr->getFolderPTR(i), folder_name);
            }
        }

        return nullptr;
    }

    folder* LCD_Interface::getFolderPTR(char* folder_name) // set equal to the current_folder_selected
    {
        folder* folder_ptr_found = getFolderPTR(base_folder_ptr, folder_name);

        if(base_folder_ptr->getFolderName() == folder_name)
        {
            return base_folder_ptr;
        }
        else if(folder_ptr_found != nullptr)
        {
            return folder_ptr_found;
        }
        
        return nullptr;
    }

    bool LCD_Interface::interfaceBlockade()
    {
        if (dial_val < 0)
        {
            dial.encoderVal = 0;
            return false;
        }
        else if (dial_val >= current_folder_open_ptr->getNumOfFolders() + current_folder_open_ptr->getNumOfFiles() + 1)
        {
            dial.encoderVal = current_folder_open_ptr->getNumOfFolders() + current_folder_open_ptr->getNumOfFiles(); 
            return false;
        }

        return true;    
    }
    
    // finds the horizontal position of the cursur to center the printed text equation centers option text
    int LCD_Interface::centerTextDisplacement(char* text)
    {   
        String text_str = text;
        return (9 - (text_str.length() / 2) ); 
    }

    int LCD_Interface::centerTextDisplacement(String text)
    {   
        return (9 - (text.length() / 2) ); 
    }

    void LCD_Interface::printArrow()
    {
        prev_row = row;

        if (dial_val > prev_dial_val)
        {
            row = 0; //top row
        }
        else if (prev_dial_val > dial_val)
        {
            row = 1;  // bottom row
        }
    
        if (dial_val != prev_dial_val)
        {
            display.clear();
            display.setCursor(0, prev_row);
            display.print(remove_arrow);
            display.setCursor(0, row);
            display.print(arrow);
        }
    }     

    void LCD_Interface::openFolder()
    {
        if (button_pressed_once && (getFolderPTR(current_content_selected) != nullptr) && (getFolderPTR(current_content_selected)->getFolderName() != current_folder_open_ptr->getFolderName()))
        {
            display.clear();
            current_folder_open_ptr = getFolderPTR(current_content_selected);
            dial_val = getFolderPTR(current_content_selected)->getNumOfFolders() +  getFolderPTR(current_content_selected)->getNumOfFiles() + 1;
            prev_dial_val = 0;
        } 
        else if(button_pressed_once && current_content_selected == BACK)
        {
            display.clear();
            current_folder_open_ptr = current_folder_open_ptr->getParentFolderPTR();
            dial_val = getFolderPTR(current_content_selected)->getNumOfFolders() +  getFolderPTR(current_content_selected)->getNumOfFiles() + 1;
            prev_dial_val = 0;
        }
            
         button_pressed_once = false; 
    }  

    void LCD_Interface::preparePrimaryContent()
    {
        // number of folders and files, plus the BACK keyword
        int num_of_contents = current_folder_open_ptr->getNumOfFolders() + current_folder_open_ptr->getNumOfFiles() + 1;
        char* contents[num_of_contents] = {};
        char** content_ptrs = current_folder_open_ptr->getContentNamePTRs(folders_before_functions);      
            
        if(current_folder_open_ptr == base_folder_ptr)
        {
           contents[num_of_contents - 1] = interface_title;
        }
        else
        {
            contents[num_of_contents - 1] = BACK;
        }     

        for(int i = 0; i < num_of_contents - 1; i++)
        {
            contents[i] = *(content_ptrs + num_of_contents - i - 2);      
        }
            
        delete[] content_ptrs; 
        current_content_selected = contents[dial_val]; 
        printArrow();
        printPrimaryContent(row, contents);
    }
        
    void LCD_Interface::printPrimaryContent(bool bottom_row, char* current_content[])
    { 
        if (interfaceBlockade())
        {
            int primary_option, peripheral_option; 

            if (!bottom_row)
            {
                if (dial_val == 0)
                {
                    peripheral_option = 0;
                }
                else
                {
                    peripheral_option = dial_val - 1; 
                }
            
                primary_option = dial_val;
            }
            else if (bottom_row)
            {
                peripheral_option = dial_val + 1;
                primary_option = dial_val;
            }

            display.setCursor(centerTextDisplacement(current_content[primary_option]), bottom_row); 
            display.print(current_content[primary_option]); 
            display.setCursor(centerTextDisplacement(current_content[peripheral_option]), !bottom_row);
    
            if (primary_option != peripheral_option)
            {
                display.print(current_content[peripheral_option]);
            }
        }
    }

    void LCD_Interface::openFile()
    {       
        for(int i = 0; i < current_folder_open_ptr->getNumOfFiles(); i++)
        {
            if(current_content_selected == current_folder_open_ptr->getFilePTR(i)->getFileName())
            {
                current_file_open_ptr = current_folder_open_ptr->getFilePTR(i);
            }
        }
    }

    void LCD_Interface::prepareSecondaryContent()
    {
       static int current_parameter_index = 0;

       if(button_pressed_once)
       {
           current_parameter_index++;  
       } 
       
       if(current_parameter_index >= current_file_open_ptr->getNumOfParameters() || nav_interface == true)
       {
           current_parameter_index = 0;
       }

       if(dial_val > prev_dial_val)
       {
           (current_file_open_ptr->getParameterPTRs() + current_parameter_index)->setData
           (
               (current_file_open_ptr->getParameterPTRs() + current_parameter_index)->getData() + parameter_resolution
           );
       }
       else if(dial_val < prev_dial_val)
       {
           (current_file_open_ptr->getParameterPTRs() + current_parameter_index)->setData
           (
               (current_file_open_ptr->getParameterPTRs() + current_parameter_index)->getData() - parameter_resolution
           );
       }

       printSecondaryContent(*(current_file_open_ptr->getParameterPTRs() + current_parameter_index));
    }

    void LCD_Interface::printSecondaryContent(parameter current_parameter)
    {
        if(current_file_open_ptr != nullptr)
        {
            String content = (String)current_parameter.getParameterName() + (String)": " + (String)current_parameter.getData();

            display.setCursor(centerTextDisplacement(current_file_open_ptr->getFileName()), 0);
            display.print(current_file_open_ptr->getFileName());
            display.setCursor(centerTextDisplacement(content), 1);
            display.print(content);
        }
    }

    void LCD_Interface::setParameterResolution(double parameter_resolution)
    {
        this->parameter_resolution = parameter_resolution;
    }

    // runs the current menu (displays current parent folder)
    void LCD_Interface::interface()
    {         
        prev_dial_val = dial_val;
        dial_val = dial.encoderValue();
        button_pressed_once = dial.buttonPress(); 

        if(button_pressed_once)
        {
            delay(BUTTON_COOL_DOWN);
            display.clear();

            for(int i = 0; i < 500; i++)
            {
                if(dial.buttonPress())
                {
                    button_pressed_once = false;
                    nav_interface = !nav_interface;
                    delay(BUTTON_COOL_DOWN);
                    break;
                }

                delay(1);
            }
        }

        primaryInterface(nav_interface);
        secondaryInterface(!nav_interface);
        delay(PERIODIC_DELAY);
    }

    // used to navigate folder and to run files
    void LCD_Interface::primaryInterface(bool active)
    {
        if(active)
        {
            current_file_open_ptr = nullptr;
            openFolder();
            preparePrimaryContent();
        } 
    }

    // used to interfact with files; updating file parameters
    void LCD_Interface::secondaryInterface(bool active)
    {
        if(active)
        {
            openFile();

            if(current_file_open_ptr != nullptr && current_file_open_ptr->getParameterPTRs() != nullptr)
            {
                prepareSecondaryContent();
            }
            else
            {
                nav_interface = true;
            }
        }
    }

    bool LCD_Interface::getFileActive(char* folder_name, char* file_name)
    {
        folder folder = *getFolderPTR(folder_name);

        for(int i = 0; i < folder.getNumOfFiles(); i++)
        {
            
        }
    }