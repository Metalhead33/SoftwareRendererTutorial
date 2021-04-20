#ifndef FIXEDSTACK_HPP
#define FIXEDSTACK_HPP
#include <array>
#include <cstring>

template <typename T, size_t siz> struct FixedStack {
public:
	typedef std::array<T,siz> array_type;
	typedef T* iterator;
	typedef const T* const_iterator;
private:
	size_t stackPointer;
	array_type storage;
public:
	FixedStack() : stackPointer(0) {

	}
	FixedStack(const FixedStack& cpy) : stackPointer(cpy.stackPointer) {
		std::memcpy(storage.data(),cpy.storage.data(),sizeof(T) * cpy.stackPointer);
	}
	FixedStack& operator=(const FixedStack& cpy) {
		this->stackPointer = cpy.stackPointer;
		std::memcpy(storage.data(),cpy.storage.data(),sizeof(T) * cpy.stackPointer);
		return *this;
	}
	void push_back(const T& val) {
		storage[stackPointer] = val;
		++stackPointer;
	}
	void push_back(T&& val) {
		storage[stackPointer] = std::move(val);
		++stackPointer;
	}
	iterator begin() {
		return storage.data();
	}
	const_iterator begin() const {
		return storage.data();
	}
	iterator end() {
		return &storage[stackPointer];
	}
	const_iterator end() const {
		return &storage[stackPointer];
	}
	void clear() {
		stackPointer = 0;
	}
	size_t size() const {
		return stackPointer;
	}
	T& get(size_t i) { return storage[i]; }
	T& operator[](size_t i) { return storage[i]; }
	const T& get(size_t i) const { return storage[i]; }
	const T& operator[](size_t i) const { return storage[i]; }
};

#endif // FIXEDSTACK_HPP
