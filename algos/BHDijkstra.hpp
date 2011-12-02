#ifndef BH_DIJKSTRA_HPP_
#define BH_DIJKSTRA_HPP_

#include <tr1/memory>
class CalculationResult;
class AdjacencyArray;
class BHDijkstra {
public:
	static const CalculationResult runStandard(
			const std::tr1::shared_ptr<AdjacencyArray> graph,
			const unsigned int source, const unsigned int target);
	static const CalculationResult runBidirectional(
			const std::tr1::shared_ptr<AdjacencyArray> graph,
			const unsigned int source, const unsigned int target);
	static const CalculationResult runGoalDirected(
			const std::tr1::shared_ptr<AdjacencyArray> graph,
			const unsigned int source, const unsigned int target);
};
#endif
