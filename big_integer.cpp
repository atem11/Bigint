#include "big_integer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <algorithm>

typedef unsigned int one_digit;
typedef unsigned long long two_digit;
typedef big_integer::my_vector my_vector;
using std::vector;
one_digit const MAX = UINT32_MAX;


void big_integer::contraction() {
    one_digit cnst = 0;
    if (sign) {
        cnst = MAX;
    }
    while (data.size() > 0 && data.back() == cnst) {
        data.pop_back();
    }
}

big_integer::big_integer(my_vector const &num, bool f) : data(num), sign(f) {
    contraction();
}

big_integer::big_integer() : sign(0) {}

big_integer::big_integer(int a): data(1), sign(a < 0) {
	data[0] = static_cast<one_digit> (a & MAX);
	contraction();
}

big_integer::big_integer(big_integer const &other) : data(other.data), sign(other.sign) {}

big_integer::big_integer(std::string const & str) : big_integer() {
	if (str.empty()) {
	} else {
		bool sgn = false;
		big_integer ans = 0;
		size_t ind = 0;
		if (str[ind] == '-') {
			sgn = true;
			ind++;
		}
		two_digit st = 9;
		one_digit pw = 1e9;
		for (; ind < str.length(); ind += st) {
			one_digit d = 0;
			size_t k = std::min(static_cast<size_t>(st), str.length() - ind);
			if (k != st) {
				ans *= static_cast<one_digit>(std::pow(10, k));
			} else {
				ans *= pw;
			}
			for (size_t i = 0; i < k; ++i) {
				d *= 10;
				d += (str[ind + i] - '0');
			}
			ans += d;
		}
		if (sgn) {
			ans = -ans;
		}
		swap(ans);
	}
}


big_integer::big_integer(one_digit const &dig): data(1), sign(0) {
	data[0] = dig;
	contraction();
}

void big_integer::swap(big_integer other) {
    using std::swap;
    swap(data, other.data);
    swap(sign, other.sign);
}

void swap(big_integer a, big_integer b) {
    using std::swap;
    swap(a.data, b.data);
    swap(a.sign, b.sign);
}

one_digit big_integer::get_digit(size_t ind) const {
    return data[ind];
}

one_digit big_integer::get_digit_inf(size_t ind) const {
    if (ind >= data.size()) {
        if (sign) {
            return MAX;
        }
        return 0;
    } else {
        return data[ind];
    }
}

size_t big_integer::length() const
{
	return data.size();
}

bool big_integer::isNeg() const
{
	return sign;
}

big_integer &big_integer::operator=(big_integer const &other) {
    big_integer a(other);
    swap(a);
    return *this;
}

big_integer & big_integer::operator+=(big_integer const & rhs)
{
	*this = *this + rhs;
	return *this;
}

big_integer & big_integer::operator-=(big_integer const & rhs)
{
	*this = *this - rhs;
	return *this;
}

big_integer & big_integer::operator*=(big_integer const & rhs)
{
	*this = *this * rhs;
	return *this;
}

big_integer & big_integer::operator*=(one_digit const & rhs)
{
	*this = *this * rhs;
	return *this;
}

big_integer & big_integer::operator*=(int const & rhs)
{
	*this = *this * rhs;
	return *this;
}

big_integer & big_integer::operator/=(big_integer const & rhs)
{
	*this = *this / rhs;
	return *this;
}

big_integer & big_integer::operator/=(one_digit const & rhs)
{
	*this = *this / rhs;
	return *this;
}

big_integer & big_integer::operator/=(int const & rhs)
{
	*this = *this / rhs;
	return *this;
}

big_integer & big_integer::operator%=(big_integer const & rhs)
{
	*this = *this % rhs;
	return *this;
}

big_integer & big_integer::operator&=(big_integer const & rhs)
{
	*this = *this & rhs;
	return *this;
}

big_integer & big_integer::operator%=(one_digit const & rhs)
{
	*this = *this / rhs;
	return *this;
}

big_integer & big_integer::operator%=(int const & rhs)
{
	*this = *this / rhs;
	return *this;
}

big_integer & big_integer::operator|=(big_integer const & rhs)
{
	*this = *this | rhs;
	return *this;
}

big_integer & big_integer::operator^=(big_integer const & rhs)
{
	*this = *this ^ rhs;
	return *this;
}

big_integer & big_integer::operator<<=(int rhs)
{
	*this = *this << rhs;
	return *this;
}

big_integer & big_integer::operator>>=(int rhs)
{
	*this = *this >> rhs;
	return *this;
}

bool operator==(big_integer const & a, big_integer const & b)
{
	return a.sign == b.sign && a.data == b.data;
}

bool operator!=(big_integer const & a, big_integer const & b)
{
	return !(a == b);
}

bool operator<(big_integer const & a, big_integer const & b)
{
	if (a.isNeg() != b.isNeg())
	{
		return a.isNeg() > b.isNeg();
	}
	if (a.length() != b.length())
	{
		return a.length() < b.length();
	}
	for (size_t i = a.data.size(); i > 0; i--)
	{
		if (a.get_digit(i - 1) != b.get_digit(i - 1))
		{
			return a.get_digit(i - 1) < b.get_digit(i - 1);
		}
	}
	return false;
}

bool operator>(big_integer const & a, big_integer const & b)
{
	return b < a;
}

bool operator<=(big_integer const & a, big_integer const & b)
{
	return !(a > b);
}

bool operator>=(big_integer const & a, big_integer const & b)
{
	return !(a < b);
}

big_integer operator+(big_integer const &a, big_integer const &b) {
    two_digit carry = 0;
	size_t len = std::max(a.length(), b.length()) + 1;
    my_vector ans(len + 1);
    for (size_t i = 0; i <= len; ++i) {
        two_digit newcarry = carry + a.get_digit_inf(i) + b.get_digit_inf(i);
        carry = newcarry >> 32;
        ans[i] = (static_cast<one_digit >(newcarry & MAX));
    }
    bool sign = (ans[ans.size() - 1] >> 31) > 0;
    return big_integer(ans, sign);
}

big_integer big_integer::operator~() const {
	size_t len = data.size();
    my_vector res(len);
    for (size_t i = 0; i < len; ++i) {
        res[i] = (~data[i]);
    }
    return big_integer(res, !sign);
}

big_integer & big_integer::operator++()
{
	*this += 1;
	return *this;
}

big_integer big_integer::operator++(int a)
{
	*this += 1;
	return *this;
}

big_integer & big_integer::operator--()
{
	*this -= 1;
	return *this;
}

big_integer big_integer::operator--(int a)
{
	*this -= 1;
	return *this;
}

big_integer big_integer::abs() const {
	if (sign) {
		return -*this;
	}
	return *this;
}

big_integer big_integer::operator-() const {
    return ~*this + 1;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer operator-(big_integer const& a, big_integer const& b) {
    return a + (-b);
}

big_integer operator*(big_integer const & a, big_integer const & b) {
	my_vector res(a.length() + b.length());
	big_integer _a = a.abs(), _b = b.abs();
	for (size_t i = 0; i < a.length(); ++i) {
		two_digit carry = 0;
		for (size_t j = 0; j < b.length(); ++j) {
			size_t ind = i + j;
			two_digit newcarry = static_cast<two_digit>(res[ind]) + carry + static_cast<two_digit>(_a.get_digit(i)) * static_cast<two_digit>(_b.get_digit(j));
			res[ind] = static_cast<one_digit>(newcarry & (MAX));
			carry = newcarry >> 32;
		}
		res[i + b.length()] += static_cast<one_digit>(carry);
	}
	big_integer ans(res, 0);
	if (a.sign ^ b.sign)
		return -ans;
	return ans;
}

big_integer operator*(big_integer const & a, one_digit const & b)
{
	my_vector res(a.length() + 1);
	big_integer _a = a.abs();
	two_digit _b = static_cast<two_digit>(b);
	two_digit carry = 0;
	for (size_t i = 0; i < a.length(); ++i) {
		two_digit newcarry = carry + static_cast<two_digit>(_a.get_digit(i)) * _b;
		res[i] = static_cast<one_digit>(newcarry & (MAX));
		carry = newcarry >> 32;
	}
	res[a.length()] += static_cast<one_digit>(carry);
	big_integer ans(res, 0);
	if (a.isNeg()) {
		return -ans;
	}
	return ans;
}

big_integer operator*(big_integer const & a, int const & b)
{
	bool sgn = a.isNeg() ^ (b < 0);
	big_integer ans = a.abs() * static_cast<one_digit>(std::abs(b));
	if (sgn) {
		return -ans;
	}
	return ans;
}

big_integer operator/(big_integer const & a, big_integer const & b) {
	if (b == 0) {
		throw std::runtime_error("DVB");
	}
	bool new_sign = a.isNeg() ^ b.isNeg();
	big_integer _a = a.abs(), _b = b.abs();
	if (_a < _b) {
		return big_integer(0);
	}
	int n = _a.length(), m = _b.length();
	const size_t len = n - m + 1;
	my_vector ans(len);
	one_digit cof = (MAX + 1ULL) / (_b.data.back() + 1ULL);
	_a *= cof;
	_b *= cof;
	const one_digit div = _b.data.back();
	big_integer fres = _a >> ((n - m) * 32);
	for (size_t i = 0; i < len; ++i) {
		if (fres != 0) {
			fres.data[0] = _a.get_digit(len - 1 - i);
		} else {
			fres += _a.get_digit(len - 1 - i);
		}
		two_digit dd = (static_cast<two_digit>(fres.get_digit_inf(m)) << 32) + fres.get_digit_inf(m - 1);
		dd /= div;
		dd = std::min(dd, static_cast<two_digit>(MAX));
		one_digit d = static_cast<one_digit>(dd);
		while (fres < d * _b) {
			d--;
		}
		fres -= (d * _b);
		fres <<= 32;
		ans[len - 1 - i] = d;
	}
	fres = big_integer(ans, 0);
	if (new_sign) {
		return -fres;
	}
	return fres;
}

big_integer operator/(big_integer const & a, one_digit const & b) {
	if (b == 0) {
		throw std::runtime_error("DVB");
	}
	bool new_sign = a.isNeg();
	big_integer _a = a.abs();
	one_digit _b = b;
	if (_a < _b) {
		return big_integer(0);
	}
	int n = _a.length(), m = 1;
	const size_t len = n - m + 1;
	my_vector ans(len);
	one_digit cof = (MAX + 1ULL) / (_b + 1ULL);
	_a *= cof;
	_b *= cof;
	two_digit fres = static_cast<two_digit>(_a.get_digit_inf(n)) << 32;
	for (size_t i = 0; i < len; ++i) {
		fres += _a.get_digit(len - 1 - i);
		two_digit dd = fres;
		dd /= _b;
		dd = std::min(dd, static_cast<two_digit>(MAX));
		one_digit d = static_cast<one_digit>(dd);
		while (fres < d * _b) {
			d--;
		}
		fres -= (d * _b);
		fres <<= 32;
		ans[len - 1 - i] = d;
	}
	big_integer res = big_integer(ans, 0);
	if (new_sign) {
		return -res;
	}
	return res;
}

big_integer operator/(big_integer const & a, int const & b) {
	bool sgn = a.isNeg() ^ (b < 0);
	/*!!!*/ big_integer ans = a.abs() / static_cast<one_digit>(std::abs(b));
	if (sgn) {
		return -ans;
	}
	return ans;
}

big_integer operator%(big_integer const & a, big_integer const & b)
{
	return a - (a / b) * b;
}

big_integer operator%(big_integer const & a, one_digit const & b)
{
	return a - (a / b) * b;
}

big_integer operator%(big_integer const & a, int const & b)
{
	return a - (a / b) * b;
}

template<typename Type> 
Type doBitOp(Type a, Type b, std::string mode) {
	if (mode == "and") {
		return a & b;
	} else if (mode == "or") {
		return a | b;
	} else if (mode == "xor") {
		return a ^ b;
	}
	return 0;
}

big_integer bitOpImpl(big_integer const& a, big_integer const& b, std::string mode) {
	size_t len = std::max(a.length(), b.length());
	my_vector res(len);
	for (size_t i = 0; i < len; ++i) {
		res[i] = (doBitOp(a.get_digit_inf(i), b.get_digit_inf(i), mode));
	}
	return big_integer(res, doBitOp(a.isNeg(), b.isNeg(), mode));
}

big_integer operator&(big_integer const & a, big_integer const & b)
{
	return bitOpImpl(a, b, "and");
}

big_integer operator|(big_integer const & a, big_integer const & b)
{
	return bitOpImpl(a, b, "or");
}

big_integer operator^(big_integer const & a, big_integer const & b)
{
	return bitOpImpl(a, b, "xor");
}

big_integer operator<<(big_integer a, int b)
{
	if (b == 0) {
		return big_integer(a);
	}
	if (b < 0) {
		return a >> -b;
	}
	two_digit mod = b % 32;
	two_digit kol = b / 32;
	size_t  size = a.length() + kol + (mod > 0);
	my_vector ans(size);
	for (size_t i = kol; i < size; ++i) {
		ans[i] = (a.get_digit_inf(i - kol) << mod) | static_cast<two_digit>(a.get_digit_inf(i - kol - 1)) >> (32 - mod);
	}
	return big_integer(ans, a.sign);
}

big_integer operator>>(big_integer a, int b)
{
	if (b == 0) {
		return big_integer(a);
	}
	if (b < 0) {
		return a << -b;
	}
	two_digit mod = b % 32;
	two_digit kol = b / 32;
	size_t  size = 0;
	if (kol < a.length()) size = a.length() - kol;
	my_vector ans(size);
	for (size_t i = 0; i < size; ++i) {
		ans[i] = (static_cast<two_digit>(a.get_digit_inf(i + kol)) >> mod) | (static_cast<two_digit>(a.get_digit_inf(i + kol + 1)) << (32 - mod));
	}
	return big_integer(ans, a.sign);
}

std::string to_string(big_integer const & a)
{
	std::string ans;
	int mod = static_cast<int>(1e9);
	big_integer _a = a.abs();
	while (_a != 0) {
		big_integer dig = _a % mod;
		one_digit my_dig = dig.get_digit_inf(0);
		_a /= mod;
		for (size_t i = 0; i < 9; ++i) {
			ans += ('0' + my_dig % 10);
			my_dig /= 10;
		}
	}
	if (ans.empty()) {
		return "0";
	}
	while (ans.size() > 1 && ans.back() == '0') {
		ans.pop_back();
	}
	if (a.isNeg()) {
		ans += "-";
	}
	std::reverse(ans.begin(), ans.end());
	return ans;
}

