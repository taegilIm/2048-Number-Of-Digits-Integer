#include <iostream>
#include <string>
#include "Int_inf.cpp"
#include "N2Array.cpp"

#include <ctime>

using namespace std;

int main()
{
    clock_t t1 = clock();

    Int_inf::setseed();
    // Int_inf n = 37;
    // cout << n.isPrime();
    Int_inf n1 = "1213231243521378912356123213";
    cout << n1.isPrime();

    clock_t t2 = clock();
    cout << "\ntime: " << t2 - t1 << "ms";
}
