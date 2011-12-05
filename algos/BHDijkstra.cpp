#include "BHDijkstra.hpp"
#include "../common/CalculationResult.hpp"
#include "../common/BinaryHeap.hpp"
#include "../common/AdjacencyArray.hpp"
#include "../common/Timer.hpp"
#include "../common/assert.hpp"

#include <limits>
#include <iostream>

const CalculationResult BHDijkstra::runStandard(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {
	const unsigned int nodeCount = graph.getNodeCount();
	ASSERT(source < nodeCount, "Invalid node id for source!");
	ASSERT(target < nodeCount, "Invalid node id for target!");
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	Timer runtimeTimer;
	runtimeTimer.start();

	unsigned int pqOps = 0;

	BinaryHeap pq;

	unsigned int distances[nodeCount];
	unsigned int heapItemForNode[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		heapItemForNode[i] = maxValue;
	}

	distances[source] = 0U;
	unsigned int srcHeapItem = pq.insert(source, 0U);
	heapItemForNode[source] = srcHeapItem;
	while (!pq.isEmpty()) {
		const HeapItem &top = pq.min();
		pq.deleteMin();
		++pqOps;

		const unsigned int currentNode = top.getItem();
		heapItemForNode[currentNode] = maxValue;
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

					ASSERT(heapItemForNode[other] == maxValue,
							"Item should not be included in PQ!");

					const unsigned int newItem = pq.insert(other,
							relaxedDistance);
					distances[other] = relaxedDistance;
					heapItemForNode[other] = newItem;
				} else if (distances[other] > relaxedDistance) {

					ASSERT(heapItemForNode[other] != maxValue,
							"Item should be included in PQ!");

					const unsigned int othersHeapItem = heapItemForNode[other];
					distances[other] = relaxedDistance;
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
	const unsigned int nodeCount = graph.getNodeCount();
	ASSERT(source < nodeCount, "Invalid node id for source!");
	ASSERT(target < nodeCount, "Invalid node id for target!");
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	unsigned int pqOps = 0;

	BinaryHeap forwardPQ;
	BinaryHeap backwardPQ;

	unsigned int forwardDistances[nodeCount];
	unsigned int backwardDistances[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		forwardDistances[i] = maxValue;
		backwardDistances[i] = maxValue;
	}
	unsigned int forwardHeapItemForNode[nodeCount];
	unsigned int backwardHeapItemForNode[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		forwardHeapItemForNode[i] = maxValue;
		backwardHeapItemForNode[i] = maxValue;
	}

	while (!forwardPQ.isEmpty() && !backwardPQ.isEmpty()) {
		//TODO
	}

	Timer runtimeTimer;
	runtimeTimer.start();
	runtimeTimer.stop();
	const double distance = forwardDistances[target]
			+ backwardDistances[target];
	const double calculationTime = runtimeTimer.elapsed();
	const CalculationResult result(distance, calculationTime, pqOps,
			"heap, bidirectional");
	return result;

}
const CalculationResult BHDijkstra::runGoalDirected(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {

	const unsigned int nodeCount = graph.getNodeCount();
	ASSERT(source < nodeCount, "Invalid node id for source!");
	ASSERT(target < nodeCount, "Invalid node id for target!");
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	Timer runtimeTimer;
	runtimeTimer.start();

	unsigned int pqOps = 0;

	BinaryHeap pq;

	unsigned int distances[nodeCount];
	double skewedDistances[nodeCount];
	unsigned int heapItemForNode[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		skewedDistances[i] = maxValue;
		heapItemForNode[i] = maxValue;
	}

	distances[source] = 0;
	skewedDistances[source] = 0;
	unsigned int srcHeapItem = pq.insert(source, 0U);
	heapItemForNode[source] = srcHeapItem;
	while (!pq.isEmpty()) {
		const HeapItem &top = pq.min();
		pq.deleteMin();
		++pqOps;

		const unsigned int currentNode = top.getItem();
		heapItemForNode[currentNode] = maxValue;
		const double currentSkewedDist = top.getKey();
		ASSERT(currentSkewedDist == skewedDistances[currentNode],
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
				const double goalDistance = relaxedDistance
						+ graph.distanceBound(other, target)
						- graph.distanceBound(currentNode, target);
				// target node not processed, yet
				if (maxValue == distances[other]) {

					ASSERT(heapItemForNode[other] == maxValue,
							"Item should not be included in PQ!");

					const unsigned int newItem = pq.insert(other, goalDistance);
					distances[other] = relaxedDistance;
					skewedDistances[other] = goalDistance;
					heapItemForNode[other] = newItem;
				} else if (skewedDistances[other] > goalDistance) {

					ASSERT(heapItemForNode[other] != maxValue,
							"Item should be included in PQ!");

					const unsigned int othersHeapItem = heapItemForNode[other];
					distances[other] = relaxedDistance;
					skewedDistances[other] = goalDistance;
					pq.decreaseKey(othersHeapItem, goalDistance);
				}
			} // iteration over neighbors of current node
		}
	}

	runtimeTimer.stop();
	int distance = distances[target];
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps,
			"heap, A*");
	return result;
}
