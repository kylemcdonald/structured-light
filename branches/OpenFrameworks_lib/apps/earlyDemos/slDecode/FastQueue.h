#pragma once

#include <string>
#include <ostream>
using namespace std;

template <class T>
class FastQueue {
private:
	T* _data;
	int _start, _end, _size, _capacity;
public:
	FastQueue() {
		_data = NULL;
		_capacity = 0;
		clear();
	}
	~FastQueue() {
		if(_data != NULL)
			delete [] _data;
	}
	void clear() {
		_start = _end = _size = 0;
	}
	void reserve(int n) {
		_capacity = n;
		if(_data != NULL)
			delete [] _data;
		_data = new T[_capacity];
		clear();
	}
	int size() const {
		return _size;
	}
	int capacity() const {
		return _capacity;
	}
	bool empty() const {
		return _size == 0;
	}
	void push_back(T& e) {
		_data[_end] = e;
		_end++;
		if(_end == _capacity)
			_end = 0;
		_size++;
	}
	T front() const {
		return _data[_start];
	}
	T back() const {
		if(_end == 0)
			return _data[_capacity - 1];
		else
			return _data[_end - 1];
	}
	void pop_front() {
		_start++;
		if(_start == _capacity)
			_start = 0;
		_size--;
	}
	T& operator[](int i) const {
		int offset = _start + i;
		if(offset < _capacity)
			return _data[offset];
		else
			return _data[offset - _capacity];
	}
};

template <class T>
ostream& operator<<(ostream& os, const FastQueue<T>& fastQueue) {
	os << "[" << fastQueue.size() << "] {";
	for(int i = 0; i < fastQueue.size() - 1; i++)
		os << fastQueue[i] << ", ";
	os << fastQueue.back() << "}";
	return os;
}
