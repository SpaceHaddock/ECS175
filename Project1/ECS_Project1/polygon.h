#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "vector2.h"
#include <string>

class Polygon
{
public:
	Polygon();

	std::string name;
	std::vector<Vector2> vertices;

	void Translate(Vector2);
	void Scale(Vector2);
	void Rotate(float);

	void ScaleCenter(Vector2);
	void RotateCenter(float);

	Vector2 Centroid();
};

#endif // POLYGON_H
