#pragma once

#include <string>

#include "N2Array.hpp"

class Int_2048
{
private:
    N2Array _value;

public:
    // constructors
    Int_2048() : _value(0) {};
    Int_2048(const char *);
    Int_2048(const std::string &);
    Int_2048(const int &);
    Int_2048(const N2Array &);

    // copy constructors
    Int_2048(const Int_2048 &);

    // destructor
    ~Int_2048() {};

    void print() const;

public:
    Int_2048 operator+(const Int_2048 &) const;
    Int_2048 operator-(const Int_2048 &) const;
    Int_2048 operator*(const Int_2048 &) const;
};