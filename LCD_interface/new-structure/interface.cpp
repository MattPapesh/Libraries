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
        file** file_ptr_ptrs = nullptr;
        int num_of_file_ptrs = 0;
        folder** folder_ptr_ptrs = nullptr;
        int num_of_folder_ptrs = 0;

        public:
        
        folder(){}
        folder(char* folder_name)
        :   folder_name(folder_name){}

        ~folder()
        {
            delete[] *file_ptr_ptrs, file_ptr_ptrs, *folder_ptr_ptrs, folder_ptr_ptrs;
            file_ptr_ptrs = nullptr, folder_ptr_ptrs = nullptr;
        }

        void addFolder(folder* folder_ptr)
        {
            folder** new_folder_ptr_ptrs = new folder*[num_of_folder_ptrs + 1];

            for(int i = 0; i < num_of_folder_ptrs; i++)
            {
                *(new_folder_ptr_ptrs + i) = *(folder_ptr_ptrs + i);
                (*(new_folder_ptr_ptrs + i))->setParentFolderName(folder_name); 
                (*(new_folder_ptr_ptrs + i))->setParentFolderPTR(this);
            }
            std::cout<<"HELP";
            *(new_folder_ptr_ptrs + num_of_folder_ptrs + 1) = folder_ptr;
            (*(new_folder_ptr_ptrs + num_of_folder_ptrs + 1))->setParentFolderName(folder_name);
            (*(new_folder_ptr_ptrs + num_of_folder_ptrs + 1))->setParentFolderPTR(this);

           // delete[] folder_ptr_ptrs;
            folder_ptr_ptrs = new_folder_ptr_ptrs;
            num_of_folder_ptrs++; 
        }

        void addFile(file* file_ptr)
        {
            file** new_file_ptr_ptrs = new file*[num_of_file_ptrs + 1];

            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                *(new_file_ptr_ptrs + i) = *(file_ptr_ptrs + i);
            }
   
            *(new_file_ptr_ptrs + num_of_file_ptrs + 1) = file_ptr;            

            delete[] file_ptr_ptrs;
            file_ptr_ptrs = new_file_ptr_ptrs;
            num_of_file_ptrs++;
        }

        char** getContentNamePTRs()// returns a dynamically allocated array of char*s that represent the names of files and child folders inside of a folder
        {
            char** content_name_ptrs = new char*[num_of_file_ptrs + num_of_folder_ptrs];

            for(int i = 0; i < num_of_file_ptrs; i++)
            {
                *(content_name_ptrs + i) = (*(file_ptr_ptrs + i))->getFileName();
            }

            for(int i = 0; i < num_of_folder_ptrs; i++)
            {
                *(content_name_ptrs + num_of_file_ptrs + i) = (*(folder_ptr_ptrs + i))->getFolderName();
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
                if((*(file_ptr_ptrs + i))->getFileName() == file_name)
                {
                    return *(*(file_ptr_ptrs + i));
                }
            }
        }

        int getNumOfFiles()
        {
            return num_of_file_ptrs;
        }

        folder** getFolderPTR_PTRs()
        {
            return folder_ptr_ptrs;
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
        
        folder getFolder(char* folder_name)
        {
            int num_of_lvl_indexes = 20;
            int current_lvl = 0;
            int* lvl_index_ptrs = new int[num_of_lvl_indexes]; 
            folder* current_folder_ptr = base_folder_ptr;

            for(int i = 0; i < num_of_lvl_indexes; i++){*(lvl_index_ptrs + i) = 0;}

            while(current_folder_ptr->getFolderName() != folder_name)
            {   
                if((*(current_folder_ptr->getFolderPTR_PTRs() + *(lvl_index_ptrs + current_lvl + 1))) != NULL)
                {   
                    current_folder_ptr = *(current_folder_ptr->getFolderPTR_PTRs() + *(lvl_index_ptrs + current_lvl + 1));
                    current_lvl++;

                    //if(current_lvl + 1 < num_of_lvl_indexes)
                    {
                      //  incrementNumOfLevelPTRs(&lvl_index_ptrs, &num_of_lvl_indexes);
                    }
                }
                else
                {  
                    if(*(lvl_index_ptrs + current_lvl) < current_folder_ptr->getParentFolderPTR()->getNumOfFolders())
                    {
                        (*(lvl_index_ptrs + current_lvl))++;
                        current_folder_ptr = current_folder_ptr->getParentFolderPTR();
                        current_lvl = current_lvl - 1; 
                    }
                    else
                    {
                        *(lvl_index_ptrs + current_lvl) = 0;
                        current_folder_ptr = current_folder_ptr->getParentFolderPTR()->getParentFolderPTR();
                        current_lvl = current_lvl - 2;
                    }
                }
                
                std::cout<<current_folder_ptr->getFolderName()<<std::endl;
                _sleep(1000);
            }
            std::cout<<"done!";
            delete[] lvl_index_ptrs; 
            return folder();//*current_folder_ptr;
        }

        public:

        LCD_Interface(folder* base_folder_ptr)
        {
            this->base_folder_ptr = base_folder_ptr; 
            folder bad = getFolder("[] child3");
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
        children[0].addFolder(&grand_children1[0]);
        children[0].addFolder(&grand_children1[1]);
        children[1].addFolder(&grand_children2[0]);
        children[1].addFolder(&grand_children2[1]);
        base.addFolder(&children[0]);
        base.addFolder(&children[1]); 
        base.addFolder(&children[2]);  
        base.addFile(&files[0]);
        base.addFile(&files[1]);
std::cout << "o=jjhihihiio \n \n";
        LCD_Interface i = LCD_Interface(&base);
        
        return 0;
    }