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
private:
    //  max digits
    static const int MAXARRAY = 65535;

    enum OP_Type
    {
        sum,
        sub,
        mul,
        div,
        mod,
        bigger,
        smaller,
        equals
    };

    enum Flag
    {
        zero,
        positive,
        negative
    };

    std::string _data;
    Flag _flag;
    int _len;

    // private static method
    static bool isNumber(const std::string &);
    static std::pair<int *, int> intToIntArray(int, Flag);
    static std::string intArrayToString(const int *, int &, Flag);

public:
    // static method
    static std::string &removeZeros(std::string &, bool);

    // constructors
    N2Array();
    N2Array(std::string &);
    N2Array(const int);

    // copy constructor
    N2Array(const N2Array &);

    // destructor
    ~N2Array() {}

    // public methods
    std::string getString() const;
    N2Array power(const N2Array &);

private:
    // private method
    // base operations
    void operations(const N2Array &, OP_Type);
    // operation method (static)
    static void setFlag(std::string &, bool, Flag, Flag, OP_Type);
    static std::string add(const std::string &, const std::string &);
    static std::string subtract(const std::string &, const std::string &);
    static std::string product(const std::string &, const std::string &);
    static std::pair<std::string, std::string> divide(const std::string &, std::string);
    static std::string modulus(const std::string &, const std::string &);

    // compare operation
    bool compare(const N2Array &, OP_Type) const;
    static std::pair<bool, bool> compareString(const std::string &, const std::string &, Flag, Flag);

public:
    // operators
    N2Array operator+(const N2Array &) const;
    N2Array operator-(const N2Array &) const;
    N2Array operator*(const N2Array &) const;
    N2Array operator*(const int &) const;
    void operator*=(const N2Array &);
    N2Array operator/(const N2Array &) const;
    N2Array operator%(const N2Array &) const;

    bool operator==(const N2Array &) const;
    bool operator!=(const N2Array &) const;
    bool operator>(const N2Array &) const;
    bool operator>=(const N2Array &) const;
    bool operator<(const N2Array &) const;
    bool operator<=(const N2Array &) const;

    // iostream operator
    friend std::ostream &operator<<(std::ostream &, const N2Array &);
    friend std::istream &operator>>(const std::istream &, N2Array &);
};