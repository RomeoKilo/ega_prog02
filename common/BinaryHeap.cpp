#include "BinaryHeap.hpp"

HeapItem &BinaryHeap::insert(const unsigned int item, const double key) {
	_items.push_back(HeapItem(item, key, _binaryHeap.size()));
	_binaryHeap.push_back(_binaryHeap.size());
	HeapItem &result = _items.at(_binaryHeap.size() - 1);
	this->_siftUp(result);
	return result;
}

void BinaryHeap::deleteMin() {
	_binaryHeap.at(0) = _binaryHeap.size() - 1;
	HeapItem &item = _items.at(_binaryHeap.at(0));
	item._setIndex(0);
	this->_siftDown(item);
}

void BinaryHeap::decreaseKey(HeapItem &item, const double key) {
	item._setKey(key);
	_siftUp(item);
}

void BinaryHeap::_siftUp(HeapItem &item) {
	while (_hasParent(item) && _getParent(item).getKey() > item.getKey()) {
		_swapItems(item, _getParent(item));
	}
}

void BinaryHeap::_siftDown(HeapItem &item) {
	while ((_hasLeft(item) && _getLeft(item).getKey() < item.getKey())
			|| (_hasRight(item) && _getRight(item).getKey() < item.getKey())) {

		if (_hasLeft(item) && _getLeft(item).getKey() < item.getKey()) {
			_swapItems(item, _getLeft(item));
		} else {
			_swapItems(item, _getRight(item));
		}
	}
}
