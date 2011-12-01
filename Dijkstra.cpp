#include "common/BinaryHeap.hpp"
#include "common/AdjacencyArray.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

using std::tr1::shared_ptr;

struct STR {
	int i;
	STR(int i) :
			i(i) {
	}
};

int main(int argc, char *argv[]) {

	shared_ptr<AdjacencyArray> adjArr = AdjacencyArray::fromFile("../../prog02/samples/sample2.graph", true);

	adjArr->print();

	return 0;

	int errorcode = 0;
	if (argc == 1) {
		std::cerr << "Usage: Dijkstra <filename> -s <src> -t <dst> <options>"
				<< std::endl;
	}

	std::string filename = argv[1];

	if (!std::ifstream(filename.c_str())) {
		std::cerr << "File '" << filename << "' is not accessible!" << std::endl;
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

	if (!useBinaryHeap && !useDialsImplementation) {
		std::cerr
				<< "Either binary heap or Dial's implementation must be chosen! (-h or -d)"
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
		// business logic here
		// print some statistics
	} else {
		std::cerr
				<< "Encountered problem while parsing command line options. Will now abort."
				<< std::endl;
	}
	std::cerr << "file: " << filename << std::endl;
	std::cerr << "bidi " << useBidirectional << std::endl;
	std::cerr << "binary " << useBinaryHeap << std::endl;
	std::cerr << "goal " << useGoalDirected << std::endl;
	std::cerr << "dial " << useDialsImplementation << std::endl;
	std::cerr << source << std::endl;
	std::cerr << target << std::endl;
	return errorcode;
}
