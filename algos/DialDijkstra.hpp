#ifndef DIAL_DIJKSTRA_HPP_
#define DIAL_DIJKSTRA_HPP_

#include <list>

class AdjacencyArray;
class CalculationResult;

class DialDijkstra {
public:
	static const CalculationResult runStandard(
			const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
	static const CalculationResult runBidirectional(
			const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
	static const CalculationResult runGoalDirected(
			const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
	static void print(std::list<unsigned int> *bucketQueue, unsigned int len, unsigned int current);
};
#endif
