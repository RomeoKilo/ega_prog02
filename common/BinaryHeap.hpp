#ifndef BINARY_HEAP_HPP_
#define BINARY_HEAP_HPP_

#include "HeapItem.hpp"

#include <vector>
#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>

template<typename T>
class BinaryHeap {
private:
	// the actual heap items
	std::vector<HeapItem<T> > _items;
	// each item i is an index in _items
	std::vector<unsigned int> _binaryHeap;

	void _siftUp(HeapItem<T> &item);
	void _siftDown(HeapItem<T> &item);

	bool _hasParent(const HeapItem<T> &item) {
		return item._getIndex() > 0;
	}
	HeapItem<T> &_getParent(const HeapItem<T> &item) {
		return _items.at(_binaryHeap.at((item._getIndex() - 1) / 2));
	}

	bool _hasLeft(const HeapItem<T> &item) {
		return 2 * item._getIndex() + 1 < _binaryHeap.size();
	}
	HeapItem<T> &_getLeft(const HeapItem<T> &item) {
		return _items.at(_binaryHeap.at(2 * item._getIndex() + 1));
	}

	bool _hasRight(const HeapItem<T> &item) {
		return 2 * item._getIndex() + 2 < _binaryHeap.size();
	}
	HeapItem<T> *_getRight(const HeapItem<T> &item) {
		return _items.at(_binaryHeap.at(2 * item._getIndex() + 2));
	}
	void _swapItems(HeapItem<T> &item1, HeapItem<T> &item2) {

		std::swap(_binaryHeap[item1._getIndex()],
				_binaryHeap[item2._getIndex()]);

		int tmpIndex = item1._getIndex();
		item1._setIndex(item2._getIndex());
		item2._setIndex(tmpIndex);
	}

public:
	explicit BinaryHeap() {
	}
	;

	HeapItem<T> &insert(T &item, int key);
	void decreaseKey(HeapItem<T> &item, const int key);
	HeapItem<T> &min() const {
		return _items.at(_binaryHeap.at(0));
	}

	void deleteMin();

	bool isEmpty() const {
		return 0 == _items.size();
	}

	void print() {
		std::cout << "BH [";
		for (std::vector<unsigned int>::const_iterator iter = _binaryHeap.begin(); iter != _binaryHeap.end(); ++iter)
		{
			unsigned int index = *iter;
			std::cout << _items.at(index).getKey() << ", ";
		}
		std::cout << "]" << std::endl;
	}
};

template<typename T>
inline HeapItem<T> &BinaryHeap<T>::insert(T &item, const int key) {
	_items.push_back(HeapItem<T>(item, key, _binaryHeap.size()));
	_binaryHeap.push_back(_binaryHeap.size());
	HeapItem<T> &result = _items.at(_binaryHeap.size() - 1);
	this->_siftUp(result);
	return result;
}

template<typename T>
inline void BinaryHeap<T>::deleteMin() {
	_binaryHeap.at(0) = _binaryHeap.size() - 1;
	HeapItem<T> &item = _items.at(_binaryHeap.at(0));
	item._setIndex(0);
	this->_siftDown(item);
}

template<typename T>
void BinaryHeap<T>::decreaseKey(HeapItem<T> &item, const int key) {
	item._setKey(key);
	_siftUp(item);
}

template<typename T>
inline void BinaryHeap<T>::_siftUp(HeapItem<T> &item) {
	while (_hasParent(item) && _getParent(item).getKey() > item.getKey()) {
		_swapItems(item, _getParent(item));
	}
}

template<class T>
inline void BinaryHeap<T>::_siftDown(HeapItem<T> &item) {
	while ((_hasLeft(item) && _getLeft(item).getKey() < item.getKey())
			|| (_hasRight(item) && _getRight(item).getKey() < item.getKey())) {

		if (_hasLeft(item) && _getLeft(item).getKey() < item.getKey()) {
			_swapItems(item, _getLeft(item));
		} else {
			_swapItems(item, _getRight(item));
		}
	}
}
#endif /*BINARY_HEAP_HPP_*/
