#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "vector3.h"
#include "edge.h"
#include "matrix.h"
#include <string>

class Polygon
{
public:
	Polygon();

	std::string name;
	std::vector<Vector3> vertices;
	std::vector<Edge> edges;

	void AddEdge(int index_a, int index_b);

	void Translate(Vector3);
	void Scale(Vector3);

	void RotateXAxis(float amount);
	void RotateYAxis(float amount);
	void RotateZAxis(float amount);

	void RotateArbitrary(float, Vector3 pointa, Vector3 pointb);
	void ScaleCenter(Vector3);

	Vector3 MultiplyByMatrix(Vector3 v, Matrix<4,4> m);
	void CalculateEdges();
	Vector3 Centroid();
};

#endif // POLYGON_H
