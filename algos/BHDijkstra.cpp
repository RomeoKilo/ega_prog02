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

	Timer runtimeTimer;
	runtimeTimer.start();
	unsigned int minimalTotalDistance = maxValue;
	int meetingPoint = -1;

	BinaryHeap forwardPQ;
	BinaryHeap backwardPQ;

	unsigned int forwardDistances[nodeCount];
	bool forwardPoppedFromQueue[nodeCount];
	unsigned int backwardDistances[nodeCount];
	bool backwardPoppedFromQueue[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		forwardDistances[i] = maxValue;
		forwardPoppedFromQueue[i] = false;
		backwardDistances[i] = maxValue;
		backwardPoppedFromQueue[i] = false;
	}
	unsigned int forwardHeapItemForNode[nodeCount];
	unsigned int backwardHeapItemForNode[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		forwardHeapItemForNode[i] = maxValue;
		backwardHeapItemForNode[i] = maxValue;
	}

	forwardHeapItemForNode[source] = forwardPQ.insert(source, 0);
	forwardDistances[source] = 0;

	backwardHeapItemForNode[source] = backwardPQ.insert(target, 0);
	backwardDistances[target] = 0;

	while (!forwardPQ.isEmpty() && !backwardPQ.isEmpty()) {
		// Select the queue with the smaller min-key to do the next step
		if (forwardPQ.min().getKey() <= backwardPQ.min().getKey()) {
			const HeapItem &top = forwardPQ.min();
			forwardPQ.deleteMin();
			const unsigned int currentNode = top.getItem();
			forwardPoppedFromQueue[currentNode] = true;
			++pqOps;

			ASSERT(top.getKey() == forwardDistances[currentNode],
					"Node distances inconsistent!")

			// current node has been settled by both queues
			if (backwardPoppedFromQueue[currentNode]) {
				break;
			}

			OutgoingEdgeIterator iterator = graph.outgoingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getTarget();

				const double relaxedDistance = forwardDistances[currentNode]
						+ edge.getWeight();
				const unsigned int pathCost = forwardDistances[currentNode]
						+ edge.getWeight() + backwardDistances[other];

				if (backwardDistances[other] != maxValue
						&& pathCost < minimalTotalDistance) {
					minimalTotalDistance = pathCost;
					meetingPoint = other;
				}

				if (maxValue == forwardDistances[other]) {

					ASSERT(forwardHeapItemForNode[other] == maxValue,
							"Item should not be included in forward PQ!");

					const unsigned int newItem = forwardPQ.insert(other,
							relaxedDistance);
					forwardDistances[other] = relaxedDistance;
					forwardHeapItemForNode[other] = newItem;
				} else if (forwardDistances[other] > relaxedDistance) {

					ASSERT(forwardHeapItemForNode[other] != maxValue,
							"Item should be included in forward PQ!");

					const unsigned int othersHeapItem =
							forwardHeapItemForNode[other];
					forwardDistances[other] = relaxedDistance;
					forwardPQ.decreaseKey(othersHeapItem, relaxedDistance);
				}
			} // iteration over outgoing edges of current node
		} else { // Do one step with the backward search
			const HeapItem &top = backwardPQ.min();
			backwardPQ.deleteMin();
			const unsigned int currentNode = top.getItem();
			++pqOps;
			backwardPoppedFromQueue[currentNode] = true;

			ASSERT(top.getKey() == backwardDistances[currentNode],
					"Node distances inconsistent!")
			// current node has been settled by both queues
			if (forwardPoppedFromQueue[currentNode]) {
				break;
			}

			IncomingEdgeIterator iterator = graph.incomingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getSource();

				const double relaxedDistance = backwardDistances[currentNode]
						+ edge.getWeight();
				const unsigned int pathCost = backwardDistances[currentNode]
						+ edge.getWeight() + forwardDistances[other];

				if (forwardDistances[other] != maxValue
						&& pathCost < minimalTotalDistance) {
					minimalTotalDistance = pathCost;
					meetingPoint = other;
				}

				if (maxValue == backwardDistances[other]) {

					ASSERT(backwardHeapItemForNode[other] == maxValue,
							"Item should not be included in backward PQ!");

					const unsigned int newItem = backwardPQ.insert(other,
							relaxedDistance);
					backwardDistances[other] = relaxedDistance;
					backwardHeapItemForNode[other] = newItem;

				} else if (backwardDistances[other] > relaxedDistance) {

					ASSERT(backwardHeapItemForNode[other] != maxValue,
							"Item should be included in backward PQ!");

					const unsigned int othersHeapItem =
							backwardHeapItemForNode[other];
					backwardDistances[other] = relaxedDistance;
					backwardPQ.decreaseKey(othersHeapItem, relaxedDistance);
				}
			} // iteration over outgoing edges of current node
		}

	}

	runtimeTimer.stop();
	const double distance = forwardDistances[meetingPoint]
			+ backwardDistances[meetingPoint];
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
	int heapItemForNode[nodeCount];
	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		skewedDistances[i] = maxValue;
		heapItemForNode[i] = -1;
	}

	distances[source] = 0;
	unsigned int srcHeapItem = pq.insert(source, 0U);
	heapItemForNode[source] = srcHeapItem;
	while (!pq.isEmpty()) {
		const HeapItem &top = pq.min();
		pq.deleteMin();
		++pqOps;

		const unsigned int currentNode = top.getItem();
		heapItemForNode[currentNode] = -1;

//		std::cout << "DM: " << currentNode << " w=" << distances[currentNode]
//				<< " b=" << top.getKey() << std::endl;

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

//						std::cout << "\tINS: " << other << " w="
//								<< relaxedDistance << " 'b="
//								<< (goalDistance - distances[currentNode])
//								<< std::endl;
					} else {
						const unsigned int othersHeapItem =
								heapItemForNode[other];
						distances[other] = relaxedDistance;
						pq.decreaseKey(othersHeapItem, goalDistance);

//						std::cout << "\tDK: " << other << " w="
//								<< relaxedDistance << std::endl;
					}
				} // relaxation condition met
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
