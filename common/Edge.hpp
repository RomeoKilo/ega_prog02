#ifndef EDGE_HPP_
#define EDGE_HPP_

#include "Node.hpp"
#include <iostream>

class Edge {
private:
	unsigned int _source;
	unsigned int _target;
	unsigned int _weight;
public:
	Edge() {
	}
	Edge(const unsigned int &source, const unsigned int &target,
			const unsigned int weight) :
			_source(source), _target(target), _weight(weight) {
	}

	const unsigned int &getSource() const {
		return _source;
	}
	const unsigned int &getTarget() const {
		return _target;
	}
	unsigned int getWeight() const {
		return _weight;
	}

};

inline std::ostream &operator<<(std::ostream &stream, const Edge &edge) {
	stream << "e " << edge.getSource() << " " << edge.getTarget() << " "
			<< edge.getWeight();
	return stream;
}


#endif
