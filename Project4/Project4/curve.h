#ifndef CURVE_H
#define CURVE_H

#include "vector"
#include "vector2.h"
#include "string"

class Curve
{
public:
	bool is_spline;
	std::string name;
	std::vector<Vector2> polygon_points;

	Curve();
	virtual std::vector<Vector2> CalculatedPoints() = 0;
};

#endif // CURVE_H
