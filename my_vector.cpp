#include <cassert>
#include "my_vector.h"

typedef unsigned int one_digit;

size_t new_cap(size_t n) {
	if (n == 0) {
		return 4;
	} 
	return (n << 1);
}

one_digit* copy_data(const one_digit* data, size_t cnt, size_t cap) {
	one_digit* ans = static_cast<one_digit*>(operator new(cap * sizeof(one_digit)));
	memcpy(ans, data, cnt * sizeof(one_digit));
	memset(ans + cnt, 0, (cap - cnt) * sizeof(one_digit));
	return ans;
}

bool my_vector::isBig() const {
	return (cur_data != _data.small_data);
}

void my_vector::make_big(size_t cap) {
	new(&_data.big_data) big(copy_data(cur_data, _size, cap), cap);
	cur_data = _data.big_data.ptr.get();
}

void my_vector::set_capacity(size_t cap) {
	if (isBig() || (cap > _SIZE)) {
		if (!isBig()) {
			make_big(cap);
		} else {
			_data.big_data.ptr.reset(copy_data(cur_data, size(), cap), Del());
			_data.big_data._capacity = cap;
			cur_data = _data.big_data.ptr.get();
		}
	}
}

void my_vector::swap_diff_data(typename my_vector::all_data & a, typename my_vector::all_data & b) noexcept {
	one_digit copy[_SIZE];
	memcpy(copy, b.small_data, _SIZE * sizeof(one_digit));
	new(&b.big_data) big(a.big_data);
	a.big_data.~big();
	memcpy(a.small_data, copy, _SIZE * sizeof(one_digit));
}

void my_vector::reserve(size_t new_cap) {
	if (new_cap > get_cap()) {
		set_capacity(new_cap);
	}
}

void my_vector::resize(size_t new_size) {
	reserve(new_size);
	_size = new_size;
}

size_t my_vector::size() const {
	return _size;
}

bool my_vector::empty() {
	return (_size == 0);
}

one_digit my_vector::back()
{
	return cur_data[_size - 1];
}

void my_vector::push_back(const one_digit a) {
	if (get_cap() <= _size) {
		reserve(new_cap(_size));
	}
	cur_data[_size] = a;
	_size++;
}

void my_vector::pop_back() {
	_size--;
}

my_vector::my_vector() : _size(0), cur_data(_data.small_data) {
	memset(cur_data, 0, _SIZE * sizeof(one_digit));
}

my_vector::my_vector(size_t size) : my_vector() {
	reserve(size);
	_size = size;
}

my_vector::my_vector(my_vector const & other): _size(other._size) {
	if (other.isBig()) {
		new(&_data.big_data) big(other._data.big_data);
		cur_data = _data.big_data.ptr.get();
	} else {
		memcpy(_data.small_data, other._data.small_data, _SIZE * sizeof(one_digit));
		cur_data = _data.small_data;
	}
}

my_vector::~my_vector() {
	if (isBig()) {
		_data.big_data.~big();
	}
}

size_t my_vector::get_cap() const {
	if (isBig()) {
		return _data.big_data._capacity;
	} else {
		return _SIZE;
	}
}

my_vector::big::big(one_digit * a, size_t cap) : _capacity(cap), ptr(a) {
}

void my_vector::big::swap(big & other) noexcept {
	std::swap(ptr, other.ptr);
	std::swap(_capacity, other._capacity);
}

one_digit & my_vector::operator[](size_t ind)
{
	return cur_data[ind];
}

one_digit const & my_vector::operator[](size_t ind) const
{
	return cur_data[ind];
}

my_vector & my_vector::operator=(my_vector const & other)
{
	my_vector copy(other);
	swap(copy);
	return *this;
}

void my_vector::swap(my_vector & other) noexcept {
	using std::swap;
	if (!isBig() && !other.isBig()) {
		for (size_t i = 0; i < _SIZE; ++i) {
			swap(_data.small_data[i], other._data.small_data[i]);
		}
	} else if (isBig() && other.isBig()) {
		swap(_data.big_data, other._data.big_data);
		cur_data = _data.big_data.ptr.get();
		other.cur_data = other._data.big_data.ptr.get();
	} else if (isBig()) {
		swap_diff_data(_data, other._data);
		cur_data = _data.small_data;
		other.cur_data = other._data.big_data.ptr.get();
	} else {
		swap_diff_data(other._data, _data);
		other.cur_data = other._data.small_data;
		cur_data = _data.big_data.ptr.get();
	}
	swap(_size, other._size);
}

bool operator==(const my_vector &a,const my_vector &b)
{
	if (a.size() != b.size()) {
		return 0;
	}
	return (memcmp(a.cur_data, b.cur_data, a._size * sizeof(one_digit)) == 0);
}

