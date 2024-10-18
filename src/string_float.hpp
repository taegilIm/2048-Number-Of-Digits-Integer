#pragma once

#include "string_int.hpp"
#include <iostream>

#define FLOAT_DIGITS 10
namespace ustr
{
    int FLOATING_COUNT = FLOAT_DIGITS;
    class DLL string_float : public string_int
    {
    private:
        /**
         *  12.345 -> 0.12345, exp = 2, IntString::s = 12345
         *  0.012 -> 0.12, exp = -1
         *  if integer, exp = len
         *
         *  floating_pos is floating point's index which is use to delete points
         */

        int exp = 0;
        int floating_pos;

    public:
        /**
         *  set digits of floatings
         *  this is important for precision
         *  affect to divide, uprintf
         */
        static void precision(const unsigned long long digits);

        string_float(const char *);
        string_float(const string_float &);
        ~string_float();

        void operator=(const string_float &);
        string_float operator+(const string_float &) const;
        string_float &operator+=(const string_float &);
        string_float operator-(const string_float &) const;
        string_float &operator-=(const string_float &);
        string_float operator*(const string_float &) const;
        string_float &operator*=(const string_float &);
        string_float operator/(const string_float &) const;
        string_float &operator/=(const string_float &);

        bool operator>(const string_float &) const;
        bool operator<(const string_float &) const;
        bool operator==(const string_float &) const;
        bool operator!=(const string_float &) const;
        bool operator>=(const string_float &) const;
        bool operator<=(const string_float &) const;

        char &operator[](const int) const;
        friend std::ostream &operator<<(std::ostream &, const string_float &);
        friend std::istream &operator>>(std::istream &, string_float &);

        /**
         *  delete float return to string_int
         */
        string_int floor() const;
        /**
         *  if float is not 0
         *  return to string_int + 1
         */
        string_int ceil() const;
        /**
         *  round on digits of float
         */
        string_float &round(const int digits);
        /**
         *  custom printf operates c-like printf
         *  specialized some options for this string class
         *  recommended use this function than std::cout
         *  only can use integer, float format (not char etc.)
         */
        bool uprintf(const char *format = "%f") const;

    private:
        // uprintf
        int parse_format(const char *, int *) const;

        bool is_float(const char *);
        void find_floating(const char *);
        void set_exp();
        void set_number(const char *);
        /**
         *  shrink_zero
         *  where is shrink from front or back
         *  default is false (from front)
         */
        void shrink_zero(bool where = false);

        void equals_the_length(string_float &);
        void select_operation(const string_float &, bool);
        void product(const string_float &);
        void divide(const string_float &);
        /**
         *  almost same operation of string_int::compare
         */
        bool compare(const string_float &, bool comp_mode = false) const;
        /**
         *  almost same operation of string_int::equals
         */
        bool equals(const string_float &) const;
    };
    DLL std::ostream &operator<<(std::ostream &, const string_float &);
    DLL std::istream &operator>>(std::istream &, string_float &);
}