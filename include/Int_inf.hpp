#pragma once

#include <string>

#include "N2Array.hpp"

#ifdef DEFINE_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

class DLL_API Int_inf
{
private:
    N2Array _value;

public:
    // static methods
    static Int_inf random_inf(Int_inf, Int_inf);
    static void setseed();
    static Int_inf gcd(const Int_inf &, const Int_inf &);

    // constructors
    Int_inf() : _value(0) {};
    Int_inf(const char *);
    Int_inf(const std::string &);
    Int_inf(const int);
    Int_inf(const N2Array &);

    // copy constructors
    Int_inf(const Int_inf &);

    // destructor
    ~Int_inf() {};

private:
    // isPrime operation
    Int_inf ML_operation(Int_inf, Int_inf) const;

public:
    // public methods
    bool isPrime() const;
    Int_inf power(const Int_inf &) const;

    // for test
    void print() const;
    N2Array &getArray();

    // operators
    Int_inf operator+(const Int_inf &) const;
    Int_inf operator-(const Int_inf &) const;
    Int_inf operator*(const Int_inf &) const;
    Int_inf operator/(const Int_inf &) const;
    Int_inf operator%(const Int_inf &) const;
    friend int operator%(const int, const Int_inf &);

    bool operator==(const Int_inf &) const;
    bool operator!=(const Int_inf &) const;
    bool operator>(const Int_inf &) const;
    friend bool operator>(const int &, const Int_inf &);
    bool operator>=(const Int_inf &) const;
    bool operator<(const Int_inf &) const;
    bool operator<=(const Int_inf &) const;
};
