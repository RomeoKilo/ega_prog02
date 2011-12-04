#include "BHDijkstra.hpp"
#include "../common/CalculationResult.hpp"
#include "../common/BinaryHeap.hpp"
#include "../common/AdjacencyArray.hpp"
#include "../common/Timer.hpp"
#include "../common/assert.hpp"

#include <limits>

const CalculationResult BHDijkstra::runStandard(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {
	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	Timer runtimeTimer;
	runtimeTimer.start();

	unsigned int pqOps = 0;

	BinaryHeap pq;

	unsigned int distances[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
	}

	HeapItem *heapItemForNode[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		heapItemForNode[i] = 0;
	}

	distances[source] = 0U;
	pq.insert(source, 0U);
	while (!pq.isEmpty()) {
		const HeapItem &top = pq.min();
		pq.deleteMin();
		++pqOps;

		const unsigned int currentNode = top.getItem();
		const double currentDist = top.getKey();
		ASSERT(currentDist == distances[currentNode],
				"Distances are not consistent anymore!");

		// Early Termination
		if (target == currentNode) {
			break;
		} else {
			OutgoingEdgeIterator iterator = graph.outgoingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getTarget();

				const double relaxedDistance = distances[currentNode]
						+ edge.getWeight();
				// target node not processed, yet
				if (maxValue == distances[other]) {

					ASSERT(heapItemForNode[other] == 0,
							"Item should not be included in PQ!");

					HeapItem &newItem = pq.insert(other, relaxedDistance);
					heapItemForNode[other] = &newItem;
				} else if (distances[other] > relaxedDistance) {

					ASSERT(heapItemForNode[other] != 0,
							"Item should be included in PQ!");

					HeapItem &othersHeapItem = *heapItemForNode[other];
					pq.decreaseKey(othersHeapItem, relaxedDistance);
				}
			} // iteration over neighbors of current node
		}
	}

	runtimeTimer.stop();
	double distance = distances[target];
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps, "heap");
	return result;
}
const CalculationResult BHDijkstra::runBidirectional(
		const AdjacencyArray &graph, const unsigned int source,
		const unsigned int target) {

	unsigned int pqOps = 0;

	Timer runtimeTimer;
	runtimeTimer.start();
	runtimeTimer.stop();
	const double distance = -1.0; //TODO
	const double calculationTime = runtimeTimer.elapsed();
	const CalculationResult result(distance, calculationTime, pqOps,
			"heap, bidirectional");
	return result;

}
const CalculationResult BHDijkstra::runGoalDirected(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {

	unsigned int pqOps = 0;
	Timer runtimeTimer;
	runtimeTimer.start();
	runtimeTimer.stop();
	const double distance = -1.0; //TODO
	const double calculationTime = runtimeTimer.elapsed();
	const CalculationResult result(distance, calculationTime, pqOps,
			"heap, A*");
	return result;
}
