#include "DialDijkstra.hpp"

#include "../common/CalculationResult.hpp"
#include "../common/AdjacencyArray.hpp"
#include "../common/Timer.hpp"
#include "../common/assert.hpp"

#include <list>
#include <limits>
#include <iterator>
#include <iostream>

const CalculationResult DialDijkstra::runStandard(const AdjacencyArray &graph,
		const unsigned int source, const unsigned int target) {

	const unsigned int nodeCount = graph.getNodeCount();
	const unsigned int maxEdgeLength = graph.getMaxEdgeLength();
	const unsigned int bucketCount = maxEdgeLength + 1;
	unsigned int pqOps = 0;
	ASSERT(source < nodeCount, "Invalid node id for source!");
	ASSERT(target < nodeCount, "Invalid node id for target!");

	Timer runtimeTimer;
	runtimeTimer.start();

	const unsigned int maxValue = std::numeric_limits<unsigned int>::max();

	unsigned int distances[nodeCount];
	std::list<unsigned int>::iterator bqItemForNode[nodeCount];
	int bqIndexForNode[nodeCount];
	std::list<unsigned int> bucketQueue[bucketCount];

	unsigned int elementsInBQ = 0;

	for (unsigned int i = 0; i < nodeCount; ++i) {
		distances[i] = maxValue;
		bqIndexForNode[i] = -1;
	}

	unsigned int currentPositionInBQ = 0; // the index which holds the nodes to be selected next
	bucketQueue[currentPositionInBQ].push_back(source);
	bqItemForNode[source] =  --bucketQueue[currentPositionInBQ].end();
	++elementsInBQ;
	distances[source] = 0;
	while (elementsInBQ) {
		// Retrieve a non-empty bucket (guaranteed to exist)
		while (bucketQueue[currentPositionInBQ].empty()) {
			currentPositionInBQ = (currentPositionInBQ + 1) % bucketCount;
		}
		std::list<unsigned int> &currentBucket = bucketQueue[currentPositionInBQ];

		// Pop the next element from the queue
		const unsigned int currentNode = currentBucket.front();
		currentBucket.pop_front();

		--elementsInBQ;
		++pqOps;

//		std::cout << "\nDM: " << currentNode << " w= " << distances[currentNode]
//				<< std::endl;

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
//					std::cout << "INS: " << other << " -> " << targetBucket
//							<< "(" << edge << ")" << std::endl;
				} else if (distances[other] > relaxedDistance) {

					// Erase from old bucket
					std::list<unsigned int>::iterator oldIter =
							bqItemForNode[other];
					std::list<unsigned int> &oldBucket =
							bucketQueue[bqIndexForNode[other]];
					oldBucket.erase(oldIter);

//					std::cout << "DK: " << other << " F: "
//							<< bqIndexForNode[other] << " T:" << targetBucket
//							<< std::endl;
					// Insert into new bucket
					std::list<unsigned int>::iterator iter = bucket.insert(
							bucket.end(), other);
					bqItemForNode[other] = iter;
					bqIndexForNode[other] = targetBucket;

					distances[other] = relaxedDistance;
//					print(bucketQueue, bucketCount, currentPositionInBQ);
				}
			} // iteration over neighbors of current node
		}

	}

	runtimeTimer.stop();
	double distance = distances[target];
	const double calculationTime = runtimeTimer.elapsed();

	const CalculationResult result(distance, calculationTime, pqOps, "dial");
	return result;
}

void DialDijkstra::print(std::list<unsigned int> *bucketQueue, unsigned int len,
		unsigned int current) {
	std::cout << "\tCurrent: " << current << std::endl;
	for (unsigned int i = 0; i < len; ++i) {
		std::list<unsigned int> &currentBucket = bucketQueue[i];
		if (!bucketQueue[i].empty()) {
			std::cout << "\t" << i << ":";
//			std::ostream_iterator<unsigned int> out_it(std::cout, ", ");
//			std::copy(bucketQueue[i].begin(), bucketQueue[i].end(), out_it);
			for (std::list<unsigned int>::iterator iter = currentBucket.begin();
					iter != currentBucket.end(); ++iter) {
				std::cout << *iter << ", ";
			}
			std::cout << std::endl;
		}
	}
}

const CalculationResult DialDijkstra::runBidirectional(
		const AdjacencyArray &graph, const unsigned int source,
		const unsigned int target) {
}
const CalculationResult DialDijkstra::runGoalDirected(
		const AdjacencyArray &graph, const unsigned int source,
		const unsigned int target) {
}
