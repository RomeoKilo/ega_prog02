#ifndef NODE_HPP_
#define NODE_HPP_
class Node {
private:
	long _lng;
	long _lat;
public:
	Node(){}
	Node(const long lng, const long lat) :
			_lng(lng), _lat(lat) {
	}
	static double lowerDistanceBound(const Node &node1, const Node &node2);
	long getLat() const {
		return _lat;
	}
	long getLng() const {
		return _lng;
	}
};
#endif
