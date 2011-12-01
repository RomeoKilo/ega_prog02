#ifndef EDGE_ITERATOR_HPP_
#define EDGE_ITERATOR_HPP_

#include "Edge.hpp"

class AdjacencyArray;

/**
 * An iterator over the outgoing edges of a node v.
 *
 * v will be the source node of each edge returned.
 */
class OutgoingEdgeIterator {
	friend class AdjacencyArray;
private:
	unsigned int _current;
	const unsigned int _start;
	const unsigned int _end;
	const AdjacencyArray &_array;
	OutgoingEdgeIterator(const unsigned int start, const unsigned int end,
			const AdjacencyArray &array) :
			_current(start), _start(start), _end(end), _array(array) {
	}
public:
	bool hasNext() const {
		return _start <= _current && _current < _end;
	}
	const Edge &next();
};

/**
 * An iterator over the incoming edges of a node v.
 *
 * v will be the target node of each edge returned.
 */
class IncomingEdgeIterator {
	friend class AdjacencyArray;
private:
	unsigned int _current;
	const unsigned int _start;
	const unsigned int _end;
	const AdjacencyArray &_array;
	IncomingEdgeIterator(const unsigned int start, const unsigned int end,
			const AdjacencyArray &array) :
			_current(start), _start(start), _end(end), _array(array) {
	}
public:
	bool hasNext() const {
		return _start <= _current && _current < _end;
	}
	const Edge &next();
};

#endif
