#ifndef CALCULATION_RESULT_HPP_
#define CALCULATION_RESULT_HPP_

#include <string>
#include <vector>

class CalculationResult {
private:
	double _distance;
	double _calculationTime;
	unsigned int _pqOperations;
	std::string _parameters;
public:
	/**
	 * Initializes the result with all necessary information.
	 * If distance is set to MAX_UINT, then this is equivalent to a
	 * non-existing route.
	 */
	CalculationResult(const double distance, const double calculationTime,
			const unsigned int pqOps, const std::string &parameters) :
			_distance(distance), //
			_calculationTime(calculationTime), //
			_pqOperations(pqOps), //
			_parameters(parameters) {
	}

	static std::string format(const std::vector<CalculationResult> &results);
	static CalculationResult average(const std::vector<CalculationResult> &results);
	static std::string formattingHeader();
	std::string format() const;
	std::string toString() const;
};

#endif /* CALCULATION_RESULT_HPP_ */
