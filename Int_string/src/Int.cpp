#include "Int.hpp"

#include <climits>
#include <iostream>
#include <string>

Int::Int()
    : Int(0)
{
}

Int::Int(long long int l)
    : value(""), sign(true)
{
    if (l < 0) {
        l = -l;
        sign = false;
    } else if (l == 0) {
        value = "0";
    }
    while (l > 0) {
        value = std::to_string(l % 10) + value;
        l /= 10;
    }
}
Int::Int(std::string const& s)
{
    size_t i = 0;
    bool start_found = false;
    bool sign_obtained = false;
    for (char c : s) {
        if ((c > '9' || c < '0') && (sign_obtained && (c != '+' || c != '-')))
            throw Exception::INVALID_STRING;
        if (c == '+' || c == '-') {
            sign = (c == '+');
            sign_obtained = true;
        }
        if ((c == '0' || c == '+' || c == '-') && !start_found)
            i++;
        else if (c != '+' && c != '-')
            start_found = true;
    }
    value = s.substr(i, s.length() - i);
    if (value == "") {
        value = "0";
        sign = true;
    }
    if (!sign_obtained)
        sign = true;
}
Int& Int::operator=(Int const& I)
{
    value = I.value;
    sign = I.sign;
    return *this;
}

bool Int::operator==(Int const& I) const
{
    return (value == I.value && sign == I.sign);
}
bool Int::operator!=(Int const& I) const
{
    return (value != I.value || sign != I.sign);
}
bool Int::operator>(Int const& I) const
{
    if (sign && !I.sign)
        return true;
    else if (!sign && I.sign)
        return false;
    else if (!sign && !I.sign)
        return ((-I) > (-(*this)));

    if (value.length() > I.value.length())
        return true;
    else if (I.value.length() > value.length())
        return false;
    for (size_t i = 0; i < value.length(); i++) {
        if ((int)value[i] != (int)I.value[i])
            return ((int)value[i] > (int)I.value[i]);
    }
    return false;
}
bool Int::operator>=(Int const& I) const
{
    return (*this > I) || (*this == I);
}
bool Int::operator<(Int const& I) const
{
    return (I > *this);
}
bool Int::operator<=(Int const& I) const
{
    return (I >= *this);
}

Int::operator long long int() const
{
    if (*this > Int(LLONG_MAX) || *this < Int(LLONG_MIN))
        throw Exception::TYPECAST_OVERFLOW;
    long long int t = value[0] - '0';
    for (size_t i = 1; i < value.length(); i++) {
        t *= 10;
        t += value[i];
    }
    t *= (sign ? 1 : -1);
    return t;
}
std::string std::to_string(Int const& I)
{
    return I.value;
}
std::ostream& operator<<(std::ostream& o, Int const& a)
{
    o << (a.sign ? "" : "-") << a.value;
    return o;
}

Int Int::operator+() const
{
    return *this;
}
Int Int::operator-() const
{
    Int a = *this;
    a.sign = !a.sign;
    return a;
}
Int Int::operator+(Int const& I) const
{
    if (!sign && I.sign)
        return (I - (-(*this)));
    else if (sign && !I.sign)
        return (*this - (-I));
    else if (!sign && !I.sign)
        return (-((-(*this)) + (-I)));
    else if (I > *this)
        return (I + *this);

    std::string result = "";
    int carry = 0;
    int l = I.value.length(), m = value.length();

    for (int i = l - 1; i >= 0; i--) {
        int d = (value[i + m - l] - '0') + (I.value[i] - '0') + carry;
        if (d > 9) {
            carry = d / 10;
            d %= 10;
        } else
            carry = 0;
        result = std::to_string(d) + result;
    }
    for (int i = m - l - 1; i >= 0; i--) {
        int d = (value[i] - '0') + carry;
        if (d > 9) {
            carry = d / 10;
            d %= 10;
        } else
            carry = 0;
        result = std::to_string(d) + result;
    }
    if (carry != 0)
        result = std::to_string(carry) + result;
    return Int(result);
}
Int Int::operator-(Int const& I) const
{
    if (!sign && I.sign)
        return -(I + (-(*this)));
    else if (sign && !I.sign)
        return (*this + (-I));
    else if (!sign && !I.sign)
        return ((-I) - (-*this));
    else if (I > *this)
        return (-(I - *this));

    std::string result = value;
    int borrow = 0;
    int l = I.value.length(), m = value.length();

    for (int i = l - 1; i >= 0; i--) {
        int d = (int)result[i + m - l] - (int)I.value[i] - borrow;
        if (d < 0) {
            d += 10;
            borrow = 1;
        } else
            borrow = 0;
        result[i + m - l] = '0' + d;
    }
    for (int i = m - l - 1; i >= 0; i--) {
        int d = (int)result[i] - (int)'0' - borrow;
        if (d < 0) {
            d += 10;
            borrow = 1;
        } else
            borrow = 0;
        result[i] = '0' + d;
    }
    return Int(result);
}
Int& Int::operator+=(Int const& I)
{
    *this = *this + I;
    return *this;
}
Int& Int::operator-=(Int const& I)
{
    *this = *this - I;
    return *this;
}
Int Int::operator++()
{
    Int a(*this);
    *this = *this + 1;
    return a;
}
Int Int::operator--()
{
    Int a(*this);
    *this = *this - 1;
    return a;
}
Int& Int::operator++(int)
{
    *this = *this + 1;
    return *this;
}
Int& Int::operator--(int)
{
    *this = *this - 1;
    return *this;
}

Int Int::operator*(Int const& I) const
{
    if (!sign && I.sign)
        return -((-(*this)) * I);
    else if (sign && !I.sign)
        return -((*this) * (-I));
    else if (!sign && !I.sign)
        return ((-*this) * (-I));
    else if (I > *this)
        return (I * (*this));
    if (I.value == "0")
        return Int(0);
    else if (I.value.length() == 1) {
        Int ans(0);
        for (int c = (int)I.value[0] - (int)'0'; c > 0; c--)
            ans += *this;
        return ans;
    } else {
        Int ans(0), temp(0);
        std::string prefix = "";
        for (int i = I.value.length() - 1; i >= 0; i--) {
            temp = (*this) * (I.value[i] - '0');
            ans += Int(temp.value + prefix);
            prefix += "0";
        }
        return ans;
    }
}
Int Int::operator/(Int const& I) const
{
    if (I.value == "0")
        throw Exception::DIV_BY_ZERO;
    if (!sign && I.sign)
        return -((-(*this)) / I);
    else if (sign && !I.sign)
        return -((*this) / (-I));
    else if (!sign && !I.sign)
        return ((-*this) / (-I));
    Int D(*this), ans(0);
    while (D >= I) {
        D -= I;
        ans++;
    }
    return ans;
}
Int& Int::operator*=(Int const& I)
{
    *this = *this * I;
    return *this;
}
Int& Int::operator/=(Int const& I)
{
    *this = *this / I;
    return *this;
}
Int Int::operator%(Int const& I) const
{
    return (*this - I * (*this / I));
}
Int& Int::operator%=(Int const& I)
{
    *this = *this % I;
    return *this;
}

Int Int::pow(Int const& b, Int const& e)
{
    if (b == 0) {
        if (e == 0)
            throw Exception::ZERO_POW_ZERO;
        else if (e < 0)
            throw Exception::DIV_BY_ZERO;
        else
            return 0;
    } else if (e < 0)
        return 0;
    else if (b < 0)
        return pow(-b, e) * (Int(1) - (e % 2) * Int(2));

    Int ans(1), p(e), r(b);
    while (p > 0) {
        if (p % 2 == 1)
            ans *= r;
        r *= r;
        p /= 2;
    }
    return ans;
}

size_t Int::num_digits() const
{
    return value.length();
}
