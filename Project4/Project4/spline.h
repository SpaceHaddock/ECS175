#ifndef SPLINE_H
#define SPLINE_H

#include "vector"
#include "vector2.h"
#include "curve.h"

class Spline : public Curve
{
public:
	Spline();
	int degree;
	std::vector<float> knots;

	void SetDegree(int value);
	void DeletePoint(int index);
	void AddPoint(int index);
	void SetupKnots();
	int Index(int x, int y);
	std::vector<Vector2> CalculatedPoints();
};

#endif // SPLINE_H
