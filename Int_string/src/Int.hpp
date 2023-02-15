#ifndef INT_HPP
#define INT_HPP

#include <iostream>
#include <string>

class Int;
namespace std {
std::string to_string(Int const&);
}
std::ostream& operator<<(std::ostream&, Int const&);

class Int {
protected:
    bool sign; // true='+', false='-', 0 has true
    std::string value; // no leading zeros (unless number itself is zero, in which case "0"

public:
    Int();
    Int(long long int);
    Int(std::string const&);
    Int& operator=(Int const&);

    enum class Exception {
        INVALID_STRING,
        DIV_BY_ZERO,
        ZERO_POW_ZERO,
        TYPECAST_OVERFLOW
    };

    bool operator==(Int const&) const;
    bool operator!=(Int const&) const;
    bool operator>(Int const&) const;
    bool operator>=(Int const&) const;
    bool operator<(Int const&) const;
    bool operator<=(Int const&) const;

    explicit operator long long int() const;
    friend std::string std::to_string(Int const&);
    friend std::ostream& operator<<(std::ostream&, Int const&);

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

    static Int pow(Int const& b, Int const& e);
    size_t num_digits() const;
};

#endif // INT_HPP
