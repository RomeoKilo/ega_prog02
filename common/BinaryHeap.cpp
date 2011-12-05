#include "BinaryHeap.hpp"

unsigned int BinaryHeap::insert(const unsigned int item, const double key) {
	_items.push_back(HeapItem(item, key, _binaryHeap.size()));
	_binaryHeap.push_back(_items.size() - 1);
	HeapItem &result = _items.at(_binaryHeap.back());
	this->_siftUp(result);
	return _items.size() - 1;
}

void BinaryHeap::deleteMin() {
	if (this->size() > 1) {
		_binaryHeap.at(0) = _binaryHeap.at(_binaryHeap.size() - 1);
		HeapItem &item = _items.at(_binaryHeap.at(0));
		item._setIndex(0);

		_binaryHeap.pop_back();

		this->_siftDown(item);
	} else {
		_binaryHeap.pop_back();
		_items.pop_back();
	}
}

void BinaryHeap::decreaseKey(const unsigned int heapItem, const double key) {
	HeapItem &item = _items[heapItem];
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
		const bool leftEx = _hasLeft(item);
		const bool rightEx = _hasRight(item);

		// Both candidates are possible
		if (leftEx && _getLeft(item).getKey() < item.getKey() //
		&& rightEx && _getRight(item).getKey() < item.getKey()) {
			if (_getLeft(item).getKey() < _getRight(item).getKey()) {
				_swapItems(item, _getLeft(item));
			} else {
				_swapItems(item, _getRight(item));
			}
		} else if (leftEx && _getLeft(item).getKey() < item.getKey()) {
			_swapItems(item, _getLeft(item));
		} else {
			_swapItems(item, _getRight(item));
		}
	}
}
