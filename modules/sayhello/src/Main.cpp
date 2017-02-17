#include <iostream>


const char* name = "sayhello";


extern "C"
void exec()
{
    std::cout << "Hello!" << std::endl;
}
