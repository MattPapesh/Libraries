#include <iostream>
#include <stdlib.h>
#include <functional>

struct myStruct
{
    int x = 0;
    myStruct(int temp_x)
    :   x(temp_x){}
};

class example
{
    public:

    myStruct data_set_1 = myStruct(1);
    myStruct data_set_2 = myStruct(2);
    myStruct data_set_3 = myStruct(3);

    void method(std::function<int()> function)
    {
        printf("this data is "); std::cout << function;
    }

    example()
    {
        method([=]()->int{return data_set_1.x;});
    }
};

example myExample; 

int main()
{
    return 0;
}