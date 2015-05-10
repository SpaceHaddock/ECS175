#ifndef BEZIER_H
#define BEZIER_H

#include "vector"
#include "vector2.h"
#include "curve.h"

class Bezier : public Curve
{
public:
	Bezier();

	std::vector<Vector2> CalculatedPoints();
	void DeletePoint(int index);
	void AddPoint(int index);
};

#endif // BEZIER_H
