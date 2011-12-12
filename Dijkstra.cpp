#include "algos/BHDijkstra.hpp"
#include "algos/DialDijkstra.hpp"
#include "common/AdjacencyArray.hpp"
#include "common/CalculationResult.hpp"
#include "common/BinaryHeap.hpp"

#include <tr1/memory>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

using std::tr1::shared_ptr;

int main(int argc, char *argv[]) {

	int errorcode = 0;
	if (argc == 1) {
		std::cerr << "Usage: Dijkstra <filename> -s <src> -t <dst> <options>"
				<< std::endl;
		return 1;
	}

	std::string filename = argv[1];

	if (!std::ifstream(filename.c_str())) {
		std::cerr << "File '" << filename << "' is not accessible!"
				<< std::endl;
		errorcode = -4;
	}

	int source = -1;
	int target = -1;
	bool useBinaryHeap = false;
	bool useDialsImplementation = false;
	bool useBidirectional = false;
	bool useGoalDirected = false;
	bool runComparison = false;

	char c;
	while ((c = getopt(argc, argv, "s:t:hdbza")) != -1) {
		switch (c) {
		case 's':
			source = atoi(optarg);
			break;
		case 't':
			target = atoi(optarg);
			break;
		case 'h':
			if (!useDialsImplementation) {
				useBinaryHeap = true;
			} else {
				std::cerr << "Warning! Dial opt is enabled - ignoring -h option"
						<< std::endl;
			}
			break;
		case 'd':
			if (!useBinaryHeap) {
				useDialsImplementation = true;
			} else {
				std::cerr
						<< "Warning! Binary Heap opt is enabled - ignoring -d option"
						<< std::endl;
			}
			break;
		case 'b':
			useBidirectional = true;
			break;
		case 'z':
			useGoalDirected = true;
			break;
		case 'a':
			runComparison = true;
			break;
		}
	}

	// sanity check

	if (!useBinaryHeap && !useDialsImplementation && !runComparison) {
		std::cerr
				<< "Either binary comparison mode,  heap or Dial's implementation must be chosen! (-a,-h or -d)"
				<< std::endl;
		errorcode = -1;
	}
	if (source < 0) {
		std::cerr
				<< "Source node has been set inapproriately (or even left out?)"
				<< std::endl;
		errorcode = -2;
	}
	if (target < 0) {
		std::cerr
				<< "Target node has been set inapproriately (or even left out?)"
				<< std::endl;
		errorcode = -3;
	}

	if (0 == errorcode) {
		const shared_ptr<const AdjacencyArray> adjArray =
				AdjacencyArray::fromFile(filename, true);
		const int nodeCount = adjArray->getNodeCount();
		const int edgeCount = adjArray->getEdgeCount();
		if (source >= nodeCount || target >= nodeCount) {
			std::cerr << "Node IDs (s=" << source << ",t=" << target
					<< ") outside node count (n=" << nodeCount << ")"
					<< std::endl;
			errorcode = -5;
		} else {
			std::cout << "File: " << filename << " with n=" << nodeCount
					<< " m=" << edgeCount  << std::endl;
			std::cout << "Query: s=" << source << ", t=" << target << std::endl;
			if (useBinaryHeap) {
				if (useBidirectional) {
					const CalculationResult result =
							BHDijkstra::runBidirectional(*adjArray, source,
									target);
					std::cout << result.toString() << std::endl;
				} else if (useGoalDirected) {
					const CalculationResult result =
							BHDijkstra::runGoalDirected(*adjArray, source,
									target);
					std::cout << result.toString() << std::endl;
				} else {
					const CalculationResult result = BHDijkstra::runStandard(
							*adjArray, source, target);
					std::cout << result.toString() << std::endl;
				}
			} else if (useDialsImplementation) {
				if (useBidirectional) {
					const CalculationResult result =
							DialDijkstra::runBidirectional(*adjArray, source,
									target);
					std::cout << result.toString() << std::endl;
				} else if (useGoalDirected) {
					const CalculationResult result =
							DialDijkstra::runGoalDirected(*adjArray, source,
									target);
					std::cout << result.toString() << std::endl;
				} else {
					const CalculationResult result = DialDijkstra::runStandard(
							*adjArray, source, target);
					std::cout << result.toString() << std::endl;
				}
			} else //comparison
			{
				std::vector<CalculationResult> results;
				results.push_back(
						BHDijkstra::runStandard(*adjArray, source, target));
				results.push_back(
						BHDijkstra::runBidirectional(*adjArray, source,
								target));
				results.push_back(
						BHDijkstra::runGoalDirected(*adjArray, source, target));

				results.push_back(
						DialDijkstra::runStandard(*adjArray, source, target));
				results.push_back(
						DialDijkstra::runBidirectional(*adjArray, source,
								target));
				results.push_back(
						DialDijkstra::runGoalDirected(*adjArray, source,
								target));
				std::cout << CalculationResult::format(results) << std::endl;
			}
		}
	} else {
		std::cerr
				<< "Encountered problem while parsing command line options. Will now abort."
				<< std::endl;
	}
	return errorcode;
}
