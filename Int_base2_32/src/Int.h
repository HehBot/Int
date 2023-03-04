#ifndef INT_H
#define INT_H

#include <iostream>
#include <string>
#include <vector>

typedef unsigned long long int ull;

class Int;
namespace std {
std::string to_string(Int const&);
}
std::ostream& operator<<(std::ostream&, Int const&);

class Int {
private:
    bool sign; // true='+', false='-', 0 has true
    std::vector<ull> value; // data stored in base 2**b, value[0] is least significant and value[l - 1] is most significant

    ull modK() const;
    ull mod2() const;
    void divby2();

public:
    Int();
    Int(long long int);
    Int(std::string);
    Int& operator=(Int const&);

    enum class Exception {
        INVALID_STRING,
        DIV_BY_ZERO,
        ZERO_POW_ZERO,
        TYPECAST_OVERFLOW,
        UNIMPLEMENTED
    };

    bool operator==(Int const&) const;
    bool operator!=(Int const&) const;
    bool operator>(Int const&) const;
    bool operator>=(Int const&) const;
    bool operator<(Int const&) const;
    bool operator<=(Int const&) const;

    friend std::string std::to_string(Int const&);
    friend std::ostream& operator<<(std::ostream&, Int const&);
    explicit operator long long int() const;

    Int operator+() const;
    Int operator-() const;
    Int operator+(Int const&) const;
    Int operator-(Int const&) const;
    Int& operator+=(Int const&);
    Int& operator-=(Int const&);
    Int operator++();
    Int operator--();
    Int& operator++(int);
    Int& operator--(int);

    Int operator*(Int const&) const;
    Int operator/(Int const&) const;
    Int& operator*=(Int const&);
    Int& operator/=(Int const&);
    Int operator%(Int const&) const;
    Int& operator%=(Int const&);

    static Int pow(Int const&, Int);
};

#endif // INT_H
