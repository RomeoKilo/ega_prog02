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

	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	unsigned int pqOps = 0;

	// Data structures

	BinaryHeap &pq = *(new BinaryHeap);
	unsigned int *distances = new unsigned int[nodeCount];
	int *heapItemForNode = new int[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		heapItemForNode[i] = -1;
	}

	// Initialize source
	distances[source] = 0U;
	unsigned int srcHeapItem = pq.insert(source, 0U);
	heapItemForNode[source] = srcHeapItem;

	while (!pq.isEmpty()) {
		const HeapItem top = pq.min();
		pq.deleteMin();

		++pqOps;

		const unsigned int currentNode = top.getItem();
		heapItemForNode[currentNode] = -1;
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

					ASSERT(heapItemForNode[other] == -1,
							"Item should not be included in PQ!");

					const unsigned int newItem = pq.insert(other,
							relaxedDistance);
					distances[other] = relaxedDistance;
					heapItemForNode[other] = newItem;
				} else if (distances[other] > relaxedDistance) {

					ASSERT(heapItemForNode[other] != -1,
							"Item should be included in PQ!");

					const unsigned int othersHeapItem = heapItemForNode[other];
					distances[other] = relaxedDistance;
					pq.decreaseKey(othersHeapItem, relaxedDistance);
				}
			} // iteration over neighbors of current node
		}
	}

	const double distance = distances[target];

	delete &pq;
	delete[] distances;
	delete[] heapItemForNode;

	runtimeTimer.stop();
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps, "heap");
	return result;
}
const CalculationResult BHDijkstra::runBidirectional(
		const AdjacencyArray &graph, const unsigned int source,
		const unsigned int target) {
	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	unsigned int pqOps = 0;

	// Data for coordinating both queues
	unsigned int minimalTotalDistance = maxValue;
	int meetingPoint = -1;

	/*
	 * DATA STRUCTURES
	 */
	BinaryHeap &fwPQ = *(new BinaryHeap());
	BinaryHeap &bwPQ = *(new BinaryHeap());

	unsigned int *fwDistances = new unsigned int[nodeCount];
	bool *fwPoppedFromQueue = new bool[nodeCount];
	int *fwHeapItemForNode = new int[nodeCount];

	unsigned int *bwDistances = new unsigned int[nodeCount];
	bool *bwPoppedFromQueue = new bool[nodeCount];
	int *bwHeapItemForNode = new int[nodeCount];

	for (unsigned int i = 0; i < nodeCount; ++i) {
		fwDistances[i] = maxValue;
		fwPoppedFromQueue[i] = false;
		fwHeapItemForNode[i] = -1;

		bwDistances[i] = maxValue;
		bwPoppedFromQueue[i] = false;
		bwHeapItemForNode[i] = -1;
	}

	/*
	 * Initialize source and target data
	 */
	fwHeapItemForNode[source] = fwPQ.insert(source, 0);
	fwDistances[source] = 0;

	bwHeapItemForNode[target] = bwPQ.insert(target, 0);
	bwDistances[target] = 0;

	while (!fwPQ.isEmpty() || !bwPQ.isEmpty()) {
		// Select the queue with the smaller min-key to do the next step
		if (bwPQ.isEmpty()
				|| (!fwPQ.isEmpty()
						&& fwPQ.min().getKey() <= bwPQ.min().getKey())) {

			const HeapItem &top = fwPQ.min();
			fwPQ.deleteMin();

			const unsigned int currentNode = top.getItem();
			fwPoppedFromQueue[currentNode] = true;
			fwHeapItemForNode[currentNode] = -1;

			++pqOps;

			ASSERT(top.getKey() == fwDistances[currentNode],
					"Node distances inconsistent!")

			// current node has been settled by both queues
			if (bwPoppedFromQueue[currentNode]) {
				break;
			}

			OutgoingEdgeIterator iterator = graph.outgoingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getTarget();

				const double relaxedDistance = fwDistances[currentNode]
						+ edge.getWeight();
				const unsigned int pathCost = fwDistances[currentNode]
						+ edge.getWeight() + bwDistances[other];

				if (bwDistances[other] != maxValue
						&& pathCost < minimalTotalDistance) {
					minimalTotalDistance = pathCost;
					meetingPoint = other;
				}

				if (maxValue == fwDistances[other]) {

					ASSERT(fwHeapItemForNode[other] == -1,
							"Item should not be included in forward PQ!");

					const unsigned int newItem = fwPQ.insert(other,
							relaxedDistance);
					fwDistances[other] = relaxedDistance;
					fwHeapItemForNode[other] = newItem;
				} else if (fwDistances[other] > relaxedDistance) {

					ASSERT(fwHeapItemForNode[other] != -1,
							"Item should be included in forward PQ!");

					const unsigned int othersHeapItem = fwHeapItemForNode[other];
					fwDistances[other] = relaxedDistance;
					fwPQ.decreaseKey(othersHeapItem, relaxedDistance);
				}
			} // iteration over outgoing edges of current node
		} else { // Do one step with the backward search

			const HeapItem &top = bwPQ.min();
			bwPQ.deleteMin();

			const unsigned int currentNode = top.getItem();
			++pqOps;
			bwPoppedFromQueue[currentNode] = true;
			bwHeapItemForNode[currentNode] = -1;

			ASSERT(top.getKey() == bwDistances[currentNode],
					"Node distances inconsistent!")
			// current node has been settled by both queues
			if (fwPoppedFromQueue[currentNode]) {
				break;
			}

			IncomingEdgeIterator iterator = graph.incomingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getSource();

				const double relaxedDistance = bwDistances[currentNode]
						+ edge.getWeight();
				const unsigned int pathCost = bwDistances[currentNode]
						+ edge.getWeight() + fwDistances[other];

				if (fwDistances[other] != maxValue
						&& pathCost < minimalTotalDistance) {
					minimalTotalDistance = pathCost;
					meetingPoint = other;
				}

				if (maxValue == bwDistances[other]) {

					ASSERT(bwHeapItemForNode[other] == -1,
							"Item should not be included in backward PQ!");

					const unsigned int newItem = bwPQ.insert(other,
							relaxedDistance);
					bwDistances[other] = relaxedDistance;
					bwHeapItemForNode[other] = newItem;

				} else if (bwDistances[other] > relaxedDistance) {

					ASSERT(bwHeapItemForNode[other] != -1,
							"Item should be included in backward PQ!");

					const unsigned int othersHeapItem = bwHeapItemForNode[other];
					bwDistances[other] = relaxedDistance;
					bwPQ.decreaseKey(othersHeapItem, relaxedDistance);
				}
			} // iteration over outgoing edges of current node
		}

	}

	const double distance =
			meetingPoint != -1 ?
					fwDistances[meetingPoint] + bwDistances[meetingPoint] :
					maxValue;

	delete &fwPQ;
	delete[] fwDistances;
	delete[] fwHeapItemForNode;
	delete[] fwPoppedFromQueue;

	delete &bwPQ;
	delete[] bwDistances;
	delete[] bwHeapItemForNode;
	delete[] bwPoppedFromQueue;

	runtimeTimer.stop();
	const double calculationTime = runtimeTimer.elapsed();
	const CalculationResult result(distance, calculationTime, pqOps,
			"heap, bidirectional");
	return result;

}
const CalculationResult BHDijkstra::runGoalDirected(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {
	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	unsigned int pqOps = 0;

	// Data structures
	BinaryHeap &pq = *(new BinaryHeap());
	unsigned int *distances = new unsigned int[nodeCount];
	int *heapItemForNode = new int[nodeCount];

	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		heapItemForNode[i] = -1;
	}

	//Initialize source node
	distances[source] = 0;
	const unsigned int srcHeapItem = pq.insert(source, 0U);
	heapItemForNode[source] = srcHeapItem;

	while (!pq.isEmpty()) {
		const HeapItem &top = pq.min();
		pq.deleteMin();
		++pqOps;

		const unsigned int currentNode = top.getItem();
		heapItemForNode[currentNode] = -1;

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
						+ graph.distanceBound(other, target);
				if (distances[other] > relaxedDistance) {
					if (-1 == heapItemForNode[other]) {

						const unsigned int newItem = pq.insert(other,
								goalDistance);
						distances[other] = relaxedDistance;
						heapItemForNode[other] = newItem;

					} else {
						const unsigned int othersHeapItem =
								heapItemForNode[other];
						distances[other] = relaxedDistance;
						pq.decreaseKey(othersHeapItem, goalDistance);

					}
				} // relaxation condition met
			} // iteration over neighbors of current node
		}
	}

	const unsigned int distance = distances[target];

	delete &pq;
	delete[] distances;
	delete[] heapItemForNode;

	runtimeTimer.stop();
	const double calculationTime = runtimeTimer.elapsed();
	const CalculationResult result(distance, calculationTime, pqOps,
			"heap, A*");
	return result;
}
