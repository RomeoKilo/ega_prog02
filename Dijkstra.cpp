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
#include <ctime>

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
	bool runRandom = false;

	char c;
	while ((c = getopt(argc, argv, "s:t:hdbzar")) != -1) {
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
		case 'r':
			runRandom = true;
			break;
		}
	}

	// sanity check

	if (!useBinaryHeap && !useDialsImplementation && !runComparison && !runRandom) {
		std::cerr
				<< "Either comparison mode, random testing,  heap or Dial's implementation must be chosen! (-a,-r,-h or -d)"
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
					<< " m=" << edgeCount << std::endl;
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
			} else if (runComparison) //comparison
			{
				const unsigned int numRuns = 1;
				std::cout << "Running comparison tests with " << numRuns
						<< " runs per configuration..." << std::endl;
				std::vector<CalculationResult> globalResults;
				std::vector<CalculationResult> localResults;
				for (unsigned int i = 0; i < numRuns; ++i)
					localResults.push_back(
							BHDijkstra::runStandard(*adjArray, source, target));

				globalResults.push_back(
						CalculationResult::average(localResults));
				localResults.clear();
				for (unsigned int i = 0; i < numRuns; ++i)
					localResults.push_back(
							BHDijkstra::runBidirectional(*adjArray, source,
									target));

				globalResults.push_back(
						CalculationResult::average(localResults));
				localResults.clear();
				for (unsigned int i = 0; i < numRuns; ++i)
					localResults.push_back(
							BHDijkstra::runGoalDirected(*adjArray, source,
									target));

				globalResults.push_back(
						CalculationResult::average(localResults));
				localResults.clear();
				for (unsigned int i = 0; i < numRuns; ++i)
					localResults.push_back(
							DialDijkstra::runStandard(*adjArray, source,
									target));

				globalResults.push_back(
						CalculationResult::average(localResults));
				localResults.clear();
				for (unsigned int i = 0; i < numRuns; ++i)
					localResults.push_back(
							DialDijkstra::runBidirectional(*adjArray, source,
									target));

				globalResults.push_back(
						CalculationResult::average(localResults));
				localResults.clear();
				for (unsigned int i = 0; i < numRuns; ++i)
					localResults.push_back(
							DialDijkstra::runGoalDirected(*adjArray, source,
									target));

				globalResults.push_back(
						CalculationResult::average(localResults));
				localResults.clear();

				std::cout << CalculationResult::format(globalResults)
						<< std::endl;
			} else if (runRandom) {
				const unsigned int numRuns = target;
				srand(time(NULL));
				std::cout << "Running random tests with " << numRuns
						<< " runs..." << std::endl;

				std::vector<CalculationResult> globalResultsForBHStd;
				std::vector<CalculationResult> globalResultsForBHBi;
				std::vector<CalculationResult> globalResultsForBHGoal;
				std::vector<CalculationResult> globalResultsForDialStd;
				std::vector<CalculationResult> globalResultsForDialBi;
				std::vector<CalculationResult> globalResultsForDialGoal;

				for (unsigned int r = 0; r < numRuns; ++r) {
					const unsigned int randTarget = rand() % nodeCount;
					const unsigned int randSource = rand() % nodeCount;
                    std::cout << std::endl;
                    std::cout << "Run number: " << (r+1) << std::endl;
					std::cout << "Source: " << randSource << " -> Target: " << randTarget << std::endl;

					std::vector<CalculationResult> globalResults;
					std::vector<CalculationResult> localResults;
					for (unsigned int i = 0; i < numRuns; ++i)
						localResults.push_back(
								BHDijkstra::runStandard(*adjArray, randSource,
										randTarget));

					globalResults.push_back(
							CalculationResult::average(localResults));
					globalResultsForBHStd.push_back(
							CalculationResult::average(localResults));
					localResults.clear();
					for (unsigned int i = 0; i < numRuns; ++i)
						localResults.push_back(
								BHDijkstra::runBidirectional(*adjArray,
										randSource, randTarget));

					globalResults.push_back(
							CalculationResult::average(localResults));
					globalResultsForBHBi.push_back(
							CalculationResult::average(localResults));
					localResults.clear();
					for (unsigned int i = 0; i < numRuns; ++i)
						localResults.push_back(
								BHDijkstra::runGoalDirected(*adjArray,
										randSource, randTarget));

					globalResults.push_back(
							CalculationResult::average(localResults));
					globalResultsForBHGoal.push_back(
							CalculationResult::average(localResults));
					localResults.clear();
					for (unsigned int i = 0; i < numRuns; ++i)
						localResults.push_back(
								DialDijkstra::runStandard(*adjArray, randSource,
										randTarget));

					globalResults.push_back(
							CalculationResult::average(localResults));
					globalResultsForDialStd.push_back(
							CalculationResult::average(localResults));
					localResults.clear();
					for (unsigned int i = 0; i < numRuns; ++i)
						localResults.push_back(
								DialDijkstra::runBidirectional(*adjArray,
										randSource, randTarget));

					globalResults.push_back(
							CalculationResult::average(localResults));
					globalResultsForDialBi.push_back(
							CalculationResult::average(localResults));
					localResults.clear();

					for (unsigned int i = 0; i < numRuns; ++i)
						localResults.push_back(
								DialDijkstra::runGoalDirected(*adjArray,
										randSource, randTarget));

					globalResults.push_back(
							CalculationResult::average(localResults));
					globalResultsForDialGoal.push_back(
							CalculationResult::average(localResults));
					localResults.clear();

					std::cout << CalculationResult::format(globalResults)
							<< std::endl;
				} // iteration over random test cases
				
				std::vector<CalculationResult> overallResults;
				overallResults.push_back(CalculationResult::average(globalResultsForBHStd));
				overallResults.push_back(CalculationResult::average(globalResultsForBHBi));
				overallResults.push_back(CalculationResult::average(globalResultsForBHGoal));
				overallResults.push_back(CalculationResult::average(globalResultsForDialStd));
				overallResults.push_back(CalculationResult::average(globalResultsForDialBi));
				overallResults.push_back(CalculationResult::average(globalResultsForDialGoal));

                std::cout << "Overall results of random testing:\n" << std::endl;
				std::cout << CalculationResult::format(overallResults)
						<< std::endl;
			}
		}
	} else {
		std::cerr
				<< "Encountered problem while parsing command line options. Will now abort."
				<< std::endl;
	}
	return errorcode;
}
