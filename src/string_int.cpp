#include "string_int.hpp"

#include <iostream>
#include <cstring>

using namespace ustr;

/**
 *  default print function
 */

bool string_int::uprintf(const char *format, int end_point) const
{
    /**
     *  default printf function
     *  it is operated like c-printf
     *  available format is below
     *  (integer only)
     *  "%d"
     *  "%5d" (5 digits)
     *  "%05d" (5 digits but fill 0)
     *  "anything %any anything"
     *  if format is not vaild, "%d" is operated
     *
     *  if success, return true
     *  end point is -1 default (do nothing)
     */

    int seperate[2] = {0};
    int select = parse_format(format, seperate);

    if (end_point == -1)
        std::cout << "\"";

    // prefix
    if (select >= 0)
        for (int i = 0; i < seperate[0]; i++)
            std::cout << format[i];

    if (sign == negative)
        std::cout << "-";

    if (select > 0)
    {
        int count = 0;
        int digits = 1;
        for (int i = seperate[1] - 1; i > seperate[0]; i--)
        {
            count += (format[i] - '0') * digits;
            digits *= 10;
        }

        if (count < len)
            count = len;
        else
        {
            for (int i = 0; i < count - len - 1; i++)
            {
                if (select == 1)
                    std ::cout << ' ';
                else
                    std ::cout << '0';
            }
        }
    }
    if (end_point == 0)
        std::cout << '0';
    for (int i = 0; i < len; i++)
    {
        if (i == end_point)
            break;
        std::cout << s[i];
    }

    // surfix
    if (select >= 0)
        for (int i = seperate[1] + 1; i < (int)strlen(format); i++)
            std::cout << format[i];

    std::cout << "\"" << std::endl;
    return select >= 0 ? true : false;
}

int string_int::parse_format(const char *format, int *seperate) const
{
    int select = 0;
    int index = 0;
    while (format[index] != '\0')
    {
        if (format[index] == '%')
        {
            seperate[0] = index++;
            while (format[index] != 'd')
            {
                if (format[index] == '\0')
                    return -1;
                else if (format[index] > '0' && format[index] <= '9')
                    select = 1;
                else if (format[index] < '0' && format[index] > '9')
                    return -1;
                index++;
            }
            seperate[1] = index;
            if (format[seperate[0] + 1] == '0')
                select = 2;
            return select;
        }
        index++;
    }
    return -1;
}

/**
 *  ### constructor & destructor section (w. base function) ###
 */

string_int::string_int(const char *s)
{
    // if input string is not a number then will "0" is stored
    if (this->is_integer(s))
    {
        if (sign == negative)
            s = s + 1;
        this->copy(s, sign);
    }
    else
        this->copy("0", positive);
    this->shrink_zero();
}

string_int::string_int(const string_int &str)
{
    this->copy(str.s, str.sign);
}

string_int::~string_int()
{
    this->delete_memory();
}

bool string_int::is_integer(const char *s, int ignore_point)
{
    if (s[0] == '-')
    {
        s = s + 1;
        sign = negative;
        ignore_point--;
    }
    else
        sign = positive;

    int index = 0;
    while (s[index] != '\0')
    {
        if (s[index] < '0' || s[index] > '9')
            return false;
        index++;
        if (index == ignore_point)
            index++;
    }
    return true;
}

/* base functions */

void string_int::copy(const char *s, Sign sign)
{
    len = (int)strlen(s);
    this->sign = sign;
    this->s = new char[len + 1];
    strcpy(this->s, s);
}

void string_int::insert_zero(int length, bool where)
{
    /**
     *  (example)
     *  s = 1234
     *  insert_zeros(1, true) -> 01234
     *
     *  mode == true is append from front
     *  mode == false is append from back
     *  default is false
     */

    char src[length + 1];
    src[length] = '\0';
    for (int i = 0; i < length; i++)
        src[i] = '0';
    this->append(src, where);
}

void string_int::append(const char *s, bool where)
{
    /**
     *  append string
     *  where == true, append from front
     *  where == false, append from back
     *  default is false (from back)
     */

    int length = (int)strlen(s) + len;
    char temp[length + 1];

    // from front
    if (where)
    {
        strcpy(temp, s);
        strcat(temp, this->s);
    }
    // from back
    else
    {
        strcpy(temp, this->s);
        strcat(temp, s);
    }
    this->delete_memory();
    this->copy(temp, sign);
}

void string_int::delete_memory()
{
    delete[] s;
    s = nullptr;
}

void string_int::shrink_zero()
{
    int mov_count = 0;
    while (s[mov_count] == '0')
        mov_count++;

    len -= mov_count;
    if (len == 0)
    {
        this->sign = positive;
        len++, mov_count--;
    }

    char ret[len];
    strcpy(ret, &s[mov_count]);
    this->delete_memory();
    this->copy(ret, sign);
}

bool string_int::is_zero() const
{
    int index = 0;
    while (s[index] != '\0')
    {
        if (s[index] != '0')
            return false;
        index++;
    }
    return true;
}

void string_int::cut(const int index)
{
    int temp = index;
    if (temp < 0 || temp > len)
        temp = len;
    s[temp] = '\0';
    char ret[temp + 1];
    strcpy(ret, s);
    this->delete_memory();
    this->copy(ret, sign);
}

/**
 *  ### operator section ###
 */

void string_int::operator=(const string_int &str)
{
    if (this->s != str.s)
        this->delete_memory();
    this->copy(str.s, str.sign);
}

string_int string_int::operator+(const string_int &str) const
{
    string_int ret = *this;
    ret.select_operation(str, true);
    ret.shrink_zero();
    return ret;
}

string_int &string_int::operator+=(const string_int &str)
{
    this->select_operation(str, true);
    this->shrink_zero();
    return *this;
}

string_int string_int::operator-(const string_int &str) const
{
    string_int ret = *this;
    ret.select_operation(str, false);
    ret.shrink_zero();
    return ret;
}

string_int &string_int::operator-=(const string_int &str)
{
    this->select_operation(str, false);
    this->shrink_zero();
    return *this;
}

void string_int::select_operation(const string_int &str, bool op)
{
    /**
     *  this function choose what operation is selected
     *  they converted freely
     *
     *  (some example)
     *  if this->sign is negative and str.sign is positive then operation is sub
     *  -> (-) - (+) -> (-) + (-) so, sum is selected to actual operation
     */

    bool dest_negative = this->sign == negative ? true : false;
    bool src_negative = str.sign == negative ? true : false;

    /**
     *  dest src | sum sub
     *  ------------------
     *   0   0   |  0   0
     *   0   1   |  1   1
     *   1   0   |  1   1
     *   1   1   |  0   0
     *  0 is positive, 1 is negative
     *  dest xor src
     *
     *  op == true is sum, false is sub
     */

    string_int temp = str;
    if (dest_negative ^ src_negative)
    {
        if (dest_negative)
        {
            if (!op)
                this->sum(str);
            else
            {
                temp.sub(*this);
                *this = temp;
            }
        }
        else
            !op ? this->sum(str) : this->sub(str);
    }
    else
    {
        if (dest_negative)
        {
            if (op)
                this->sum(str);
            else
            {
                temp.sub(*this);
                *this = temp;
            }
        }
        else
            op ? this->sum(str) : this->sub(str);
    }
}

void string_int::sum(const string_int &str)
{
    int new_len = this->len >= str.len ? this->len + 1 : str.len + 1;
    // result
    char ret[new_len + 1] = "0";

    int carry = 0;
    int index = new_len - 1;
    int i = this->len - 1;
    int j = str.len - 1;
    while (i >= 0 || j >= 0)
    {
        int num1 = i >= 0 ? this->s[i] - '0' : 0;
        int num2 = j >= 0 ? str.s[j] - '0' : 0;
        int sum = num1 + num2 + carry;
        carry = sum / 10;
        sum = sum % 10;
        ret[index] = sum != 0 ? sum + '0' : '0';
        index--, i--, j--;
    }
    ret[0] = carry != 0 ? '1' : '0';

    this->delete_memory();
    this->copy(ret, this->sign);
}

void string_int::sub(const string_int &str)
{
    int new_len = this->len >= str.len ? this->len + 1 : str.len + 1;
    char ret[new_len + 1] = "0";

    // for compare
    bool bigger = this->compare(str, true);

    int borrow = 0;
    int index = new_len - 1;
    int i = this->len - 1;
    int j = str.len - 1;
    while (i >= 0 || j >= 0)
    {
        int num1 = i >= 0 ? this->s[i] - '0' : 0;
        int num2 = j >= 0 ? str.s[j] - '0' : 0;
        int sub = bigger ? num1 - num2 - borrow : num2 - num1 - borrow;
        if (sub >= 0)
            borrow = 0;
        else
        {
            sub += 10;
            borrow = 1;
        }
        ret[index] = sub != 0 ? sub + '0' : '0';
        index--, i--, j--;
    }

    this->delete_memory();
    this->copy(ret, bigger ? positive : negative);
}

string_int string_int::operator*(const string_int &str) const
{
    string_int ret = *this;
    ret.product(str);
    ret.shrink_zero();
    return ret;
}

string_int &string_int::operator*=(const string_int &str)
{
    this->product(str);
    this->shrink_zero();
    return *this;
}

void string_int::product(const string_int &str)
{
    int new_len = this->len + str.len;
    char ret[new_len + 1] = "0";

    int index = new_len - 1;
    int i = this->len - 1;
    while (i >= 0)
    {
        int dest = this->s[i] - '0';
        int carry = 0;
        int iter = 0;
        for (int j = str.len - 1; j >= 0; j--)
        {
            int src = str.s[j] - '0';
            int temp = ret[index - iter] != '\0' ? ret[index - iter] - '0' : 0;
            int product = dest * src + carry + temp;
            carry = product / 10;
            product = product % 10;
            ret[index - iter] = product != 0 ? product + '0' : '0';
            iter++;
        }
        ret[index - iter] = carry > 0 ? carry + '0' : '0';
        index--, i--;
    }

    this->delete_memory();
    this->copy(ret, sign == str.sign ? positive : negative);
}

string_int string_int::operator/(const string_int &str) const
{
    string_int ret = *this;
    ret.divide(str);
    return ret;
}

string_int &string_int::operator/=(const string_int &str)
{
    this->divide(str);
    return *this;
}

void string_int::divide(const string_int &str)
{
    /**
     *  divide has 2 sub routines named by comp_divide and sub_divide
     *  comp_divide is compare dest to str within blocks size of str.len (from ~ to)
     *  and sub_divide is just subtract size of blocks
     *  mode == false is divide, mode == true is modulus
     *
     *  (example)
     *  dest = 100, src = 90
     *  from = 0, to = 2 because top of numbers is smaller than src
     *  then comp_divide process 100 >= 90 which is true
     *  so, sub_divide is process 100 - 90 = 10 and divide is over
     */

    // exceptions
    if (this->s[0] == '0' || str.s[0] == '0')
    {
        this->delete_memory();
        this->copy("0", positive);
        return;
    }

    if (this->len < str.len)
    {
        this->delete_memory();
        this->copy("0", positive);
        return;
    }

    int new_len = this->len - str.len + 1;
    int iter = new_len;
    char ret[new_len + 1] = "0";

    int index = 0;
    int from = -1;
    int to = str.len - 1;
    while (iter-- > 0)
    {
        int count = 0;
        while (this->comp_divide(str.s, from, to))
        {
            this->sub_divide(str.s, from, to);
            count++;
        }
        ret[index++] = count != 0 ? count + '0' : '0';
        from++, to++;
    }
    sign = sign != str.sign ? negative : positive;
    this->delete_memory();
    this->copy(ret, sign);
    this->shrink_zero();
}

bool string_int::comp_divide(const char *s, int from, int to) const
{
    bool ret = false;
    bool equal = true;
    int index = to - from - 1;
    for (int i = to; i >= from; i--)
    {
        int num1 = i >= 0 ? this->s[i] - '0' : 0;
        int num2 = index >= 0 ? s[index--] - '0' : 0;
        if (num1 > num2)
        {
            ret = true;
            equal = false;
        }
        else if (num1 < num2)
        {
            ret = false;
            equal = false;
        }
        else
        {
            if (equal)
                equal = true;
        }
    }
    return ret || equal;
}

void string_int::sub_divide(const char *s, int from, int to)
{
    int borrow = 0;
    int i = to;
    int j = to - from - 1;
    while (i >= 0 || j >= 0)
    {
        int num1 = i <= this->len - 1 ? this->s[i] - '0' : 0;
        int num2 = j >= 0 ? s[j] - '0' : 0;
        int sub = num1 - num2 - borrow;
        if (sub >= 0)
            borrow = 0;
        else
        {
            sub += 10;
            borrow = 1;
        }
        this->s[i] = sub != 0 ? sub + '0' : '0';
        i--, j--;
    }
}

/**
 *  ### compare operator section ###
 */

bool string_int::operator>(const string_int &str) const
{
    return this->compare(str);
}

bool string_int::operator<(const string_int &str) const
{
    return !this->compare(str) && !this->equals(str);
}

bool string_int::operator==(const string_int &str) const
{
    return this->equals(str);
}

bool string_int::operator!=(const string_int &str) const
{
    return !this->equals(str);
}

bool string_int::operator>=(const string_int &str) const
{
    return this->compare(str) || this->equals(str);
}

bool string_int::operator<=(const string_int &str) const
{
    return !this->compare(str) || this->equals(str);
}

bool string_int::compare(const string_int &str, bool comp_mode) const
{
    /**
     *  if comp_mode is true, they compared real value except sign
     *  but if mode is false, just compare each with sign
     *  default is false
     */

    bool dest_negative = this->sign == negative && !comp_mode;
    bool src_negative = str.sign == negative && !comp_mode;

    // if both sign are diffrent then dest's sign is return (p == true, n == false)
    if (dest_negative != src_negative)
        return !dest_negative;

    int i = this->len - 1;
    int j = str.len - 1;
    // if both length of string are diffrent then dest's sign is return (if i > j, p == true, n == false)
    if (i != j)
        return i > j ? !dest_negative : dest_negative;

    // same length and same sign
    bool ret = false;
    while (i >= 0 && j >= 0)
    {
        int dest = this->s[i] - '0';
        int src = str.s[i] - '0';
        if (dest > src)
            ret = !dest_negative;
        else
            ret = dest_negative;
        i--, j--;
    }
    return ret;
}

bool string_int::equals(const string_int &str) const
{
    if (this->sign != str.sign)
        return false;

    int i = this->len - 1;
    int j = str.len - 1;
    if (i != j)
        return false;

    bool ret = true;
    // i == j
    while (i >= 0)
    {
        int dest = this->s[i] - '0';
        int src = str.s[i] - '0';
        if (dest == src)
        {
            if (ret)
                ret = true;
        }
        else
            ret = false;
        i--, j--;
    }
    return ret;
}

/**
 *  other operators
 */

char &string_int::operator[](const int index) const
{
    int temp = index;
    if (index < 0 || index >= len)
        temp = 0;
    return s[temp];
}

namespace ustr
{
    std::ostream &operator<<(std::ostream &out, const string_int &str)
    {
        out << str.uprintf("%d");
        return out;
    }

    std::istream &operator>>(std::istream &in, string_int &str)
    {
        char temp[BLK_SIZE];
        in >> temp;
        str = string_int(temp);
        return in;
    }
}