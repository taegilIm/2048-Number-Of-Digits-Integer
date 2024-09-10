#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Int_inf.hpp"

using namespace std;

// static methods
/**
 *  setSeed() is required
 *
 *  from : minimum (include)
 *  to : maximum (include)
 *
 *  * example
 *  Inf_inf::random_inf(1, 5); -> 1 ~ 5
 */
Int_inf Int_inf::random_inf(Int_inf from, Int_inf to)
{
    // from > to
    if (from > to)
    {
        Int_inf temp = to;
        to = from;
        from = temp;
    }

    // setup
    int i = from._value.getString().length() - 1;
    int j = to._value.getString().length() - 1;

    int range = j - i + 1;
    int digits = rand() % (range - i) + i + 1;

    string min = from._value.getString();
    string max = to._value.getString();

    std::reverse(min.begin(), min.end());
    std::reverse(max.begin(), max.end());

    string result;
    int num;
    // first digits
    // digits == from's digits
    if (digits == i + 1)
    {
        do
        {
            num = rand() % 10;
        } while (num < min[i] - '0');
    }
    // digits == to's digits
    else if (digits == j + 1)
    {
        do
        {
            num = rand() % 10;
        } while (num > max[i] - '0');
    }
    // digits: from < x < to
    else
        num = rand() % 10;

    num != 0 ? result.push_back(num + '0') : result.push_back('0');

    // next digits
    for (int k = 1; k < digits; k++)
    {
        int num1 = i >= 0 ? min[i] - '0' : '0';
        int num2 = j >= 0 ? max[j] - '0' : '0';

        // length: from <= x < to
        if (digits < range)
        {
            // same digits to from
            if (digits == from._value.getString().length())
            {
                do
                {
                    num = rand() % 10;
                } while (num < num1);
            }
            else
                num = rand() % 10;
        }
        // length: from <= x <= to
        else
        {
            if (digits == from._value.getString().length())
            {
                do
                {
                    num = rand() % 10;
                } while (num < num1);
            }
            else if (digits == to._value.getString().length())
            {
                do
                {
                    num = rand() % 10;
                } while (num > num2);
            }
            else
                num = rand() % 10;
        }
        num != 0 ? result.push_back(num + '0') : result.push_back('0');
        i--, j--;
    }

    N2Array::removeZeros(result, true);
    return Int_inf(result);
}

void Int_inf::setseed()
{
    srand(time(NULL));
}

Int_inf Int_inf::gcd(const Int_inf &n1, const Int_inf &n2)
{
    Int_inf p = n1;
    Int_inf q = n2;

    if (p < q)
    {
        Int_inf temp = p;
        p = q;
        q = p;
    }

    Int_inf temp;
    while (q != 0)
    {
        temp = p % q;
        p = q;
        q = temp;
    }
    return p;
}

// constructors
Int_inf::Int_inf(const char *s)
{
    string str = s;
    _value = str;
}

Int_inf::Int_inf(const string &str) : _value(str)
{
}

Int_inf::Int_inf(const int num) : _value(num)
{
}

Int_inf::Int_inf(const N2Array &ary) : _value(ary)
{
}

// copy constructor
Int_inf::Int_inf(const Int_inf &n) : _value(n._value)
{
}

// public method
bool Int_inf::isPrime() const
{
    int primes[] = {2, 3, 5, 7, 9, 11, 13, 17, 19, 23, 29, 31, 37};

    Int_inf pow = *this - 1;
    for (int i : primes)
    {
        Int_inf prime = i;
        if (prime % *this == 0)
            return true;

        while (true)
        {
            Int_inf temp = ML_operation(prime, pow);
            if (temp == pow)
                return true;
            if (pow % 2 == 1)
                return temp == 1;
            pow = pow / 2;
        }
    }
    return false;
}

// isPrime operation
Int_inf Int_inf::ML_operation(Int_inf prime, Int_inf pow) const
{
    Int_inf result = 1;

    if (prime > *this)
        prime = prime % *this;
    prime.print();
    pow.print();
    while (pow > 0)
    {
        if (pow % 2 == 1)
            result = (result * prime) % *this;
        prime = (prime * prime) % *this;
        pow = pow / 2;
        prime.print();
        pow.print();
        result.print();
    }
    return result;
}

inline void Int_inf::print() const
{
    cout << this->_value.getString() << " ";
}

inline Int_inf Int_inf::power(const Int_inf &n) const
{
    Int_inf result = *this;
    result._value.power(n._value);
    return result;
}

// test method

inline N2Array &Int_inf::getArray()
{
    return this->_value;
}
// operators
inline Int_inf Int_inf::operator+(const Int_inf &n) const
{
    Int_inf result = this->_value + n._value;
    return result;
}

inline Int_inf Int_inf::operator-(const Int_inf &n) const
{
    Int_inf result = this->_value - n._value;
    return result;
}

inline Int_inf Int_inf::operator*(const Int_inf &n) const
{
    Int_inf result = this->_value * n._value;
    return result;
}

inline Int_inf Int_inf::operator/(const Int_inf &n) const
{
    Int_inf result = this->_value / n._value;
    return result;
}

inline Int_inf Int_inf::operator%(const Int_inf &n) const
{
    Int_inf result = this->_value % n._value;
    return result;
}

inline int operator%(const int num, const Int_inf &n)
{
    int a = 1;
    return 1;
}

inline bool Int_inf::operator==(const Int_inf &n) const
{
    return (this->_value == n._value);
}

inline bool Int_inf::operator!=(const Int_inf &n) const
{
    return (this->_value != n._value);
}

inline bool Int_inf::operator>(const Int_inf &n) const
{
    return (this->_value > n._value);
}

inline bool operator>(const int &num, const Int_inf &n)
{
    return n._value < num;
}

inline bool Int_inf::operator>=(const Int_inf &n) const
{
    return (this->_value >= n._value);
}

inline bool Int_inf::operator<(const Int_inf &n) const
{
    return (this->_value < n._value);
}

inline bool Int_inf::operator<=(const Int_inf &n) const
{
    return (this->_value <= n._value);
}
