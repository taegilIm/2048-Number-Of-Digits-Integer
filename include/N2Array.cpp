#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>

#include "N2Array.hpp"

using namespace std;

// static methods
/** true -> from front
 *  false -> from back
 */
string &N2Array::removeZeros(string &str, bool pos)
{
    if (pos)
    {
        while (str[0] == '0')
            str.erase(0, 1);
    }
    else
    {
        while (str.back() == '0')
            str.pop_back();
    }

    return str;
}

// constructors
N2Array::N2Array() : _data("0"), _flag(zero), _len(1)
{
}

N2Array::N2Array(const string &value)
{
    if (value.length() > MAXARRAY)
        throw runtime_error("N2Array too big value exception : string must be lower than 65535 bytes");

    if (value.empty())
    {
        _data = "0";
        _flag = zero;
        _len = 1;
        return;
    }
    _data = value;
    _len = value.length();
    value[0] == '-' ? _flag = negative : _flag = positive;
}

N2Array::N2Array(const int &num)
{
    if (num == 0)
    {
        _data = "0";
        _len = 1;
        _flag = zero;
        return;
    }
    _flag = num > 0 ? positive : negative;

    pair<int *, int> value = N2Array::intToIntArray(num, _flag);
    int *ary = value.first;
    int len = value.second;

    string data = _flag == positive ? N2Array::intArrayToString(ary, len, Flag::positive) : N2Array::intArrayToString(ary, len, Flag::negative);

    _data = data;
    _len = len;
}

// copy constructor
N2Array::N2Array(const N2Array &ary) : _data(ary.getString()), _len(ary._len), _flag(ary._flag)
{
}

// private methods
string N2Array::intArrayToString(const int *ary, int &len, Flag type)
{
    string result;
    int index = len;

    if (type == negative)
    {
        result.push_back('-');
        len++;
    }

    for (int i = 0; i < index; i++)
    {
        ary[i] != 0 ? result.push_back(ary[i] + '0') : result.push_back('0');
    }

    return result;
}

/**
 * 12345 / 10 = 1234  d:1
 * 12345 / 100 = 123  d:2
 * 12345 / 1000 = 12  d:3
 * 12345 / 10000 = 1  d:4
 * 12345 / 100000 = 0 d:5
 *
 * 12345 / 10000 % 10 = 1
 * 12345 / 1000 % 10 = 2
 * 12345 / 100 % 10 = 3
 * 12345 / 10 % 10 = 4
 */
pair<int *, int> N2Array::intToIntArray(int num, Flag type)
{
    type == negative ? num = -num : num = num;

    int digits = 0;

    int result = 1;
    while (result)
    {
        digits++;
        int digit = pow(10, digits);
        result = num / digit;
    }

    int *int_array = new int[digits];
    int index = 0;
    for (int i = digits - 1; i >= 0; i--)
    {
        int_array[index++] = (num / (int)(pow(10, i))) % 10;
    }

    return make_pair(int_array, digits);
}

// public methods
inline string N2Array::getString() const
{
    return this->_data;
}

N2Array N2Array::power(const N2Array &ary)
{
    if (ary._flag == zero)
        return 1;

    if (this->_len * ary._len > MAXARRAY)
        throw runtime_error("N2Array : power operation too big number is created");

    N2Array result = *this;

    int i = ary._len - 1;
    while (i >= 0)
    {
        int num = ary._data[i] - '0';
        for (int i = 0; i < num; i++)
            result *= result;

        i--;
    }
    return result;
}

// private methods
// base operations
void N2Array::operations(const N2Array &ary, OP_Type type)
{
    // for compare
    N2Array n1 = *this;
    N2Array n2 = ary;

    if (this->_flag == negative)
        n1._data.erase(0, 1);
    if (ary._flag == negative)
        n2._data.erase(0, 1);

    // copy data
    string ary1 = this->_data;
    string ary2 = ary._data;

    if (this->_flag == negative)
        ary1.erase(0, 1);
    if (ary._flag == negative)
        ary2.erase(0, 1);

    string result;
    switch (type)
    {
    case sum:
        result = add(ary1, ary2);
        break;
    case sub:
        result = subtract(ary1, ary2);
        break;
    case mul:
        result = multiply(ary1, ary2);
        break;
    case div:
        result = divide(ary1, ary2).first;
        break;
    case mod:
        if (this->_flag == Flag::negative || ary._flag == Flag::negative)
            throw runtime_error("N2Array : modulus operator can only both numbers are positive");
        result = modulus(ary1, ary2);
        break;

    default:
        break;
    }

    // last process
    removeZeros(result, false);
    setFlag(result, n1 > n2, this->_flag, ary._flag, type);
    std::reverse(result.begin(), result.end());

    // overflow
    if (result.length() > MAXARRAY)
        throw runtime_error("N2Array : operation create too big number (over 65535 digits)");

    *this = result;
}

// private static method
// operation method
void N2Array::setFlag(string &value, bool isBig, Flag f1, Flag f2, OP_Type type)
{
    if (value.empty())
        return;

    switch (type)
    {
    case sum:
        if (f1 == f2)
        {
            if (f1 == negative)
                value.push_back('-');
        }
        else
        {
            if (isBig)
            {
                if (f1 == negative)
                    value.push_back('-');
            }
            else
            {
                if (f1 == positive)
                    value.push_back('-');
            }
        }
        break;

    case sub:
        if (f1 == f2)
        {
            if (isBig)
            {
                if (f1 == negative)
                    value.push_back('-');
            }
            else
            {
                if (f1 == positive)
                    value.push_back('-');
            }
        }
        else
        {
            if (f1 == negative)
                value.push_back('-');
        }
        break;
    case mul:
        if (f1 != f2)
            value.push_back('-');
        break;

    case div:
        if (f1 != f2)
            value.push_back('-');
        break;
    default:
        break;
    }
}

string N2Array::add(const string &str1, const string &str2)
{
    string result;

    int i = str1.length() - 1;
    int j = str2.length() - 1;

    int sum;
    int carry = 0;
    while (i >= 0 || j >= 0)
    {
        int num1 = i >= 0 ? str1[i] - '0' : 0;
        int num2 = j >= 0 ? str2[j] - '0' : 0;

        sum = num1 + num2 + carry;
        carry = sum / 10;
        sum != 0 ? result.push_back(sum % 10 + '0') : result.push_back('0');

        i--, j--;
    }
    return result;
}

string N2Array::subtract(const string &str1, const string &str2)
{
    string result;

    int i = str1.length() - 1;
    int j = str2.length() - 1;

    int sub;
    int borrow = 0;
    while (i >= 0 || j >= 0)
    {
        int num1 = i >= 0 ? str1[i] - '0' : 0;
        int num2 = j >= 0 ? str2[j] - '0' : 0;

        sub = num1 - num2 - borrow;
        if ((num1 - borrow) >= num2)
            borrow = 0;
        else
        {
            sub += 10;
            borrow = 1;
        }
        sub != 0 ? result.push_back(sub % 10 + '0') : result.push_back('0');

        i--, j--;
    }
    return result;
}

string N2Array::multiply(const string &str1, const string &str2)
{
    int i = str1.length() - 1;
    int j = str2.length() - 1;

    // initializing
    string result;
    for (int k = 0; k < i + j + 1; k++)
        result.push_back('0');

    int sum;
    int carry = 0;
    int digits = 0;
    while (j >= 0)
    {
        int num2 = str2[j] - '0';
        if (num2 != 0)
        {
            int index = 0;
            for (int k = i; k >= 0; k--)
            {
                int num1 = str1[k] - '0';
                sum = (result[index + digits] - '0') + num1 * num2 + carry;
                carry = sum / 10;

                result[index + digits] = sum != 0 ? (sum % 10) + '0' : '0';
                index++;
            }
        }
        digits++;
        j--;
    }
    return result;
}

pair<string, string> N2Array::divide(const string &str1, string str2)
{
    // a / 0
    if (str2[0] == '0')
        throw runtime_error("N2Array : cannot divide to 0");

    if (str1[0] == '0')
        return make_pair("0", "0");

    // str1 == str2
    if (compareString(str1, str2, positive, positive).second)
        return make_pair("1", "0");

    string result = "0";
    int len = str1.length() - str2.length();
    for (int i = 0; i < len; i++)
        result.push_back('0');

    string remain;
    string temp = str1;

    // set digits
    int size = str2.length();
    while (temp.length() - 1 >= str2.length())
        str2.push_back('0');

    int digits = str2.length() - size;
    int index = 0;
    while (index < size)
    {
        if (temp[index] - '0' < str2[index] - '0' && digits > 0)
        {
            str2.pop_back();
            digits--;
        }

        while (compareString(temp, str2, positive, positive).first || compareString(temp, str2, positive, positive).second)
        {
            int num = (result[index] - '0') + 1;
            num != 0 ? result[index] = num + '0' : result[index] = '0';
            temp = subtract(temp, str2);
            std::reverse(temp.begin(), temp.end());
        }
        index++;
    }
    remain = temp;
    std::reverse(remain.begin(), remain.end());
    return make_pair(result, remain);
}

string N2Array::modulus(const string &str1, const string &str2)
{
    // str1 == str2
    if (compareString(str1, str2, positive, positive).second)
        return "0";
    // str1 < str2
    if (!compareString(str1, str2, positive, positive).first)
        return str1;

    string remain = divide(str1, str2).second;
    return remain;
}

// compare operation
bool N2Array::compare(const N2Array &ary, OP_Type type) const
{
    string str1 = this->_data;
    string str2 = ary._data;

    if (this->_flag == negative)
        str1.erase(0, 1);
    if (ary._flag == negative)
        str2.erase(0, 1);

    pair<bool, bool> result = compareString(str1, str2, this->_flag, ary._flag);

    if (type == bigger)
        return result.first;
    else
        return result.second;
}

pair<bool, bool> N2Array::compareString(const string &str1, const string &str2, Flag f1, Flag f2)
{
    int len1 = str1.length() - 1;
    int len2 = str2.length() - 1;

    bool result = false;
    bool isEqual = true;

    while (len1 >= 0 || len2 >= 0)
    {
        int num1 = len1 >= 0 ? str1[len1] - '0' : 0;
        int num2 = len2 >= 0 ? str2[len2] - '0' : 0;

        if (num1 > num2)
        {
            isEqual = false;
            if (f1 == negative)
                result = false;
            else
                result = true;
        }
        else if (num1 == num2)
        {
            if (f1 == f2)
            {
                if (isEqual)
                    isEqual = true;
            }
            else
                isEqual = false;
        }
        else
        {
            isEqual = false;
            if (f1 == negative)
                result = true;
            else
                result = false;
        }
        len1--;
        len2--;
    }
    return make_pair(result, isEqual);
}

// operators
inline N2Array N2Array::operator+(const N2Array &ary) const
{
    N2Array result = *this;
    result.operations(ary, OP_Type::sum);
    return result;
}

inline N2Array N2Array::operator-(const N2Array &ary) const
{
    N2Array result = *this;
    result.operations(ary, OP_Type::sub);
    return result;
}

inline N2Array N2Array::operator*(const N2Array &ary) const
{
    N2Array result = *this;
    result.operations(ary, OP_Type::mul);
    return result;
}

inline N2Array N2Array::operator*(const int &num) const
{
    N2Array result = *this;
    N2Array oprand = num;
    result.operations(oprand, OP_Type::mul);
    return result;
}

inline void N2Array::operator*=(const N2Array &ary)
{
    (*this).operations(*this, OP_Type::mul);
}

inline N2Array N2Array::operator/(const N2Array &ary) const
{
    N2Array result = *this;
    result.operations(ary, OP_Type::div);
    return result;
}

inline N2Array N2Array::operator%(const N2Array &ary) const
{
    N2Array result = *this;
    result.operations(ary, mod);
    return result;
}

inline bool N2Array::operator==(const N2Array &ary) const
{
    return this->compare(ary, N2Array::OP_Type::equals);
}

inline bool N2Array::operator!=(const N2Array &ary) const
{
    return !this->compare(ary, N2Array::OP_Type::equals);
}

inline bool N2Array::operator>(const N2Array &ary) const
{
    return this->compare(ary, N2Array::OP_Type::bigger);
}

inline bool N2Array::operator>=(const N2Array &ary) const
{
    bool result1 = this->compare(ary, OP_Type::equals);
    bool result2 = this->compare(ary, N2Array::OP_Type::bigger);
    return result1 || result2;
}

inline bool N2Array::operator<(const N2Array &ary) const
{
    return !this->compare(ary, N2Array::OP_Type::bigger);
}

inline bool N2Array::operator<=(const N2Array &ary) const
{
    bool result1 = this->compare(ary, N2Array::OP_Type::equals);
    bool result2 = this->compare(ary, N2Array::OP_Type::bigger);
    return result1 || !result2;
}
