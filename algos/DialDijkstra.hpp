#ifndef DIAL_DIJKSTRA_HPP_
#define DIAL_DIJKSTRA_HPP_

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
};
#endif
