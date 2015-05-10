#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include "vector3.h"
//#include "edge.h"
#include "triangle.h"
#include "matrix.h"
#include "color.h"

class Polygon
{
public:
	Polygon();

	std::string name;
	std::vector<Vector3> vertices;
	std::vector<Color> vertex_colors;

	std::vector<Triangle> triangles;
	void AddTriangle(int, int, int);
	void CalculateTriangles();
	//std::vector<Edge> edges;
	//void AddEdge(int index_a, int index_b);
	//void CalculateEdges();

	void Translate(Vector3);
	void Scale(Vector3);

	void RotateXAxis(float amount);
	void RotateYAxis(float amount);
	void RotateZAxis(float amount);

	void RotateArbitrary(float, Vector3 pointa, Vector3 pointb);
	void ScaleCenter(Vector3);

	Vector3 MultiplyByMatrix(Vector3 v, Matrix<4,4> m);
	Vector3 Centroid();
};

#endif // POLYGON_H
