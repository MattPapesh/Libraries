#include <iostream>
#include <stdlib.h>
#include <stdio.h>

    class parameter // A file parameter that has a name and some kind of data associated with it
    {
        private:

        char* parameter_name = "";
        double data = 0;

        public:

        parameter(){}
        parameter(char* parameter_name, double data)
        :   parameter_name(parameter_name), data(data){}

        char* getParameterName()
        {
            return parameter_name;
        }

        double getData()
        {
            return data; 
        }
    };

    class file // A "file", where it has a name, and an address to an array of dynamically allocated parameters; a file can have multiple parameters
    {
        private:

        char* file_name = "";
        parameter* parameter_ptrs = nullptr;
        int num_of_parameter_ptrs = 0; 

        public:

        file(){}
        file(char* file_name)
        :   file_name(file_name){}

        ~file()
        {
            delete[] parameter_ptrs;
            parameter_ptrs = nullptr;
        }

        void addParameters(parameter parameters[], int num_of_parameters)
        {
            parameter* new_parameter_ptrs = new parameter[num_of_parameter_ptrs + num_of_parameters];

            for(int i = 0; i < num_of_parameter_ptrs; i++)
            {
                *(new_parameter_ptrs + i) = *(parameter_ptrs + i);
            }

            for(int i = 0; i < num_of_parameters; i++)
            {
                *(new_parameter_ptrs + num_of_parameter_ptrs + i) = parameters[i];
            }

            delete[] parameter_ptrs;
            parameter_ptrs = new_parameter_ptrs;
            num_of_parameter_ptrs = num_of_parameter_ptrs + num_of_parameters; 
        }

        char* getFileName()
        {
            return file_name; 
        }
    };

    class folder // A "folder", where it has a name, and can store other folders and files that are both dynamically allocated.
    {
        private:

        char* folder_name = "";
        char* parent_folder_name = ""; 
        folder* parent_folder_ptr = nullptr;
        file* file_ptrs = nullptr;
        int num_of_file_ptrs = 0;
        folder* folder_ptrs = nullptr;
        int num_of_folder_ptrs = 0;

        public:
        
        folder(){}
        folder(char* folder_name)
        :   folder_name(folder_name){}

        ~folder()
        {
            delete[] file_ptrs, folder_ptrs;
            file_ptrs = nullptr, folder_ptrs = nullptr;
        }

        void addFolders(folder folders[], int num_of_folders)
        {
            folder* new_folder_ptrs = new folder[num_of_folder_ptrs + num_of_folders];

            for(int i = 0; i < num_of_folder_ptrs; i++)
            {
                *(new_folder_ptrs + i) = *(folder_ptrs + i);
                (new_folder_ptrs + i)->setParentFolderName(folder_name); 
                (new_folder_ptrs + i)->setParentFolderPTR(this);
            }

            for(int i = 0; i < num_of_folders; i++)
            {
                *(new_folder_ptrs + num_of_folder_ptrs + i) = folders[i];
                (new_folder_ptrs + num_of_folder_ptrs + i)->setParentFolderName(folder_name);
                (new_folder_ptrs + num_of_folder_ptrs + i)->setParentFolderPTR(this);
            }

            delete[] folder_ptrs;
            folder_ptrs = new_folder_ptrs;
            num_of_folder_ptrs = num_of_folder_ptrs + num_of_folders; 
        }

        void addFiles(file files[], int num_of_files)
        {
            file* new_file_ptrs = new file[num_of_file_ptrs + num_of_files];

            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                *(new_file_ptrs + i) = *(file_ptrs + i);
            }

            for(int i = 0; i < num_of_files; i++)
            {   
                *(new_file_ptrs + num_of_file_ptrs + i) = files[i];
            }

            delete[] file_ptrs;
            file_ptrs = new_file_ptrs;
            num_of_file_ptrs = num_of_file_ptrs + num_of_files;
        }

        char** getContentNamePTRs()// returns a dynamically allocated array of char*s that represent the names of files and child folders inside of a folder
        {
            char** content_name_ptrs = new char*[num_of_file_ptrs + num_of_folder_ptrs];

            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                *(content_name_ptrs + i) = (file_ptrs + i)->getFileName();
            }

            for(int i = 0; i < num_of_folder_ptrs; i++)
            {
                *(content_name_ptrs + num_of_file_ptrs + i) = (folder_ptrs + i)->getFolderName();
            }

            return content_name_ptrs; 
        }
        
        void setParentFolderName(char* parent_folder_name)
        {
            this->parent_folder_name = parent_folder_name; 
        }

        void setParentFolderPTR(folder* parent_folder_ptr)
        {
            this->parent_folder_ptr = parent_folder_ptr; 
        }

        char* getFolderName()
        {
            return folder_name; 
        }

        char* getParentFolderName()
        {
            return parent_folder_name; 
        }

        folder* getParentFolderPTR()
        {
            return parent_folder_ptr;
        }

        file getFile(char* file_name)
        {
            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                if((file_ptrs + i)->getFileName() == file_name)
                {
                    return *(file_ptrs + i);
                }
            }
        }

        int getNumOfFiles()
        {
            return num_of_file_ptrs;
        }

        folder* getFolderPTRs()
        {
            return folder_ptrs;
        }

        int getNumOfFolders()
        {
            return num_of_folder_ptrs;
        }
    };

    class LCD_Interface // An interface to navigate the "folder-file" data structure -> will be prepared to eventually interface older code!
    {
        private:

        folder* base_folder_ptr = nullptr;
        folder current_folder_open = NULL;
        char* current_content_selected = "";

        void incrementNumOfLevelPTRs(int** lvl_ptrs_ptr, int* num_of_lvl_ptrs_ptr)
        {
            *num_of_lvl_ptrs_ptr++;
            int* new_lvl_ptrs = new int[*num_of_lvl_ptrs_ptr];

            for(int i = 0; i < *num_of_lvl_ptrs_ptr - 1; i++)
            {
                *(new_lvl_ptrs + i) = *(*lvl_ptrs_ptr + i);
            }

            delete[] *lvl_ptrs_ptr; 
            *lvl_ptrs_ptr = new_lvl_ptrs;
        }
        
        folder* getFolderPTR(folder* current_folder_ptr, char* folder_name)
        {
            int folder_index = 0;
            int num_of_folders = current_folder_ptr->getNumOfFolders();

            for(int i = 0; i < num_of_folders; i++)
            {
                if((current_folder_ptr->getFolderPTRs() + i)->getFolderName() == folder_name)
                {
                    return current_folder_ptr->getFolderPTRs() + i;
                }

                if(getFolderPTR(current_folder_ptr->getFolderPTRs() + i, folder_name))
                {
                    return getFolderPTR(current_folder_ptr->getFolderPTRs() + i, folder_name);
                }
            }

            return nullptr;
        }

    public:
        folder getFolder(char* folder_name)
        {
            folder* folder_ptr_found = getFolderPTR(base_folder_ptr, folder_name);

            if(folder_ptr_found != nullptr)
            {
                return *folder_ptr_found;
            }
            else
            {
                printf("Warning! Requested folder wasn't found!");
                return folder();
            }
        }

        public:

        LCD_Interface(folder* base_folder_ptr)
        {
            this->base_folder_ptr = base_folder_ptr; 
            
        }
    };

    folder base = folder("myFolder");
    folder children[3] = {folder("[] child1"), folder("[] child2"), folder("[] child3")};
    folder grand_children1[2] = {folder("[] grandchild1"), folder("[] grandchild2")};
    folder grand_children2[2] = {folder("[] grandchildA"), folder("[] grandchildB")};
    file files[2] = {file("file1"), file("file2")};
    //LCD_Interface interface = LCD_Interface();



    int main()
    {
        children[0].addFolders(grand_children1, 2);
        children[1].addFolders(grand_children2, 2);
        base.addFolders(children, 3);
        base.addFiles(files, 2);

        LCD_Interface i = LCD_Interface(&base);
        printf("folder found: %s", i.getFolder("[] grandchildB").getFolderName());

        return 0;
    }