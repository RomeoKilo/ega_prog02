#include "CalculationResult.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>

std::string CalculationResult::formattingHeader() {
	std::ostringstream stream;
	stream << std::setw(10) << std::left << "d(s,t)";
	stream << "|" << std::setw(10) << std::left << "time(ms)";
	stream << "|" << std::setw(10) << std::left << "PQ-ops";
	stream << "|" << std::setw(46) << std::left << "params";
	stream << "|" << std::endl;
	for (unsigned int i = 0; i < 80; ++i) {
		stream << "-";
	}
	stream << std::endl;
	return stream.str();
}

std::string CalculationResult::format(
		const std::vector<CalculationResult> &results) {

	std::ostringstream stream;
	stream << CalculationResult::formattingHeader();
	for (std::vector<CalculationResult>::const_iterator iter = results.begin();
			iter != results.end(); ++iter) {
		const CalculationResult &result = *iter;
		stream << result.format();
	}
	return stream.str();
}

std::string CalculationResult::format() const {
	std::ostringstream stream;
	if (_distance != std::numeric_limits<unsigned int>::max()) {
		stream << std::setw(10) << std::left << _distance;
	} else {
		stream << std::setw(10) << std::left << "Unconnected";
	}
	stream << "|" << std::setw(10) << std::left << _calculationTime;
	stream << "|" << std::setw(10) << std::left << _pqOperations;
	stream << "|" << std::setw(46) << std::left << _parameters;
	stream << "|" << std::endl;
	return stream.str();
}

std::string CalculationResult::toString() const {
	std::ostringstream stream;
	stream << CalculationResult::formattingHeader();
	stream << this->format();
	return stream.str();
}
