#include <iostream>
#include <string>

#include "Int_2048.hpp"

using namespace std;

// constructors
Int_2048::Int_2048(const char *s)
{
    string str = s;
    _value = str;
}

Int_2048::Int_2048(const string &str) : _value(str)
{
}

Int_2048::Int_2048(const int &num) : _value(num)
{
}

Int_2048::Int_2048(const N2Array &ary) : _value(ary.getString())
{
}

// copy constructor
Int_2048::Int_2048(const Int_2048 &n) : _value(n._value.getString())
{
}

// public method
inline void Int_2048::print() const
{
    cout << this->_value.getString();
}

// operators
inline Int_2048 Int_2048::operator+(const Int_2048 &n) const
{
    Int_2048 result = this->_value + n._value;
    return result;
}

inline Int_2048 Int_2048::operator-(const Int_2048 &n) const
{
    Int_2048 result = this->_value - n._value;
    return result;
}

inline Int_2048 Int_2048::operator*(const Int_2048 &n) const
{
    Int_2048 result = this->_value * n._value;
    return result;
}
