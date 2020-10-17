#include <cstdint>
#include <cstddef>
#include <array>
#include <iostream>
#include <chrono>

using namespace std::chrono;

// \brief Measure execution time of a function.
// \tparam N Number of function calls over which result will be averaged.
// \tparam D Duration type in which measure time.
// \tparam Fn Function to measure.
// \tparam Args Function arguments for perfect forwarding.
// \return Average execution time.
template<size_t N = 1, class D = nanoseconds, class Fn, class ...Args>
auto measure_time(Fn &&fn, Args &&...args)
{
	const auto start = high_resolution_clock::now();
	for (auto i = 0; i < N; ++i) (fn)(std::forward<Args>(args)...);
	const auto stop = high_resolution_clock::now();
	return duration_cast<D>(stop - start).count() / N;
}

// \brief Specialization to measure execution time of a member function.
// \tparam N Number of function calls over which result will be averaged.
// \tparam D Duration type in which measure time. 
// \tparam Fn Function to measure.
// \tparam Ptr Pointer to object instance on which member function will be called.
// \tparam Args Function arguments for perfect forwarding.
// \return Average execution time.
template<size_t N = 1, class D = nanoseconds, class Fn, class Ptr, class ...Args>
auto measure_time(Fn &&fn, Ptr *ptr, Args &&...args)
{
	const auto start = high_resolution_clock::now();
	for (auto i = 0; i < N; ++i) (ptr->*fn)(std::forward<Args>(args)...);
	const auto stop = high_resolution_clock::now();
	return duration_cast<D>(stop - start).count() / N;
}

#define RING_BUF_NODISCARD

// \brief Circular buffer implementation. Uses N-1 elements logic.
// \tparam T Data type to be stored.
// \tparam N Max buffer size. Must be >= 2.
template<class T, size_t N = 2, std::enable_if_t<N >= 2>* = nullptr>
class ring_buf {

	T buf[N] = {};
	size_t head = 0;	// First item index / beginning of the buffer.
	size_t tail = 0;	// Last item index.

	class iterator {
		ring_buf<T, N> &buf;
		size_t pos;
	public: 
		iterator(ring_buf<T, N> &buf_, size_t pos_) : buf(buf_), pos(pos_) {}

		T& operator*() { return buf[pos]; }
		T* operator->() { return &(operator*()); }
		iterator& operator++() 
		{ 
			if (++pos == N) pos = 0;
			return *this;
		}
		iterator operator++(int) 
		{   
			T tmp(*this);
			operator++();
			return tmp;
		}
		bool operator==(const iterator &other) { return pos == other.pos; }
        bool operator!=(const iterator &other) { return !operator==(other); }
	};
public:
	iterator begin() { return iterator(*this, head); }
	iterator end() { return iterator(*this, tail); }
	T& operator[](size_t idx) { return buf[idx]; }
	T& front() { return buf[head]; }
	T& back() { return buf[tail]; }
	const iterator begin() const { return iterator(*this, head); }
	const iterator end() const { return iterator(*this, tail); }
	const T& operator[](size_t idx) const { return buf[idx]; }
	const T& front() const { return buf[head]; }
	const T& back() const { return buf[tail]; }
	// Responsibility to check empty() is on the caller.
	void pop_front() { if (++head == N) head = 0; }
	void push_back(const T &item)
	{
		size_t next = tail + 1;
		if (next == N) next = 0;
		if (next == head)
#ifdef RING_BUF_NODISCARD
			return;
#else
			pop_front();
#endif
		buf[tail] = item;
		tail = next;
	}
	void clear() { head = tail = 0; }
	bool empty() const { return tail == head; }
	size_t size() const { return tail > head ? tail - head: N + tail - head; }
	size_t constexpr capacity() const { return N - 1; }
};

// \brief Circular buffer which works only with power of 2 size.
// \tparam T Data type to be stored.
// \tparam N Max buffer size. Must be power of 2.
template<class T, size_t N = 2, std::enable_if_t<(N & (N - 1)) == 0>* = nullptr>
class ring_buf_fast {

	static constexpr auto mask = N - 1;

	T buf[N] = {};
	size_t head = 0;	// First item index / beginning of the buffer.
	size_t tail = 0;	// Last item index.

	class iterator {
		ring_buf_fast<T, N> &buf;
		size_t pos;
	public: 
		iterator(ring_buf_fast<T, N> &buf_, size_t pos_) : buf(buf_), pos(pos_) {}

		T& operator*() { return buf[pos]; }
		T* operator->() { return &(operator*()); }
		iterator& operator++() 
		{ 
			pos = (pos + 1) & mask;
			return *this;
		}
		iterator operator++(int) 
		{   
			T tmp(*this);
			operator++();
			return tmp;
		}
		bool operator==(const iterator &other) { return pos == other.pos; }
        bool operator!=(const iterator &other) { return !operator==(other); }
	};
public:
	iterator begin() { return iterator(*this, head); }
	iterator end() { return iterator(*this, tail); }
	T& operator[](size_t idx) { return buf[idx]; }
	T& front() { return buf[head]; }
	T& back() { return buf[tail]; }
	const iterator begin() const { return iterator(*this, head); }
	const iterator end() const { return iterator(*this, tail); }
	const T& operator[](size_t idx) const { return buf[idx]; }
	const T& front() const { return buf[head]; }
	const T& back() const { return buf[tail]; }
	// Responsibility to check empty() is on the caller.
	void pop_front() { head = (head + 1) & mask; }
	void push_back(const T &item)
	{
		size_t next = (tail + 1) & mask;
		if (next == head)
#ifdef RING_BUF_NODISCARD
			return;
#else
			pop_front();
#endif
		buf[tail] = item;
		tail = next;
	}
	void clear() { head = tail = 0; }
	bool empty() const { return tail == head; }
	size_t size() const { return tail > head ? tail - head: N + tail - head; }
	size_t constexpr capacity() const { return N - 1; }
};
