#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "edge.h"
#include "vector2.h"
#include "math.h"

class Triangle
{
public:
	Triangle();
	Triangle(int a, int b, int c);

	int indices [3];
	Edge edges [3];

	Vector3 Normal();
	std::vector<Vector2> MaximaOrMinima(int* view);

	static int sort_priority;
	float SortMin() const;
	float SortMax() const;
	float SortAverage() const;
	bool operator < (const Triangle& other) const
	{
		return (SortAverage() < other.SortAverage());
	}
};

#endif // TRIANGLE_H
