#pragma once

#ifdef DLL_OUT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <iostream>

namespace ustr
{
    const int BLK_SIZE = 1024;
    class DLL string_int
    {
    protected:
        enum Sign
        {
            positive,
            negative
        };
        // actual data
        char *s;
        int len;
        // use sign instead "-"
        Sign sign = positive;

    public:
        string_int(const char * = "0");
        string_int(const string_int &);
        ~string_int();

        void operator=(const string_int &);
        string_int operator+(const string_int &) const;
        string_int &operator+=(const string_int &);
        string_int operator-(const string_int &) const;
        string_int &operator-=(const string_int &);
        string_int operator*(const string_int &) const;
        string_int &operator*=(const string_int &);
        string_int operator/(const string_int &) const;
        string_int &operator/=(const string_int &);

        bool operator>(const string_int &) const;
        bool operator<(const string_int &) const;
        bool operator==(const string_int &) const;
        bool operator!=(const string_int &) const;
        bool operator>=(const string_int &) const;
        bool operator<=(const string_int &) const;

        char &operator[](const int) const;
        friend std::ostream &operator<<(std::ostream &, const string_int &);
        friend std::istream &operator>>(std::istream &, string_int &);

        /**
         *  decide number is zero
         */
        bool is_zero() const;
        /**
         *  insert zeros to string
         *  where is decide where to copy
         *  default is false (from back)
         */
        void insert_zero(int, bool where = false);
        /**
         *  append string
         *  where is decide where to copy
         *  default is false (from back)
         */
        void append(const char *, bool where = false);
        /**
         *  custom printf operates c-like printf
         *  specialized some options for this string class
         *  recommended use this function than std::cout
         *  only can use integer format (not float, char etc.)
         *
         *  end_point is for string_float (not used in normal)
         */
        bool uprintf(const char *format = "%d", int end_point = -1) const;

    protected:
        /**
         *  decide number is integer
         *  ignore_point is skip the this index
         *  default is -1 (do nothing)
         */
        bool is_integer(const char *, int ignore_point = -1);
        /**
         *  copy string and with sign
         */
        void copy(const char *, Sign);
        void cut(const int);
        void shrink_zero();
        void delete_memory();

        /**
         *  decide what operation (sum or sub) is selected on specific situation
         *  if op == true, sum
         *  op == false, sub
         */
        void select_operation(const string_int &, bool op);
        void product(const string_int &);
        /**
         *  compare string value which is bigger
         *  comp_mode is how compare the string
         *  if comp_mode is true, just compare only value not included sign
         *  default is false (just compare included sign)
         */
        bool compare(const string_int &, bool comp_mode = false) const;
        /**
         *  compare string value which is equal
         */
        bool equals(const string_int &) const;
        // divide sub routine
        bool comp_divide(const char *, int, int) const;
        void sub_divide(const char *, int, int);

    private:
        // uprintf
        int parse_format(const char *, int *) const;
        // operations
        void sum(const string_int &);
        void sub(const string_int &);
        void divide(const string_int &);
    };
    DLL std::ostream &operator<<(std::ostream &, const string_int &);
    DLL std::istream &operator>>(std::istream &, string_int &);
}