#ifndef INT_H
#define INT_H

#include <iostream>
#include <string>
#include <vector>

typedef unsigned long long int ull;

class Int;
std::ostream& operator<<(std::ostream&, Int const&);
namespace std {
std::string to_string(Int const&);
Int stoI(std::string);
}

class Int {
private:
    bool sign; // true='+', false='-', 0 has true
    std::vector<ull> value; // data stored in base K, value[0] is least significant and value[-1] is most significant

    ull mod2() const;
    void divby2();

public:
    Int();
    Int(long long int);
    Int& operator=(Int const&);

    bool operator==(Int const&) const;
    bool operator!=(Int const&) const;
    bool operator>(Int const&) const;
    bool operator>=(Int const&) const;
    bool operator<(Int const&) const;
    bool operator<=(Int const&) const;

    friend std::ostream& operator<<(std::ostream&, Int const&);
    friend std::string std::to_string(Int const&);
    friend Int std::stoI(std::string);
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
