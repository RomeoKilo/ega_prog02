#ifndef HEAP_ITEM_HPP_
#define HEAP_ITEM_HPP_

#include "BinaryHeap.hpp"

#include <iostream>

class HeapItem {
	friend class BinaryHeap;
private:
	int _index;

	unsigned int _item;
	double _key;

	void _setIndex(const unsigned int i) {
		_index = i;
	}

	unsigned int _getIndex() const {
		return _index;
	}

	void _setKey(const double key) {
		_key = key;
	}

	HeapItem(){}

	HeapItem(unsigned int item, const double key, const unsigned int index) :
			_index(index), _item(item), _key(key) {
	}

public:
	double getKey() const {
		return this->_key;
	}

	unsigned int getItem() const
	{
		return _item;
	}

};

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const HeapItem &rhs) {
	lhs << rhs.getKey();
	return lhs;
}
#endif
