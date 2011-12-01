#include "CalculationResult.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>

std::string CalculationResult::format(
		const std::vector<CalculationResult> &results) {
	std::ostringstream stream;
	stream << std::setw(10) << "d(s,t)";
	stream << std::setw(10) << "time(ms)";
	stream << std::setw(10) << "PQ-ops";
	stream << std::setw(50) << "params";
	stream << std::endl;
	for (std::vector<CalculationResult>::const_iterator iter = results.begin(); iter != results.end(); ++iter)
	{
		const CalculationResult &result = *iter;
		stream << result.format();
	}
	return stream.str();
}

std::string CalculationResult::format() const {
	std::ostringstream stream;
	stream << std::setw(10) << _distance << std::endl;
	stream << std::setw(10) << _calculationTime << std::endl;
	stream << std::setw(10) << _pqOperations << std::endl;
	stream << std::setw(50) << _parameters << std::endl;
	stream << std::endl;
	return stream.str();
}
