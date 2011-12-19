#ifndef BH_DIJKSTRA_HPP_
#define BH_DIJKSTRA_HPP_

class CalculationResult;
class AdjacencyArray;
class BHDijkstra {
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
