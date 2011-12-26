#ifndef DIAL_DIJKSTRA_HPP_
#define DIAL_DIJKSTRA_HPP_

class AdjacencyArray;
class CalculationResult;

class DialDijkstra {
public:
	/**
	 * Runs an unoptimized version of Dijkstra using a Bucket Queue
	 * as PQ data structure.
	 *
	 * @param graph the graph upon which Dijkstra operates
	 * @param source the source node
	 * @param target the target node
	 *
	 * @return the result of the calculation not containing the actual route
	 */
	static const CalculationResult runStandard(const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
	/**
	 * Runs a bidirectional version of Dijkstra using a Bucket Queue
	 * as PQ data structure.
	 *
	 * @param graph the graph upon which Dijkstra operates
	 * @param source the source node
	 * @param target the target node
	 *
	 * @return the result of the calculation not containing the actual route
	 *
	 * @see BHDijkstra.hpp for more information about the applied strategy
	 */
	static const CalculationResult runBidirectional(const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
	/**
	 * Runs a goal-directed version of Dijkstra using a Bucket Queue
	 * as PQ data structure.
	 *
	 * @param graph the graph upon which Dijkstra operates
	 * @param source the source node
	 * @param target the target node
	 *
	 * @return the result of the calculation not containing the actual route
	 *
	 * @see BHDijkstra.hpp for more information about the applied strategy
	 */
	static const CalculationResult runGoalDirected(const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
};
#endif
