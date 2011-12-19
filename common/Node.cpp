#include "Node.hpp"

#include <cmath>

static const double PI = 3.141592653589793238462643383279502884197;

/**
 * Returns a lower bound for the distance of the two given nodes.
 * The distance is measured in decimeters.
 */
double Node::lowerDistanceBound(const Node &node1, const Node &node2) {
	double lat1 = node1.getLat();
	double long1 = node1.getLng();
	double lat2 = node2.getLat();
	double long2 = node2.getLng();

// conversion to radiant and standard M.m format
	lat1 = (lat1 * 2.0 * PI) / 360.0 / 1000000;
	long1 = (long1 * 2.0 * PI) / 360.0 / 1000000;
	lat2 = (lat2 * 2.0 * PI) / 360.0 / 1000000;
	long2 = (long2 * 2.0 * PI) / 360.0 / 1000000;
// earth parameters
	const double a = 6378137.0; //Earth Major Axis (WGS84)
	const double b = 6356752.3142; //	Minor Axis
	const double f = (a - b) / a; // "Flattening"
	const double e = 2.0 * f - f * f; // "Eccentricity"
// calculate some aux values for first coordinate
	double beta = (a / std::sqrt(1.0 - e * std::sin(lat1) * std::sin(lat1)));
	double cos = std::cos(lat1);
	double x = beta * cos * std::cos(long1);
	double y = beta * cos * std::sin(long1);
	double z = beta * (1.0 - e) * std::sin(lat1);

// calculate some aux values for second coordinate w.r.t. first coordinate
	beta = (a / std::sqrt(1.0 - e * std::sin(lat2) * std::sin(lat2)));
	cos = std::cos(lat2);
	x -= (beta * cos * std::cos(long2));
	y -= (beta * cos * std::sin(long2));
	z -= (beta * (1 - e) * std::sin(lat2));
// calculate lowerDistanceBound in meters
	double dist = std::sqrt((x * x) + (y * y) + (z * z));
// apply some factor in order to obtain admissible lower bounds
	dist *= 7.0;
// we’re done
	return dist;
}
