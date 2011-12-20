#include "AdjacencyArray.hpp"

#include <iostream>
#include <cstdio>
#include <string>

using std::tr1::shared_ptr;

unsigned int AdjacencyArray::getNodeCount() const {
	return _forward_nodes.size() - 1;
}
unsigned int AdjacencyArray::getEdgeCount() const {
	return _forward_edges.size();
}

unsigned int AdjacencyArray::getMaxEdgeLength() const {
	return _maximumEdgeLength;
}
bool AdjacencyArray::hasBackwardMapping() const {
	return _hasBackwardMapping;
}

const Node &AdjacencyArray::nodeForID(const unsigned int id) const {
	return _node_list[id];
}
OutgoingEdgeIterator AdjacencyArray::outgoingOf(const unsigned int node) const {
	const unsigned int start = _forward_nodes.at(node);
	const unsigned int end = _forward_nodes.at(node + 1);
	return OutgoingEdgeIterator(start, end, *this);
}
IncomingEdgeIterator AdjacencyArray::incomingOf(const unsigned int node) const {
	const unsigned int start = _backward_nodes.at(node);
	const unsigned int end = _backward_nodes.at(node + 1);
	return IncomingEdgeIterator(start, end, *this);
}

shared_ptr<const AdjacencyArray> AdjacencyArray::fromFile(
		const std::string &filename, const bool generateBackwardMapping) {

	FILE *input = fopen(filename.c_str(), "r");

	shared_ptr<AdjacencyArray> result;

	if (input) {
		result = shared_ptr<AdjacencyArray>(new AdjacencyArray());
		AdjacencyArray &adjArray = *result;
		adjArray._hasBackwardMapping = generateBackwardMapping;

		unsigned int nodecount;
		unsigned int edgecount;
		fscanf(input, "g n %d m %d\n", &nodecount, &edgecount);

		adjArray._node_list.resize(nodecount);
		adjArray._edge_list.resize(
				generateBackwardMapping ? 2 * edgecount : edgecount);

		adjArray._forward_nodes.resize(nodecount + 1);
		adjArray._forward_edges.resize(edgecount);
		if (generateBackwardMapping) {
			adjArray._backward_nodes.resize(nodecount + 1);
			adjArray._backward_edges.resize(edgecount);
		}
		for (unsigned int i = 0; i < nodecount; ++i) {
			unsigned int nodeid;
			int lat;
			int lng;

			fscanf(input, "v %d %d %d\n", &nodeid, &lng, &lat);

			adjArray._node_list.at(i) = Node(-lng, lat);
			adjArray._forward_nodes.at(i) = 0;

			if (generateBackwardMapping) {
				adjArray._backward_nodes.at(i) = 0;
			}
		}

		adjArray._maximumEdgeLength = 0;
		for (unsigned int i = 0; i < edgecount; ++i) {
			unsigned int start;
			unsigned int target;
			unsigned int weight; // in decimeters

			fscanf(input, "e %d %d %d\n", &start, &target, &weight);

			adjArray._edge_list.at(i) = Edge(start, target, weight);
			++adjArray._forward_nodes[start];

			if (generateBackwardMapping) {
				++adjArray._backward_nodes[target];
			}

			adjArray._maximumEdgeLength = std::max(adjArray._maximumEdgeLength,
					weight);
		}

		// generate the prefix sums in the node arrays
		for (unsigned int i = 1; i < nodecount; ++i) {
			adjArray._forward_nodes[i] += adjArray._forward_nodes[i - 1];
		}
		// dummy entry pointing beyond the end of the edge array
		adjArray._forward_nodes[nodecount] = edgecount;

		if (generateBackwardMapping) {
			for (unsigned int i = 1; i < nodecount; ++i) {
				adjArray._backward_nodes[i] += adjArray._backward_nodes[i - 1];
			}

			// dummy entry pointing beyond the end of the edge array
			adjArray._backward_nodes[nodecount] = edgecount;
		}

		// Fill the actual adjacency array
		for (unsigned int i = 0; i < edgecount; ++i) {
			const Edge &edge = adjArray._edge_list.at(i);
			adjArray._forward_edges[--adjArray._forward_nodes[edge.getSource()]] =
					i;
		}
		if (generateBackwardMapping) {
			for (unsigned int i = 0; i < edgecount; ++i) {
				const Edge &edge = adjArray._edge_list.at(i);
				adjArray._backward_edges[--adjArray._backward_nodes[edge.getTarget()]] =
						i;
			}
		}
		fclose(input);
	} else {
		throw "File '" + filename + "' could not be opened!";
	}

	return result;
}

void AdjacencyArray::print() const {
	std::cout << "g n " << this->getNodeCount() << " m " << this->getEdgeCount()
			<< std::endl;
	std::cout << "Backward? -> " << this->_hasBackwardMapping << std::endl;
	std::cout << "Outgoing arcs per node:" << std::endl;
	for (unsigned int i = 0; i < this->getNodeCount(); ++i) {
		std::cout << i << ": ";
		OutgoingEdgeIterator iter = this->outgoingOf(i);
		if (!iter.hasNext())
			std::cout << "\t -----" << std::endl;
		while (iter.hasNext()) {
			Edge e = iter.next();
			std::cout << "\t" << e << std::endl;
		}
	}
	std::cout << "Incoming arcs per node:" << std::endl;
	for (unsigned int i = 0; i < this->getNodeCount(); ++i) {
		std::cout << i << ": ";
		IncomingEdgeIterator iter = this->incomingOf(i);
		if (!iter.hasNext())
			std::cout << "\t -----" << std::endl;
		while (iter.hasNext()) {
			Edge e = iter.next();
			std::cout << "\t" << e << std::endl;
		}
	}
//	for (unsigned int i = 0; i < this->getNodeCount(); ++i) {
//		const Node &node = _node_list[i];
//		std::cout << "v" << i << " " << -node.getLng() << " " << node.getLat()
//				<< std::endl;
//	}
}

double AdjacencyArray::distanceBound(const unsigned int first,
		const unsigned int second) const {
	const Node &firstNode = _node_list.at(first);
	const Node &secondNode = _node_list.at(second);

	return Node::lowerDistanceBound(firstNode, secondNode);
}
