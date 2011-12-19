#ifndef BINARY_HEAP_HPP_
#define BINARY_HEAP_HPP_

#include "HeapItem.hpp"
#include "assert.hpp"

#include <vector>
#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>

class BinaryHeap {
private:
	// the actual heap items
	std::vector<HeapItem> _items;
	// each item i is an index in _items
	std::vector<unsigned int> _binaryHeap;

	void _siftUp(HeapItem &item);
	void _siftDown(HeapItem &item);

	bool _hasParent(const HeapItem &item) {
		return item._getIndex() > 0;
	}
	HeapItem &_getParent(const HeapItem &item) {
		return _items.at(_binaryHeap.at((item._getIndex() - 1) / 2));
	}

	bool _hasLeft(const HeapItem &item) {
		ASSERT(item._getIndex() >= 0, "");
		return 2 * (unsigned int) item._getIndex() + 1 < _binaryHeap.size();
	}
	HeapItem &_getLeft(const HeapItem &item) {
		return _items.at(_binaryHeap.at(2 * item._getIndex() + 1));
	}

	bool _hasRight(const HeapItem &item) {
		ASSERT(item._getIndex() >= 0, "");
		return 2 * (unsigned int) item._getIndex() + 2 < _binaryHeap.size();
	}
	HeapItem &_getRight(const HeapItem &item) {
		return _items.at(_binaryHeap.at(2 * item._getIndex() + 2));
	}
	void _swapItems(HeapItem &item1, HeapItem &item2);

public:
	explicit BinaryHeap() {
	}

	unsigned int insert(const unsigned int item, const double key);
	void decreaseKey(const unsigned int heapItem, const double key);
	const HeapItem &min() const {
		return _items.at(_binaryHeap.at(0));
	}

	void deleteMin();

	unsigned int size() const {
		return _binaryHeap.size();
	}

	bool isEmpty() const {
		return 0 == this->size();
	}

	void print() const;
};

#endif /*BINARY_HEAP_HPP_*/
