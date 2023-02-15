#include "Int.hpp"

#include <climits>
#include <cstdio> // sprintf
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static constexpr ull ull_pow(ull base, ull exp)
{
    ull ans(1), r(base);
    while (exp > 0) {
        if (exp & 1)
            ans *= r;
        r *= r;
        exp /= 2;
    }
    return ans;
}

static size_t constexpr k = 9;
static ull constexpr B = ull_pow(10, k); // working in base B
// (10**k - 1) * (10**k - 1) + (10**k - 1) = 10**k(10**k - 1) < 2**(8*sizeof(ull))
// so k <= 9

std::ostream& operator<<(std::ostream& o, Int const& a)
{
    return (o << std::to_string(a));
}
std::string std::to_string(Int const& I)
{
    Int a(I);
    a.sign = true;

    char* buf = new char[k + 1];

    std::string fmt = "%0" + std::to_string(k) + "llu";

    std::string s(std::to_string(a.value[a.value.size() - 1]));
    for (int i = I.value.size() - 2; i >= 0; i--) {
        sprintf(buf, fmt.c_str(), a.value[i]);
        s += std::string(buf);
    }

    delete[] buf;

    if (!I.sign)
        s = "-" + s;
    return s;
}
Int std::stoI(std::string s)
{
    Int ans;
    ans.value.clear();

    size_t i = 0;
    bool start_found = false;
    bool sign_obtained = false;
    for (char c : s) {
        if ((c > '9' || c < '0') && (sign_obtained && (c != '+' || c != '-'))) {
            std::cerr << "Invalid string\n";
            exit(-1);
        }
        if (c == '+' || c == '-') {
            ans.sign = (c == '+');
            sign_obtained = true;
        }
        if ((c == '0' || c == '+' || c == '-') && !start_found)
            i++;
        else if (c != '+' && c != '-')
            start_found = true;
    }
    s = s.substr(i);

    if (s == "") {
        s = "0";
        ans.sign = true;
        ans.value.push_back(0);
        return ans;
    }
    if (!sign_obtained)
        ans.sign = true;

    std::string w = s.substr(0, s.length() % k);
    if (w != "")
        ans.value.push_back(std::stoll(w));

    for (int i = s.length() - k; i >= int(s.length() % k); i -= k)
        ans.value.push_back(std::stoll(s.substr(i, k)));

    return ans;
}

ull Int::mod2() const
{
    return value[0] & 1;
}
void Int::divby2()
{
    value[0] >>= 1;
    for (int i = 1; i < value.size(); i++) {
        value[i - 1] += (B >> 1) * (value[i] & 1);
        value[i] >>= 1;
    }
    if (value[value.size() - 1] == 0 && value.size() > 1)
        value.resize(value.size() - 1);
}

Int::Int()
    : sign(true)
{
    value.push_back(0);
}
Int::Int(long long int l)
    : sign(true)
{
    if (l < 0) {
        l = -l;
        sign = false;
    }
    if (ull(l) < B)
        value.push_back(ull(l));
    else {
        value.push_back(ull(l) % B);
        value.push_back(ull(l) / B);
    }
}
Int& Int::operator=(Int const& I)
{
    value = I.value;
    sign = I.sign;
    return *this;
}

bool Int::operator==(Int const& I) const
{
    return (sign == I.sign && value == I.value);
}
bool Int::operator!=(Int const& I) const
{
    return (sign != I.sign || value != I.value);
}
bool Int::operator>(Int const& I) const
{
    if (sign && !I.sign)
        return true;
    else if (!sign && I.sign)
        return false;
    else if (!sign && !I.sign)
        return ((-I) > (-(*this)));

    if (value.size() > I.value.size())
        return true;
    else if (I.value.size() > value.size())
        return false;
    for (int i = value.size() - 1; i >= 0; i--) {
        if (value[i] != I.value[i])
            return (value[i] > I.value[i]);
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
        throw std::overflow_error("Typecast overflow for long long int");
    long long int t = value[0] + (value[1] * B);
    return t * (sign ? 1 : -1);
}

Int Int::operator+() const
{
    return *this;
}
Int Int::operator-() const
{
    Int a = *this;
    if (*this != 0)
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

    Int ret;
    ret.value.clear();

    ull carry = 0, d;
    int l = I.value.size(), m = value.size();

    ret.value.reserve(m);

    for (int i = 0; i < l; i++) {
        d = value[i] + I.value[i] + carry;
        carry = (d >= B);
        d -= B * carry;
        ret.value.push_back(d);
    }
    for (int i = l; i < m; i++) {
        d = value[i] + carry;
        carry = (d >= B);
        d -= B * carry;
        ret.value.push_back(d);
    }
    if (carry != 0)
        ret.value.push_back(carry);

    return ret;
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

    Int ret;
    ret.value.clear();

    ull borrow = 0;
    int l = I.value.size(), m = value.size();

    ret.value.reserve(m);

    for (int i = 0; i < l; i++) {
        ull d = (value[i] + (B - I.value[i])) - borrow;
        borrow = (d < B);
        d -= B * (1 - borrow);
        ret.value.push_back(d);
    }
    /*
    if (d >= B) {
        borrow = 0;
        d -= B;
    }
    else
        borrow = 1;
     */
    for (int i = l; i < m; i++) {
        ull d = value[i] + (B - borrow);
        borrow = (d < B);
        d -= B * (1 - borrow);
        ret.value.push_back(d);
    }
    int i = ret.value.size() - 1;
    while (ret.value[i] == 0)
        i--;
    ret.value.resize(i + 1);

    return ret;
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
    else if (I == 0 || *this == 0)
        return 0;
    else if (I == 1)
        return *this;

    Int ans;
    ull carry = 0;

    ans.value.clear();
    std::vector<ull>& conv = ans.value;

    for (int i = 0; i < I.value.size(); i++) {
        ull residue = carry;
        carry = 0;
        for (int j = 0; j <= i; j++) {
            residue += I.value[j] * value[i - j];
            if (residue >= B) {
                carry += residue / B;
                residue %= B;
            }
        }
        conv.push_back(residue);
    }

    for (int i = I.value.size(); i < value.size(); i++) {
        ull residue = carry;
        carry = 0;
        for (int j = 0; j < I.value.size(); j++) {
            residue += I.value[j] * value[i - j];
            if (residue >= B) {
                carry += residue / B;
                residue %= B;
            }
        }
        conv.push_back(residue);
    }

    for (int i = value.size(); i < value.size() - 1 + I.value.size(); i++) {
        ull residue = carry;
        carry = 0;
        for (int j = i + 1 - value.size(); j < I.value.size(); j++) {
            residue += I.value[j] * value[i - j];
            if (residue >= B) {
                carry += residue / B;
                residue %= B;
            }
        }
        conv.push_back(residue);
    }

    if (carry != 0)
        conv.push_back(carry);

    return ans;
}

Int Int::operator/(Int const& I) const
{
    if (I.value.size() == 1 && I.value[0] == 0)
        throw std::invalid_argument("Division by zero");
    if (!sign && I.sign)
        return -((-(*this)) / I);
    else if (sign && !I.sign)
        return -((*this) / (-I));
    else if (!sign && !I.sign)
        return ((-*this) / (-I));
    else if (I == 1)
        return *this;
    else if (*this == 0)
        return *this;

    Int ql(0), qh(*this);

    // quotient at this point is never *this (for that either *this == 0 or I == 1)

    while (true) {
        Int q = ql + qh;
        q.divby2();
        if (*this >= (q + 1) * I)
            ql = q;
        else if (*this < q * I)
            qh = q;
        else
            return q;
    }
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

Int Int::pow(Int const& base, Int exp)
{
    if (base == 0) {
        if (exp == 0)
            throw std::invalid_argument("Zero raised to zero");
        else if (exp < 0)
            throw std::invalid_argument("Division by zero");
        else
            return 0;
    } else if (exp < 0)
        return 0;
    else if (base < 0)
        return pow(-base, exp) * (1 - (exp.mod2()) * 2);

    Int ans(1), r(base);
    while (exp > 0) {
        if (exp.mod2())
            ans *= r;
        r *= r;
        exp.divby2();
    }
    return ans;
}
