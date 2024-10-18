#include <iostream>
#include <ctime>

#include "ustring.hpp"

using namespace ustr;

int main()
{
    clock_t t1 = clock();

    string_float str = "11.99";
    string_float str2 = "90.01";
    str /= str2;
    std::cout << str;

    clock_t t2 = clock();
    std::cout << (double)(t2 - t1) << "ms";
}