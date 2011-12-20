#ifndef NODE_HPP_
#define NODE_HPP_
class Node {
private:
	int _lng;
	int _lat;
public:
	Node() {
	}
	Node(const int lng, const int lat) :
			_lng(lng), _lat(lat) {
	}

	static double lowerDistanceBound(const Node &node1, const Node &node2);

	/**
	 * Returns the true latitude coordinate of this node
	 */
	int getLat() const {
		return _lat;
	}

	/**
	 * Returns the true longitude coordinate of this node
	 */
	int getLng() const {
		return _lng;
	}
};
#endif
