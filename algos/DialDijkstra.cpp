#include "DialDijkstra.hpp"

#include "../common/CalculationResult.hpp"
#include "../common/AdjacencyArray.hpp"
#include "../common/Timer.hpp"
#include "../common/assert.hpp"

#include <list>
#include <limits>
#include <iterator>
#include <iostream>
#include <tr1/memory>

using std::tr1::shared_ptr;
using std::list;

const CalculationResult DialDijkstra::runStandard(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {
	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();

	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxEdgeLength = graph.getMaxEdgeLength();
	const unsigned int bucketCount = maxEdgeLength + 1;
	unsigned int pqOps = 0;

	unsigned int *distances = new unsigned int[nodeCount];
	std::list<unsigned int>::iterator *bqItemForNode = new std::list<
			unsigned int>::iterator[nodeCount];
	int *bqIndexForNode = new int[nodeCount];

	std::list<unsigned int> *bucketQueue =
			new std::list<unsigned int>[bucketCount];

	unsigned int elementsInBQ = 0;

	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		bqIndexForNode[i] = -1;
	}

	unsigned int currentPositionInBQ = 0; // the index which holds the nodes to be selected next
	bucketQueue[currentPositionInBQ].push_back(source);
	bqItemForNode[source] = --bucketQueue[currentPositionInBQ].end();
	++elementsInBQ;
	distances[source] = 0;
	while (elementsInBQ) {
		// Retrieve a non-empty bucket (guaranteed to exist)
		while (bucketQueue[currentPositionInBQ].empty()) {
			currentPositionInBQ = (currentPositionInBQ + 1) % bucketCount;
		}
		std::list<unsigned int> &currentBucket =
				bucketQueue[currentPositionInBQ];

		// Pop the next element from the queue
		const unsigned int currentNode = currentBucket.front();
		currentBucket.pop_front();

		--elementsInBQ;
		++pqOps;

		if (target == currentNode) {
			break;
		} else {
			OutgoingEdgeIterator iterator = graph.outgoingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getTarget();

				const double relaxedDistance = distances[currentNode]
						+ edge.getWeight();

				const unsigned int targetBucket = (currentPositionInBQ
						+ edge.getWeight()) % bucketCount;
				std::list<unsigned int> &bucket = bucketQueue[targetBucket];

				if (maxValue == distances[other]) {

					bucket.push_back(other);

					bqItemForNode[other] = --bucket.end();
					bqIndexForNode[other] = targetBucket;
					distances[other] = relaxedDistance;
					++elementsInBQ;
				} else if (distances[other] > relaxedDistance) {

					// Erase from old bucket
					std::list<unsigned int>::iterator oldIter =
							bqItemForNode[other];
					std::list<unsigned int> &oldBucket =
							bucketQueue[bqIndexForNode[other]];
					oldBucket.erase(oldIter);

					// Insert into new bucket
					std::list<unsigned int>::iterator iter = bucket.insert(
							bucket.end(), other);
					bqItemForNode[other] = iter;
					bqIndexForNode[other] = targetBucket;

					distances[other] = relaxedDistance;
				}
			} // iteration over neighbors of current node
		}

	}

	const double distance = distances[target];

	delete[] distances;
	delete[] bqIndexForNode;
	delete[] bqItemForNode;
	delete[] bucketQueue;

	runtimeTimer.stop();
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps, "dial");
	return result;
}

const CalculationResult DialDijkstra::runBidirectional(
		const AdjacencyArray &graph, const unsigned int source,
		const unsigned int target) {
	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();

	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxEdgeLength = graph.getMaxEdgeLength();
	const unsigned int bucketCount = maxEdgeLength + 1;
	unsigned int pqOps = 0;

	// Coordination between queues
	unsigned int minimalTotalDistance = maxValue;
	int meetingPoint = -1;

	// FORWARD
	unsigned int *fwDistances = new unsigned int[nodeCount];
	bool *fwPoppedFromQueue = new bool[nodeCount];
	std::list<unsigned int>::iterator *fwBqItemForNode = //
			new std::list<unsigned int>::iterator[nodeCount];
	int *fwBqIndexForNode = new int[nodeCount];
	std::list<unsigned int> *fwBucketQueue =
			new std::list<unsigned int>[bucketCount];

	unsigned int fwElementsInBQ = 0;
	unsigned int fwCurrentPositionInBQ = 0;

	// BACKWARD
	unsigned int *bwDistances = new unsigned int[nodeCount];
	bool *bwPoppedFromQueue = new bool[nodeCount];
	std::list<unsigned int>::iterator *bwBqItemForNode = //
			new std::list<unsigned int>::iterator[nodeCount];
	int *bwBqIndexForNode = new int[nodeCount];
	std::list<unsigned int> *bwBucketQueue =
			new std::list<unsigned int>[bucketCount];

	unsigned int bwElementsInBQ = 0;
	unsigned int bwCurrentPositionInBQ = 0;

	for (unsigned int i = 0; i < nodeCount; ++i) {
		fwDistances[i] = maxValue;
		fwBqIndexForNode[i] = -1;
		fwPoppedFromQueue[i] = false;

		bwDistances[i] = maxValue;
		bwBqIndexForNode[i] = -1;
		bwPoppedFromQueue[i] = false;
	}

	// Initialize forward BQ
	fwBucketQueue[fwCurrentPositionInBQ].push_back(source);
	fwBqItemForNode[source] = --fwBucketQueue[fwCurrentPositionInBQ].end();
	++fwElementsInBQ;
	fwDistances[source] = 0;

	// Initialize backward BQ
	bwBucketQueue[bwCurrentPositionInBQ].push_back(target);
	bwBqItemForNode[target] = --bwBucketQueue[bwCurrentPositionInBQ].end();
	++bwElementsInBQ;
	bwDistances[target] = 0;

	while (fwElementsInBQ != 0 || bwElementsInBQ != 0) {

		// Retrieve a non-empty bucket in both queues
		while (fwElementsInBQ != 0
				&& fwBucketQueue[fwCurrentPositionInBQ].empty()) {
			fwCurrentPositionInBQ = (fwCurrentPositionInBQ + 1) % bucketCount;
		}
		while (bwElementsInBQ != 0
				&& bwBucketQueue[bwCurrentPositionInBQ].empty()) {
			bwCurrentPositionInBQ = (bwCurrentPositionInBQ + 1) % bucketCount;
		}

		std::list<unsigned int> &fwCurrentBucket =
				fwBucketQueue[fwCurrentPositionInBQ];
		std::list<unsigned int> &bwCurrentBucket =
				bwBucketQueue[bwCurrentPositionInBQ];

		const unsigned int fwCurrentNode = fwCurrentBucket.front();
		const unsigned int bwCurrentNode = bwCurrentBucket.front();

		// FORWARD queue selected
		if (bwElementsInBQ == 0
				|| (fwElementsInBQ != 0
						&& fwDistances[fwCurrentNode]
								<= bwDistances[bwCurrentNode])) {

			// Pop node from queue
			fwCurrentBucket.pop_front();
			--fwElementsInBQ;
			++pqOps;
			fwPoppedFromQueue[fwCurrentNode] = true;

			// Search spaces have met
			if (bwPoppedFromQueue[fwCurrentNode]) {
				break;
			}

			// Relax neighbors
			OutgoingEdgeIterator iterator = graph.outgoingOf(fwCurrentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getTarget();

				const double relaxedDistance = fwDistances[fwCurrentNode]
						+ edge.getWeight();

				const unsigned int targetBucket = (fwCurrentPositionInBQ
						+ edge.getWeight()) % bucketCount;
				std::list<unsigned int> &bucket = fwBucketQueue[targetBucket];

				const unsigned int pathCost = fwDistances[fwCurrentNode]
						+ edge.getWeight() + bwDistances[other];

				// Update global minimal distance
				if (bwDistances[other] != maxValue
						&& pathCost < minimalTotalDistance) {
					minimalTotalDistance = pathCost;
					meetingPoint = other;
				}

				if (maxValue == fwDistances[other]) {

					bucket.push_back(other);

					fwBqItemForNode[other] = --bucket.end();
					fwBqIndexForNode[other] = targetBucket;
					fwDistances[other] = relaxedDistance;
					++fwElementsInBQ;
				} else if (fwDistances[other] > relaxedDistance) {

					// Erase from old bucket
					std::list<unsigned int>::iterator oldIter =
							fwBqItemForNode[other];
					std::list<unsigned int> &oldBucket =
							fwBucketQueue[fwBqIndexForNode[other]];
					oldBucket.erase(oldIter);

					// Insert into new bucket
					std::list<unsigned int>::iterator iter = bucket.insert(
							bucket.end(), other);
					fwBqItemForNode[other] = iter;
					fwBqIndexForNode[other] = targetBucket;

					fwDistances[other] = relaxedDistance;
				}
			} // iteration over neighbors of current node
		}
		// BACKWARD queue selected
		else {
			// Pop node from backward queue
			bwCurrentBucket.pop_front();
			--bwElementsInBQ;
			++pqOps;
			bwPoppedFromQueue[bwCurrentNode] = true;

			// Search spaces have met
			if (fwPoppedFromQueue[bwCurrentNode]) {
				break;
			}

			// Relax neighbors
			IncomingEdgeIterator iterator = graph.incomingOf(bwCurrentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getSource();

				const double relaxedDistance = bwDistances[bwCurrentNode]
						+ edge.getWeight();

				const unsigned int targetBucket = (bwCurrentPositionInBQ
						+ edge.getWeight()) % bucketCount;
				std::list<unsigned int> &bucket = bwBucketQueue[targetBucket];
				unsigned int pathCost = bwDistances[bwCurrentNode]
						+ edge.getWeight() + fwDistances[other];

				// Update global minimal distance
				if (fwDistances[other] != maxValue
						&& pathCost < minimalTotalDistance) {
					minimalTotalDistance = pathCost;
					meetingPoint = other;
				}

				if (maxValue == bwDistances[other]) {

					bucket.push_back(other);

					bwBqItemForNode[other] = --bucket.end();
					bwBqIndexForNode[other] = targetBucket;
					bwDistances[other] = relaxedDistance;
					++bwElementsInBQ;
				} else if (bwDistances[other] > relaxedDistance) {

					// Erase from old bucket
					std::list<unsigned int>::iterator oldIter =
							bwBqItemForNode[other];
					std::list<unsigned int> &oldBucket =
							bwBucketQueue[bwBqIndexForNode[other]];
					oldBucket.erase(oldIter);

					// Insert into new bucket
					std::list<unsigned int>::iterator iter = bucket.insert(
							bucket.end(), other);
					bwBqItemForNode[other] = iter;
					bwBqIndexForNode[other] = targetBucket;

					bwDistances[other] = relaxedDistance;
				}
			} // iteration over neighbors of current node
		}
	}

	double distance =
			meetingPoint != -1 ?
					fwDistances[meetingPoint] + bwDistances[meetingPoint] :
					maxValue;

	if (source == target) {
		distance = 0;
	}

	delete[] fwDistances;
	delete[] fwBucketQueue;
	delete[] fwBqIndexForNode;
	delete[] fwBqItemForNode;
	delete[] fwPoppedFromQueue;

	delete[] bwDistances;
	delete[] bwBucketQueue;
	delete[] bwBqIndexForNode;
	delete[] bwBqItemForNode;
	delete[] bwPoppedFromQueue;

	runtimeTimer.stop();
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps,
			"dial, bidirectional");
	return result;
}
const CalculationResult DialDijkstra::runGoalDirected(
		const AdjacencyArray &graph, const unsigned int source,
		const unsigned int target) {
	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();
	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxEdgeLength = graph.getMaxEdgeLength();
	unsigned int pqOps = 0;

	unsigned int *distances = new unsigned int[nodeCount];
	unsigned int maxDistanceToTarget = 0;
	unsigned int *distanceToTarget = new unsigned int[nodeCount];

	for (unsigned int i = 0; i < nodeCount; ++i) {

		distanceToTarget[i] = static_cast<int>(graph.distanceBound(i, target));
		maxDistanceToTarget = std::max(distanceToTarget[i],
				maxDistanceToTarget);
	}

	const unsigned int bucketCount = maxEdgeLength + maxDistanceToTarget + 1;

	std::list<unsigned int>::iterator *bqItemForNode = //
			new std::list<unsigned int>::iterator[nodeCount];
	int *bqIndexForNode = new int[nodeCount];
	std::list<unsigned int> *bucketQueue =
			new std::list<unsigned int>[bucketCount];

	unsigned int elementsInBQ = 0;

	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		bqIndexForNode[i] = -1;
		bqItemForNode[i] = std::list<unsigned int>::iterator();
	}

	unsigned int currentPositionInBQ = 0; // the index which holds the nodes to be selected next
	bucketQueue[currentPositionInBQ].push_back(source);
	bqItemForNode[source] = --bucketQueue[currentPositionInBQ].end();
	++elementsInBQ;
	distances[source] = 0;
	while (elementsInBQ) {
		// Retrieve a non-empty bucket (guaranteed to exist)
		while (bucketQueue[currentPositionInBQ].empty()) {
			currentPositionInBQ = (currentPositionInBQ + 1) % bucketCount;
		}
		std::list<unsigned int> &currentBucket =
				bucketQueue[currentPositionInBQ];

		// Pop the next element from the queue
		const unsigned int currentNode = currentBucket.front();
		currentBucket.pop_front();
		bqItemForNode[currentNode] = std::list<unsigned int>::iterator();
		bqIndexForNode[currentNode] = -1;

		const double distanteOfCurrentToTarget = graph.distanceBound(
				currentNode, target);

		--elementsInBQ;
		++pqOps;

		if (target == currentNode) {
			break;
		} else {
			OutgoingEdgeIterator iterator = graph.outgoingOf(currentNode);
			while (iterator.hasNext()) {
				const Edge &edge = iterator.next();
				const unsigned int other = edge.getTarget();

				const double relaxedDistance = distances[currentNode]
						+ edge.getWeight();
				const unsigned int targetBucketOffset =
						static_cast<unsigned int>(edge.getWeight()
								+ distanceToTarget[other]
								- distanteOfCurrentToTarget);

				const unsigned int targetBucket = (currentPositionInBQ
						+ targetBucketOffset) % bucketCount;
				std::list<unsigned int> &bucket = bucketQueue[targetBucket];

				if (distances[other] > relaxedDistance) {
					if (-1 == bqIndexForNode[other]) {

						bucket.push_back(other);

						bqItemForNode[other] = --bucket.end();
						bqIndexForNode[other] = targetBucket;
						distances[other] = relaxedDistance;
						++elementsInBQ;

					} else {

						// Erase from old bucket
						std::list<unsigned int>::iterator oldIter =
								bqItemForNode[other];
						std::list<unsigned int> &oldBucket =
								bucketQueue[bqIndexForNode[other]];
						oldBucket.erase(oldIter);

						// Insert into new bucket
						std::list<unsigned int>::iterator iter = bucket.insert(
								bucket.end(), other);
						bqItemForNode[other] = iter;
						bqIndexForNode[other] = targetBucket;

						distances[other] = relaxedDistance;
					}
				} // relaxation condition met
			} // iteration over neighbors of current node
		}

	}

	const double distance = distances[target];

	delete[] distances;
	delete[] distanceToTarget;
	delete[] bqIndexForNode;
	delete[] bqItemForNode;
	delete[] bucketQueue;

	runtimeTimer.stop();
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps,
			"dial, A*");
	return result;
}
