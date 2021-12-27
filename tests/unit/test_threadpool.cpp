#include <iostream>
#include <threadpool.hpp>
using namespace ca;
int main()
{
    Threadpool pool(4);
    std::cout << "Ciao" << std::endl;
}