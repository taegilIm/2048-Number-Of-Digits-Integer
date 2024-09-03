#pragma once

#include <string>
#include <utility>

#ifdef DEFINE_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

class DLL_API N2Array
{
public:
    enum OP_Type
    {
        add = 0,
        subtract = 1,
        multiply = 2,
        bigger = 3,
        smaller = 4,
        equals = 5
    };

private:
    enum Flag
    {
        zero = 0,
        positive = 1,
        negative = -1
    };

    std::string _data;
    Flag _flag;
    int _len;

public:
    // static method
    static std::string intArrayToString(const int *, int &, Flag);
    static std::pair<int *, int> intToIntArray(int, Flag);
    static std::string &removeZeros(std::string &, bool);

    // constructors
    N2Array() : _data("0"), _flag(zero), _len(1)
    {
    }
    N2Array(const std::string &);
    N2Array(const int &);

    // copy constructor
    N2Array(const N2Array &);

    // destructor
    ~N2Array() {}

    std::string getString() const;

private:
    // base operations
    void operation(const N2Array &, const N2Array &, OP_Type);
    bool compare(const N2Array &, OP_Type) const;

public:
    // operators
    N2Array operator+(const N2Array &) const;
    N2Array operator-(const N2Array &) const;
    N2Array operator*(const N2Array &) const;
    N2Array operator*(const int &) const;
    void operator*=(const N2Array &);

    bool operator==(const N2Array &) const;
    bool operator>(const N2Array &) const;
    bool operator>=(const N2Array &) const;
    bool operator<(const N2Array &) const;
    bool operator<=(const N2Array &) const;
};