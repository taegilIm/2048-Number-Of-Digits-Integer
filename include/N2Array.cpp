#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>

#include "N2Array.hpp"

using namespace std;

// static methods
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

string &N2Array::removeZeros(string &str, bool pos)
{
    /** true -> from front
     *  false -> from back
     */
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

// constructor
N2Array::N2Array(const string &value) : _data(value)
{
    if (value.length() > 2048)
    {
        throw runtime_error("N2Array too big value exception : string must be lower than 2048 bytes");
    }
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

// public method
inline string N2Array::getString() const
{
    return this->_data;
}

// base operations
void N2Array::operation(const N2Array &ary1, const N2Array &ary2, N2Array::OP_Type type)
{
    string result;
    int carry = 0;
    int borrow = 0;

    // set up the sign
    bool negative = ary2 > ary1;
    ary1 == ary2 ? this->_flag = Flag::zero : this->_flag;
    negative ? this->_flag = Flag::negative : this->_flag = Flag::positive;

    int i = ary1._len - 1;
    int j = ary2._len - 1;

    int *temp_multi;
    int size;
    int digits;
    if (type == multiply)
    {
        if ((i + j) > 2048)
        {
            throw runtime_error("N2Array operation multiply has created too big number");
        }

        temp_multi = new int[i + j + 2];
        for (int k = 0; k < i + j + 2; k++)
        {
            temp_multi[k] = 0;
        }

        size = i + j + 2;
        digits = 0;
    }

    string str1 = ary1.getString();
    string str2 = ary2.getString();

    while (i >= 0 || j >= 0)
    {
        int num1 = i >= 0 ? str1[i] - '0' : 0;
        int num2 = j >= 0 ? str2[j] - '0' : 0;

        int sum;
        // operation type
        switch (type)
        {
        case add:
            sum = num1 + num2 + carry;
            carry = sum / 10;
            sum != 0 ? result.push_back(sum % 10 + '0') : result.push_back('0');
            break;

        case subtract:
            // n1 - n2 or n2 - n1
            negative ? sum = num2 - num1 - borrow : sum = num1 - num2 - borrow;
            if (sum >= 0)
                borrow = 0;
            else
            {
                sum += 10;
                borrow = 1;
            }

            sum != 0 ? result.push_back(sum % 10 + '0') : result.push_back('0');
            break;

        case multiply:
            int index = 0;
            if (num2 != 0)
            {
                /**
                 * 12345 * 2 :
                 * 5 * 2 -> 0
                 * 4 * 2 -> 8 + 1
                 * 3 * 2 -> 6
                 * 2 * 2 -> 4
                 * 1 * 2 -> 2
                 */
                for (int k = ary1._len - 1; k >= 0; k--)
                {
                    num1 = ary1._data[k] - '0';
                    sum = num1 * num2 + carry;
                    carry = sum / 10;

                    temp_multi[index + digits] += sum % 10;
                    index++;
                }
            }
            digits++;
            break;
        }

        i--;
        j--;
    }

    if (type == subtract)
    {
        if (negative)
            result.push_back('-');
    }
    else if (type == multiply)
    {
        // sum of all digits
        int mul_carry = 0;
        for (int k = 0; k <= size; k++)
        {
            int mul_sum = temp_multi[k] + mul_carry;
            mul_carry = mul_sum / 10;
            temp_multi[k] = mul_sum % 10;
        }

        // - * - or + * - or + * +
        if (ary1._flag == ary2._flag)
            result = N2Array::intArrayToString(temp_multi, size, Flag::positive);
        else
            result = N2Array::intArrayToString(temp_multi, size, Flag::negative);

        N2Array::removeZeros(result, false);
        delete[] temp_multi;
    }

    std::reverse(result.begin(), result.end());

    this->_data = result;
}

bool N2Array::compare(const N2Array &ary, N2Array::OP_Type type) const
{
    string ary1 = this->_data;
    string ary2 = ary._data;

    if (ary1[0] == '-')
        ary1.erase(0, 1);
    if (ary2[0] == '-')
        ary2.erase(0, 1);

    int len1 = this->_len - 1;
    int len2 = ary._len - 1;

    int sign;
    /** +, +: 0
     *  +, -: 1
     *  -, +: 2
     *  -, -: 3
     *  +, 0: 4
     *  -, 0: 5
     *  0, +: 6
     *  0, -: 7
     *  0, 0: 8
     */
    if (this->_flag == ary._flag)
    {
        if (this->_flag == positive)
            sign = 0;
        else if (this->_flag == negative)
            sign = 3;
        else
            sign = 8;
    }
    else
    {
        if (this->_flag == positive)
            ary._flag == negative ? sign = 1 : sign = 4;
        else if (this->_flag == negative)
            ary._flag == positive ? sign = 2 : sign = 5;
        else
            ary._flag == positive ? sign = 6 : sign = 7;
    }

    bool result;
    bool is_equal;
    while (len1 >= 0 || len2 >= 0)
    {
        int num1 = len1 >= 0 ? ary1[len1] - '0' : 0;
        int num2 = len2 >= 0 ? ary2[len2] - '0' : 0;

        switch (sign)
        {
        case 1:
        case 4:
        case 7:
            is_equal = false;
            result = true;
            break;
        case 2:
        case 5:
        case 6:
            is_equal = false;
            result = false;
            break;
        case 0:
        case 3:
        case 8:
            num1 == num2 ? is_equal = true : is_equal = false;
            if (sign == 3)
                num1 < num2 ? result = true : result = false;
            else
                num1 > num2 ? result = true : result = false;
            break;
        }

        len1--;
        len2--;
    }

    switch (type)
    {
    case bigger:
        return result;

    case smaller:
        return !result;

    case equals:
        return is_equal;

    default:
        return false;
    }
}

// operators
inline N2Array N2Array::operator+(const N2Array &ary) const
{
    N2Array result;
    result.operation(*this, ary, N2Array::OP_Type::add);
    return result;
}

inline N2Array N2Array::operator-(const N2Array &ary) const
{
    N2Array result;
    result.operation(*this, ary, N2Array::OP_Type::subtract);
    return result;
}

inline N2Array N2Array::operator*(const N2Array &ary) const
{
    N2Array result;
    result.operation(*this, ary, N2Array::OP_Type::multiply);
    return result;
}

inline N2Array N2Array::operator*(const int &num) const
{
    N2Array result = *this;
    N2Array oprand = num;
    result.operation(*this, oprand, N2Array::OP_Type::multiply);
    return result;
}

inline void N2Array::operator*=(const N2Array &ary)
{
    (*this).operation(*this, ary, N2Array::OP_Type::multiply);
}

inline bool N2Array::operator==(const N2Array &ary) const
{
    return this->compare(ary, N2Array::OP_Type::equals);
}

inline bool N2Array::operator>(const N2Array &ary) const
{
    return this->compare(ary, N2Array::OP_Type::bigger);
}

inline bool N2Array::operator>=(const N2Array &ary) const
{
    bool result = this->compare(ary, OP_Type::equals);
    result = this->compare(ary, N2Array::OP_Type::bigger);
    return result;
}

inline bool N2Array::operator<(const N2Array &ary) const
{
    return this->compare(ary, N2Array::OP_Type::smaller);
}

inline bool N2Array::operator<=(const N2Array &ary) const
{
    bool result = this->compare(ary, N2Array::OP_Type::equals);
    result = this->compare(ary, N2Array::OP_Type::smaller);
    return result;
}
