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
 * *** Recomendation example
 *  Inf_inf::random_inf(0, 9);
 *  Inf_inf::random_inf(0, 99);
 *  Inf_inf::random_inf("10", 999);
 *  Inf_inf::random_inf("10000000000000", "9999999999999999999");
 *  Inf_inf::random_inf("100000000000000000000000000000000000000000000", "99999999999999999999999999999999999999999999999999999999999999999999");
 *
 *  minimum: to 10 (for maximize range)
 *  maximum: to 99 (for maximize range)
 */
Int_inf Int_inf::random_inf(Int_inf from, Int_inf to)
{
    if (from > to)
        throw runtime_error("random_inf : not valid range (from > to)");

    // setup
    int i = from._value.getString().length() - 1;
    int j = to._value.getString().length() - 1;

    // range of digits from <= x <= to
    int range = j - i + 1;
    int digits = rand() % range + i + 1;

    string min = from._value.getString();
    string max = to._value.getString();

    std::reverse(min.begin(), min.end());
    std::reverse(max.begin(), max.end());

    string result;
    // first digits
    // digits == from's digits
    int num;
    bool sameNumber = false;
    if (digits == i + 1)
    {
        int num1 = min[i] - '0';
        do
        {
            num = rand() % 9 + 1;
        } while (num < num1);
        if (num == num1)
            sameNumber = true;
    }
    // digits == to's digits
    else if (digits == j + 1)
    {
        int num2 = max[j] - '0';
        do
        {
            num = rand() % 9 + 1;
        } while (num > num2);
        if (num == num2)
            sameNumber = true;
    }
    // digits: from < x < to
    else
        num = rand() % 9 + 1;

    num != 0 ? result.push_back(num + '0') : result.push_back('0');
    i--, j--;
    // next digits
    for (int k = 1; k < digits; k++)
    {
        int num1 = i >= 0 ? min[i] - '0' : 0;
        int num2 = j >= 0 ? max[j] - '0' : 0;

        // same digits to from
        if (digits == from._value.getString().length())
        {
            if (sameNumber)
            {
                do
                {
                    num = rand() % 10;
                } while (num < num1);
                if (num != num1)
                    sameNumber = false;
            }
            else
                num = rand() % 10;
        }
        // same digits to to
        else if (digits == to._value.getString().length())
        {
            if (sameNumber)
            {
                do
                {
                    num = rand() % 10;
                } while (num > num2);
                if (num != num2)
                    sameNumber = false;
            }
            else
                num = rand() % 10;
        }
        else
            num = rand() % 10;

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
Int_inf::Int_inf() : _value()
{
}

Int_inf::Int_inf(const char *s)
{
    string str = s;
    _value = str;
}

// not const string because N2Array(string) changed value
Int_inf::Int_inf(string &str) : _value(str)
{
}

// Int_inf(5), Int_inf(int)
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
/**
 *  miller-rabin primality test algorithm
 */
// bool Int_inf::isPrime() const
// {
//     int primes[] = {2, 3, 5, 7, 9, 11, 13, 17, 19, 23, 29, 31, 37};

//     if (*this <= 1 || *this == 4)
//         return false;

//     Int_inf pow = *this - 1;
//     while (pow % 2 == 0)
//         pow = pow / 2;

//     for (int i : primes)
//     {
//         if (*this == i)
//             return true;

//         Int_inf temp = powOperation(i, pow);
//         if (temp == 1 || temp == *this - 1)
//             continue;

//         while (pow != *this - 1)
//         {
//             temp = (temp * temp) % *this;
//             pow = pow * 2;
//             if (temp == 1)
//                 return false;
//             if (temp == *this - 1)
//                 continue;
//         }
//         return false;
//     }
//     return true;
// }

// // isPrime operation
// Int_inf Int_inf::powOperation(Int_inf prime, Int_inf pow) const
// {
//     Int_inf result = 1;

//     if (prime > *this)
//         prime = prime % *this;
//     while (pow > 0)
//     {
//         if (pow % 2 == 1)
//             result = (result * prime) % *this;
//         prime = (prime * prime) % *this;
//         pow = pow / 2;
//     }
//     return result;
// }

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

// iostream operator
inline ostream &operator<<(ostream &out, const Int_inf &n)
{
    out << n._value;
    return out;
}

inline istream &operator>>(istream &in, Int_inf &n)
{
    N2Array value;
    in >> value;
    n = Int_inf(value);
    return in;
}
