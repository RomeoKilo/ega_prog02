#ifndef NODE_HPP_
#define NODE_HPP_
class Node {
private:
	double _lng;
	double _lat;
public:
	Node(){}
	Node(const double lng, const double lat) :
			_lng(lng), _lat(lat) {
	}
	static double distance(const Node &node1, const Node &node2);
	double getLat() const {
		return _lat;
	}
	double getLng() const {
		return _lng;
	}
};
#endif
