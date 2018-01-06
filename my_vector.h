#ifndef MY_VECTOR
#define MY_VECTOR

#include <cstdlib>
#include <memory>
#include <cstring>

struct my_vector {
public:
	typedef unsigned int one_digit;

	my_vector();
	my_vector(size_t size);
	my_vector(my_vector const &other);
	~my_vector();

	void reserve(size_t new_cap);
	void resize(size_t new_size);
	size_t size() const;
	bool empty();

	one_digit back();
	void push_back(const one_digit a);
	void pop_back();

	one_digit& operator[](size_t ind);
	one_digit const& operator[](size_t ind) const;
	my_vector& operator=(my_vector const &other);
	friend bool operator==(const my_vector &a, const my_vector &b);
	void swap(my_vector &other) noexcept;

private:
	static const size_t _SIZE = 4;
	size_t _size;
	size_t get_cap() const;
	struct big {
		size_t _capacity;
		std::shared_ptr<one_digit> ptr;
		big(one_digit* a, size_t cap);
		void swap(big &other) noexcept;
	};

	union all_data {
		one_digit small_data[_SIZE];
		big big_data;
		all_data() {};
		~all_data() {};
	} _data;

	struct Del {
		void operator()(one_digit* a) {
			operator delete(a);
		}
	};

	one_digit* cur_data;

	bool isBig() const;
	void make_big(size_t cap);
	void set_capacity(size_t cap);
	void swap_diff_data(all_data &a, all_data &b) noexcept;
};

#endif
