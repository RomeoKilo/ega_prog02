#ifndef HEAP_ITEM_HPP_
#define HEAP_ITEM_HPP_

#include "BinaryHeap.hpp"

#include <iostream>

class HeapItem {
	friend class BinaryHeap;
private:
	int _index;

	int _item;
	int _key;

	void _setIndex(const unsigned int i) {
		_index = i;
	}

	void _unsetIndex()
	{
		_index = -1;
	}

	int _getIndex() const {
		return _index;
	}

	void _setKey(const int key) {
		_key = key;
	}

	HeapItem(){}

	HeapItem(unsigned int item, const double key, const unsigned int index) :
			_index(index), _item(item), _key(key) {
	}

public:
	int getKey() const {
		return this->_key;
	}

	unsigned int getItem() const
	{
		return _item;
	}

};

inline std::ostream& operator<<(std::ostream& lhs, const HeapItem &rhs) {
	lhs << rhs.getKey();
	return lhs;
}
#endif
