#ifndef HEAP_ITEM_HPP_
#define HEAP_ITEM_HPP_

#include "BinaryHeap.hpp"

#include <iostream>

template<typename T>
class BinaryHeap;
template<typename T>
class HeapItem {
	friend class BinaryHeap<T> ;
private:
	int _index;

	T *_item;
	int _key;

	void _setIndex(unsigned int i) {
		_index = i;
	}

	int _getIndex() const {
		return _index;
	}

	void _setKey(int key) {
		_key = key;
	}

	HeapItem(T &item, const int key, const unsigned int index) :
			_index(index), _item(&item), _key(key) {
	}

public:
	int getKey() const {
		return this->_key;
	}

	HeapItem &getItem()
	{
		return &_item;
	}

};

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const HeapItem<T> & rhs) {
	lhs << rhs.getKey();
	return lhs;
}
#endif
