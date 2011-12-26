#ifndef BH_DIJKSTRA_HPP_
#define BH_DIJKSTRA_HPP_

class CalculationResult;
class AdjacencyArray;
class BHDijkstra {
public:

	/**
	 * Runs an unoptimized version of Dijkstra using a Binary Heap
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
	 * Runs a bidirectional version of Dijkstra using a Binary Heap
	 * as PQ data structure.
	 *
	 * During the search, we keep a distance which denotes the shortest
	 * path which can currently be taken using only settled nodes.
	 * If the sum of the minkeys of both searches, forward search and
	 * backward search, exceeds this distance value we know that we
	 * cannot do better and the search is aborted.
	 *
	 * @param graph the graph upon which Dijkstra operates
	 * @param source the source node
	 * @param target the target node
	 *
	 * @return the result of the calculation not containing the actual route
	 */
	static const CalculationResult runBidirectional(const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);

	/**
	 * Runs a goal-directed version of Dijkstra using a Binary Heap
	 * as PQ data structure.
	 *
	 * The relaxed weight of a node t is modified by adding the
	 * minimal distance from t to the target, therefore favoring
	 * edges which "tend" to point in the right direction.
	 *
	 * Note, however, that this approach may re-insert nodes into
	 * the queue thus producing exponential worst case runtime.
	 *
	 * @param graph the graph upon which Dijkstra operates
	 * @param source the source node
	 * @param target the target node
	 *
	 * @return the result of the calculation not containing the actual route
	 */
	static const CalculationResult runGoalDirected(const AdjacencyArray &graph,
			const unsigned int source, const unsigned int target);
};
#endif
