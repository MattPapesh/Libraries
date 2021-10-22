#include <iostream>
#include <stdlib.h>
#include <stdio.h>

    struct parameter // A file parameter that has a name and some kind of data associated with it
    {
        char* parameter_name = "";
        double data = 0;

        parameter(char* parameter_name, double data)
        :   parameter_name(parameter_name), data(data){}
    };

    struct file // A "file", where it has a name, and an address to an array of dynamically allocated parameters; a file can have multiple parameters
    {
        char* file_name = "";
        parameter* parameter_ptrs = nullptr;

        file(char* file_name, parameter* parameter_ptrs)
        :   file_name(file_name), parameter_ptrs(parameter_ptrs){}

        ~file()
        {
            delete[] parameter_ptrs;
            parameter_ptrs = nullptr;
        }
    };

    class folder // A "folder", where it has a name, and can store other folders and files that are both dynamically allocated.
    {
        private:

        char* folder_name = "";
        char* parent_folder_name = ""; 
        file* file_ptrs = nullptr;
        int num_of_file_ptrs = 0;
        folder* folder_ptrs = nullptr;
        int num_of_folder_ptrs = 0;

        public:

        folder(char* folder_name, file* file_ptrs, int num_of_file_ptrs, folder* folder_ptrs, int num_of_folder_ptrs)
        :   folder_name(folder_name), file_ptrs(file_ptrs), num_of_file_ptrs(num_of_file_ptrs), folder_ptrs(folder_ptrs), 
            num_of_folder_ptrs(num_of_folder_ptrs){}

        ~folder()
        {
            delete[] file_ptrs, folder_ptrs;
            file_ptrs = nullptr, folder_ptrs = nullptr;
        }

        char** getContentNamePTRs()
        {
            char** content_name_ptrs = new char*[num_of_file_ptrs + num_of_folder_ptrs];
            int current_content_index = 0;

            for(NULL; current_content_index < num_of_file_ptrs; current_content_index++)
            {
                *(content_name_ptrs + current_content_index) = (file_ptrs + current_content_index)->file_name;
            }

            for(int current_folder_index = num_of_file_ptrs; current_content_index < num_of_file_ptrs + num_of_folder_ptrs; current_content_index++, current_folder_index++)
            {
                *(content_name_ptrs + current_content_index) = (folder_ptrs + current_folder_index)->getFolderName();
            }

            return content_name_ptrs; 
        }

        char* getFolderName()
        {
            return folder_name; 
        }

        char* getParentFolderName()
        {
            return parent_folder_name; 
        }
    };

    class LCD_Interface // An interface to navigate the "folder-file" data structure -> will be prepared to eventually interface older code!
    {
        private:

        public:

        LCD_Interface()
        {

        }
    };

int main()
{
    printf("Hello world!");

    return 0;
}