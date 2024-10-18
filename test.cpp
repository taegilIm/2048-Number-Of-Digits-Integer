#include <iostream>
#include <ctime>

#include "src/string_int.cpp"
#include "src/string_float.cpp"

using namespace ustr;

int main()
{
    clock_t t1 = clock();

    // string_int str = "100";
    // string_int str2 = "90";

    string_float::precision(10);

    string_float str = "1212";
    string_float str2 = "112";
    // str /= str2;
    std::cout << str;

    clock_t t2 = clock();
    std::cout << (double)(t2 - t1) << "ms";
}