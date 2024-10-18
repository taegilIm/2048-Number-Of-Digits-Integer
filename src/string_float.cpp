#include "string_float.hpp"

#include <iostream>
#include <cstring>

using namespace ustr;

/**
 *  default printf function
 */

/**
 *  can operate same thing of string_int::uprintf
 *  and also print float to int (%d)
 *
 *  below is available format
 *  "%d"
 *  "%5d"
 *  "%05d"
 *  "%f"
 *  "%5f"
 *  "%.5f"
 *  "%.05f"
 *  "%5.5f"
 *  "%05.5f"
 *  "%05.05f"
 *  "anything %any anything"
 *  if format is not valid, default is "%.FLOAT_DIGITSf"
 *
 *  if success, return true
 */
bool string_float::uprintf(const char *format) const
{
    int seperate[2] = {0};
    int select = parse_format(format, seperate);

    std::cout << "\"";

    // prefix
    if (select >= 0)
        for (int i = 0; i < seperate[0]; i++)
            std::cout << format[i];

    if (sign == negative)
        std::cout << "-";

    int start_point = 0;
    int end_point = FLOATING_COUNT;
    // %f
    if (select > 0)
    {
        int count[2] = {0};
        int pos = 1;
        int digits = 1;
        for (int i = seperate[1] - 1; i > seperate[0]; i--)
        {
            if (format[i] == '.')
            {
                pos = 0;
                digits = 1;
                continue;
            }
            count[pos] += (format[i] - '0') * digits;
            digits *= 10;
        }
        start_point = count[0];
        end_point = count[1];
        if (start_point < (exp > 0 ? exp : 1))
            start_point = exp > 0 ? exp : 1;
    }
    // %d
    else if (select == -2)
    {
        int point = exp > 0 ? exp : 0;
        return string_int::uprintf(format, point);
    }

    // start point
    for (int i = 0; i < start_point - (exp > 0 ? exp : 1); i++)
    {
        if (select == 1 || select == 5 || select == 6)
            std::cout << " ";
        else if (select == 2 || select == 7 || select == 8)
            std::cout << "0";
    }

    int float_count = 0;
    if (exp <= 0)
    {
        std::cout << "0.";
        for (int index = exp; index < 0; index++)
        {
            if (float_count >= end_point - 1)
                break;
            std::cout << "0";
            float_count++;
        }
        for (int i = 0; i < len; i++)
        {
            if (float_count >= end_point)
            {
                if (s[i + 1] - '0' >= 5 && s[i + 1] != '\0')
                    std::cout << (s[i] - '0') + 1;
                else
                    std::cout << s[i];
                break;
            }
            std::cout << s[i];
            float_count++;
        }
    }
    else
    {
        // integer
        for (int i = 0; i < exp; i++)
            std::cout << s[i];
        if (exp >= len)
            std::cout << ".0";
        else
            std::cout << ".";
        // float
        for (int i = exp; i < len; i++)
        {
            if (float_count >= end_point)
            {
                if (s[i + 1] - '0' >= 5 && s[i + 1] != '\0')
                    std::cout << (s[i] - '0') + 1;
                else
                    std::cout << s[i];
                break;
            }
            std::cout << s[i];
            float_count++;
        }
    }
    // end point
    for (int i = float_count; i < end_point; i++)
    {
        if (select == 3 || select == 5 || select == 7)
            std::cout << " ";
        else if (select == 4 || select == 6 || select == 8)
            std::cout << "0";
    }

    // surfix
    if (select >= 0)
        for (int i = seperate[1] + 1; i < (int)strlen(format); i++)
            std::cout << format[i];

    std::cout << "\"" << std::endl;

    return select >= 0 ? true : false;
}

int string_float::parse_format(const char *format, int *seperate) const
{
    /**
     *  %d -> -2
     *  %error -> -1
     *  % -> 0
     *  %5 -> 1
     *  %05 -> 2
     *  %.5 -> 3
     *  %.05 -> 4
     *  %5.5 -> 5
     *  %5.05 -> 6
     *  %05.5 -> 7
     *  %05.05 -> 8
     */

    int select[2] = {0};
    int index = 0;
    while (format[index] != '\0')
    {
        if (format[index] == '%')
        {
            seperate[0] = index++;

            bool point = false;
            int point_index = 0;
            int pos = 0;
            while (format[index] != 'f')
            {
                if (format[index] == 'd')
                    return -2;
                else if (format[index] == '.')
                {
                    if (point)
                        return -1;
                    if (format[index + 1] == '\0')
                        return -1;
                    point = true;
                    point_index = index;
                    pos = 1;
                }
                else if (format[index] > '0' && format[index] <= '9')
                    select[pos] = 1;
                index++;
            }
            seperate[1] = index;
            if (format[seperate[0] + 1] == '0')
                select[0] = 2;
            if (format[point_index + 1] == '0')
                select[1] = 2;

            // %5
            if (select[0] == 1 && select[1] == 0)
                return 1;
            // %05
            else if (select[0] == 2 && select[1] == 0)
                return 2;
            // %.5
            else if (select[0] == 0 && select[1] == 1)
                return 3;
            // %.05
            else if (select[0] == 0 && select[1] == 2)
                return 4;
            // %5.5
            else if (select[0] == 1 && select[1] == 1)
                return 5;
            // %5.05
            else if (select[0] == 1 && select[1] == 2)
                return 6;
            // %05.5
            else if (select[0] == 2 && select[1] == 1)
                return 7;
            // %05.05
            else if (select[0] == 2 && select[1] == 2)
                return 8;
            return 0;
        }
        index++;
    }
    return -1;
}

/**
 *  static function section
 */

void string_float::precision(const unsigned long long digits)
{
    FLOATING_COUNT = digits;
}

/**
 *  constructor & destructor section (w. base function)
 */

string_float::string_float(const char *s = "0")
{
    this->find_floating(s);
    if (this->is_float(s))
    {
        if (sign == negative)
        {
            s = s + 1;
            floating_pos--;
        }

        this->set_number(s);
        this->set_exp();
    }
    else
        this->copy("0", positive);
    this->string_int::shrink_zero();
}

string_float::string_float(const string_float &str)
{
    this->exp = str.exp;
    this->copy(str.s, str.sign);
}

string_float::~string_float()
{
}

/* base functions */

void string_float::find_floating(const char *s)
{
    int index = 0;
    while (s[index] != '\0')
    {
        if (s[index] == '.')
        {
            if (s[index + 1] == '\0')
            {
                // not a number (NaN)
                floating_pos = -2;
                exp = 0;
                return;
            }
            floating_pos = index;
            return;
        }
        index++;
    }
    // 100, 12345 integer case
    floating_pos = -1;
}

bool string_float::is_float(const char *s)
{
    if (floating_pos == 0 || floating_pos == -2)
        return false;
    // floating_pos is ignoring point that is floating point
    return this->is_integer(s, floating_pos);
}

void string_float::set_number(const char *s)
{
    /**
     *  delete "." char inside string
     *  1.00 -> 0.100 -> 100 and copy to member
     */

    int len = (int)strlen(s);
    char org[len + 1];
    strcpy(org, s);
    if (floating_pos > 0)
    {
        org[floating_pos] = '\0';
        strcat(org, &s[floating_pos + 1]);
    }
    this->copy(org, sign);
}

void string_float::set_exp()
{
    /**
     *  floating_pos is point's index
     *  -0.123 -> floating_pos = 2, exp = 0
     */

    // 0.xx
    if (s[0] == '0')
    {
        int index = 1;
        int temp_pos = floating_pos;
        // 0.00000000xxx
        while (s[index++] == '0')
            temp_pos--;

        exp = temp_pos - 1;
    }
    // 1.xx
    else
    {
        if (floating_pos == -1)
            exp = len;
        else
            exp = floating_pos;
    }
}

void string_float::shrink_zero(bool where)
{
    /**
     *  if where == true, shrink from front
     *  where == false, back
     *  default is false
     */

    int mov_count = 0;
    if (!where)
    {
        while (s[mov_count] == '0')
            mov_count++;
        exp -= mov_count;
    }
    else
    {
        while (s[len - mov_count - 1] == '0')
            mov_count++;
        if (len != 1)
        {
            if (len - mov_count >= exp)
                s[len - mov_count] = '\0';
            else
                s[exp] = '\0';
        }
    }
    len -= mov_count;
    if (len == 0)
    {
        this->sign = positive;
        len++, mov_count--;
        exp = 0;
    }

    char ret[len];
    if (!where)
        strcpy(ret, &s[mov_count]);
    else
        strcpy(ret, s);
    this->delete_memory();
    this->copy(ret, sign);
}

/**
 *  public method sections
 */

string_int string_float::floor() const
{
    char ret[len + 1];
    strcpy(ret, s);
    if (exp > 0)
        ret[exp] = '\0';
    else
    {
        ret[0] = '0';
        ret[1] = '\0';
    }
    return string_int(ret);
}

string_int string_float::ceil() const
{
    char ret[len + 1];
    strcpy(ret, s);
    int index = exp >= 0 ? exp : 0;
    while (ret[index] != '\0')
    {
        if (ret[index] > '0')
        {
            if (exp <= 0)
            {
                ret[0] = '1';
                ret[1] = '\0';
            }
            else
            {
                ret[exp - 1] += 1;
                ret[exp] = '\0';
            }
            break;
        }
        index++;
    }
    return string_int(ret);
}

string_float &string_float::round(const int floating)
{
    char ret[len + 1];
    strcpy(ret, s);
    int temp = floating - 1;
    if (temp < 0)
        temp = 0;

    // set position
    if (exp >= 0)
    {
        if (temp > len - exp)
            temp = len - 1;
        else
            temp += exp;
    }
    else
    {
        if (temp + exp > len - 1)
            temp = len - 1;
        else if (temp + exp <= 0)
            temp = 0;
        else
            temp += exp;
    }
    // insert value
    if (temp == 0)
    {
        if (exp > 0)
        {
            if (ret[exp] >= '5')
                ret[exp - 1] += 1;
            ret[exp] = '\0';
        }
        else
        {
            if (exp == 0)
            {
                if (ret[0] >= '5')
                    ret[0] = '1';
            }
            else
                ret[0] = '0';
            ret[1] = '\0';
            exp = 0;
        }
    }
    else
    {
        if (ret[temp] >= '5')
            ret[temp - 1] += 1;
        ret[temp] = '\0';
    }
    this->delete_memory();
    this->copy(ret, sign);
    return *this;
}

/**
 *  operation sections
 */

void string_float::operator=(const string_float &str)
{
    this->exp = str.exp;
    this->string_int::operator=(str);
}

string_float string_float::operator+(const string_float &str) const
{
    string_float ret = *this;
    ret.select_operation(str, true);
    return ret;
}

string_float &string_float::operator+=(const string_float &str)
{
    this->select_operation(str, true);
    return *this;
}

string_float string_float::operator-(const string_float &str) const
{
    string_float ret = *this;
    ret.select_operation(str, false);
    return ret;
}

string_float &string_float::operator-=(const string_float &str)
{
    this->select_operation(str, false);
    return *this;
}

void string_float::select_operation(const string_float &str, bool op)
{
    /**
     *  equals digits of number
     *  and then select what operation is doing
     */

    string_float src = str;
    this->equals_the_length(src);
    this->exp = this->exp >= str.exp ? this->exp + 1 : str.exp + 1;
    this->string_int::select_operation(src, op);
    this->shrink_zero();
    this->shrink_zero(true);
}

void string_float::equals_the_length(string_float &str)
{
    /**
     *  set number size evenly
     *  (example)
     *  12345, exp = 2 (12.345)
     *  1234, exp = 0 (0.1234)
     *  -> 123450
     *     001234
     *
     *  first, set integer digits
     *  and then equals float digits
     */

    // integer
    if (this->exp > str.exp)
        str.insert_zero(this->exp - str.exp, true);
    else if (this->exp < str.exp)
        this->insert_zero(str.exp - this->exp, true);

    // float
    if (this->len > str.len)
        str.insert_zero(this->len - str.len);
    else if (this->len < str.len)
        this->insert_zero(str.len - this->len);
}

string_float string_float::operator*(const string_float &str) const
{
    string_float ret = *this;
    ret.product(str);
    return ret;
}

string_float &string_float::operator*=(const string_float &str)
{
    this->product(str);
    return *this;
}

void string_float::product(const string_float &str)
{
    this->exp += str.exp;
    this->string_int::product(str);
    this->shrink_zero();
    this->shrink_zero(true);
}

string_float string_float::operator/(const string_float &str) const
{
    string_float ret = *this;
    ret.divide(str);
    return ret;
}

string_float &string_float::operator/=(const string_float &str)
{
    this->divide(str);
    return *this;
}

void string_float::divide(const string_float &str)
{
    if (this->compare(str, true))
        this->exp -= str.exp - 1;
    else
        this->exp -= str.exp;

    // digits of float
    int iter = FLOATING_COUNT;
    char ret[iter + 2] = "0";

    int index = 0;
    int from = -1;
    int to = str.len - 1;
    while (iter-- >= 0)
    {
        int count = 0;
        while (this->comp_divide(str.s, from, to) && this->len >= str.len)
        {
            this->sub_divide(str.s, from, to);
            count++;
        }
        ret[index++] = count != 0 ? count + '0' : '0';
        if (this->len >= str.len)
            from++, to++;
        this->append("0");
    }
    this->delete_memory();
    this->copy(ret, sign);
    this->string_int::shrink_zero();
    this->shrink_zero(true);
}

bool string_float::operator>(const string_float &str) const
{
    return this->compare(str);
}

bool string_float::operator<(const string_float &str) const
{
    return !this->compare(str) && !this->equals(str);
}

bool string_float::operator==(const string_float &str) const
{
    return this->equals(str);
}

bool string_float::operator!=(const string_float &str) const
{
    return !this->equals(str);
}

bool string_float::operator>=(const string_float &str) const
{
    return this->compare(str) || this->equals(str);
}

bool string_float::operator<=(const string_float &str) const
{
    return !this->compare(str) || this->equals(str);
}

bool string_float::compare(const string_float &str, bool comp_mode) const
{
    if (this->exp > str.exp)
        return this->sign != negative;
    else if (this->exp < str.exp)
        return this->sign == negative;

    /**
     *  almost same as string_int::compare
     */

    bool dest_negative = this->sign == negative && !comp_mode;
    bool src_negative = str.sign == negative && !comp_mode;

    // if both sign are diffrent then dest's sign is return (p == true, n == false)
    if (dest_negative != src_negative)
        return !dest_negative;

    int i = this->len - 1;
    int j = str.len - 1;
    bool ret = false;
    while (i >= 0 || j >= 0)
    {
        bool is_long = i >= j;
        int dest = is_long ? this->s[i--] - '0' : 0;
        int src = is_long ? 0 : str.s[j--];
        if (dest > src)
            ret = !dest_negative;
        else
            ret = dest_negative;
    }
    return ret;
}

bool string_float::equals(const string_float &str) const
{
    if (this->exp != str.exp)
        return false;

    /**
     *  almost same as string_int::equals
     */

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

char &string_float::operator[](const int index) const
{
    int temp = index;
    if (index < 0 || index >= len)
        temp = 0;
    return s[temp];
}

namespace ustr
{
    std::ostream &operator<<(std::ostream &out, const string_float &str)
    {
        out << str.uprintf("%f");
        return out;
    }

    std::istream &operator>>(std::istream &in, string_float &str)
    {
        char temp[BLK_SIZE];
        in >> temp;
        str = string_float(temp);
        return in;
    }
}