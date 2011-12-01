#include "EdgeIterator.hpp"

#include "AdjacencyArray.hpp"

const Edge &OutgoingEdgeIterator::next() {
	const unsigned int edgeListIndex = _array._forward_edges.at(_current++);
	return _array._edge_list.at(edgeListIndex);
}

const Edge &IncomingEdgeIterator::next() {
	const unsigned int edgeListIndex = _array._backward_edges.at(_current++);
	return _array._edge_list.at(edgeListIndex);
}
