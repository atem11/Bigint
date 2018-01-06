#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cstddef>
#include <iosfwd>
#include <vector>
#include <climits>
#include <string>
#include <stdexcept>
//#include "my_vector.h"

struct big_integer
{
    typedef unsigned int one_digit;
	typedef std::vector<one_digit> my_vector;
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
	explicit big_integer(my_vector const& num, bool f);
	big_integer(one_digit const& big);
    explicit big_integer(std::string const& str);
    //~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
	big_integer& operator*=(one_digit const& rhs);
	big_integer& operator*=(int const& rhs);
    big_integer& operator/=(big_integer const& rhs);
	big_integer& operator/=(one_digit const& rhs);
	big_integer& operator/=(int const& rhs);
    big_integer& operator%=(big_integer const& rhs);
	big_integer& operator%=(one_digit const& rhs);
	big_integer& operator%=(int const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

	big_integer abs() const;

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

	friend big_integer operator+(big_integer const& a, big_integer const& b);
	friend big_integer operator-(big_integer const& a, big_integer const& b);
	friend big_integer operator*(big_integer const& a, big_integer const& b);
	friend big_integer operator*(big_integer const& a, one_digit const& b);
	friend big_integer operator*(big_integer const& a, int const& b);
	friend big_integer operator/(big_integer const& a, big_integer const& b);
	friend big_integer operator/(big_integer const& a, one_digit const& b);
	friend big_integer operator/(big_integer const& a, int const& b);
	friend big_integer operator%(big_integer const& a, big_integer const& b);
	friend big_integer operator%(big_integer const& a, one_digit const& b);
	friend big_integer operator%(big_integer const& a, int const& b);

	friend big_integer operator&(big_integer const& a, big_integer const& b);
	friend big_integer operator|(big_integer const& a, big_integer const& b);
	friend big_integer operator^(big_integer const& a, big_integer const& b);

	friend big_integer operator<<(big_integer a, int b);
	friend big_integer operator>>(big_integer a, int b);

    friend std::string to_string(big_integer const& a);
    void swap(big_integer other);
    friend void swap(big_integer a, big_integer b);
    one_digit get_digit(size_t ind) const;
    one_digit get_digit_inf(size_t ind) const;
	size_t length() const;
	bool isNeg() const;

private:
    my_vector data;
    bool sign;
    void contraction();
};

#endif // BIG_INTEGER_H
