#ifndef ADJACENCY_ARRAY_HPP_
#define ADJACENCY_ARRAY_HPP_

#include "Edge.hpp"
#include "EdgeIterator.hpp"

#include <vector>
#include <tr1/memory>

class AdjacencyArray {
	friend class OutgoingEdgeIterator;
	friend class IncomingEdgeIterator;
private:
	// store the actual nodes
	std::vector<Node> _node_list;
	// stores the edges
	std::vector<Edge> _edge_list;
	// holds indices to the end of one node's edge section
	std::vector<unsigned int> _forward_nodes;
	// holds indices in the 'egde_list' array
	std::vector<unsigned int> _forward_edges;

	bool _hasBackwardMapping;

	std::vector<unsigned int> _backward_nodes;
	// holds indices in the 'egde_list' array
	std::vector<unsigned int> _backward_edges;

	unsigned int _maximumEdgeLength;
public:

	void print() const;

	unsigned int getNodeCount() const;
	unsigned int getEdgeCount() const;
	unsigned int getMaxEdgeLength() const;
	bool hasBackwardMapping() const;
	OutgoingEdgeIterator outgoingOf(const unsigned int node) const;
	IncomingEdgeIterator incomingOf(const unsigned int node) const;

	static std::tr1::shared_ptr<AdjacencyArray> fromFile(
			const std::string &filename, const bool generateBackwardMapping);

};
#endif
