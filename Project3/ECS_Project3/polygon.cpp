#include "polygon.h"
#include "math.h"
#include "vector3.h"
#include "matrix.h"

Polygon::Polygon()
{
}

void Polygon::Translate(Vector3 translate_amount)
{
	if(vertices.empty()) return;

	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		Matrix<4, 4> a = {1, 0, 0, translate_amount.x,
						  0, 1, 0, translate_amount.y,
						  0, 0, 1, translate_amount.z,
						  0, 0, 0, 1};
		vertices[i] = MultiplyByMatrix(vertices[i], a);
	}
}

void Polygon::Scale(Vector3 scale_amount)
{
	if(vertices.empty()) return;

	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		Matrix<4, 4> a = {scale_amount.x, 0, 0, 0,
						  0, scale_amount.y, 0, 0,
						  0, 0, scale_amount.z, 0,
						  0, 0, 0, 1};
		vertices[i] = MultiplyByMatrix(vertices[i], a);
	}
}

void Polygon::ScaleCenter(Vector3 scale_amount)
{
	if(vertices.empty()) return;
	Vector3 centroid = Centroid();
	Translate(-centroid);
	Scale(scale_amount);
	Translate(centroid);
}

Vector3 Polygon::Centroid()
{
	Vector3 result;
	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		result = result + vertices[i];
	}
	result.x /= vertices.size();
	result.y /= vertices.size();
	result.z /= vertices.size();

	return result;
}

void Polygon::AddTriangle(int a, int b, int c)
{
	triangles.push_back(Triangle(a, b, c));
}

void Polygon::CalculateTriangles()
{
	for(int i=0; (unsigned) i<triangles.size(); i++)
	{
		for(int j=0; (unsigned) j<3; j++)
		{
			triangles[i].edges[j].start = &vertices[triangles[i].edges[j].start_index];
			triangles[i].edges[j].end = &vertices[triangles[i].edges[j].end_index];
		}
	}
}

/* Used for edges, now using triangles
void Polygon::AddEdge(int index_a, int index_b)
{
	Edge e;
	e.start_index = index_a;
	e.end_index = index_b;
	edges.push_back(e);
}

void Polygon::CalculateEdges()
{
	for(int i=0; (unsigned) i<edges.size(); i++)
	{
		edges[i].start = &vertices[edges[i].start_index];
		edges[i].end = &vertices[edges[i].end_index];
	}
}
*/

void Polygon::RotateXAxis(float angle)
{
	angle *= 3.14159/180;
	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		Matrix<4, 4> a = {1, 0, 0, 0,
						  0, cos(angle), -sin(angle), 0,
						  0, sin(angle), cos(angle), 0,
						  0, 0, 0, 1};
		vertices[i] = MultiplyByMatrix(vertices[i], a);
	}
}

void Polygon::RotateYAxis(float angle)
{
	angle *= 3.14159/180;
	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		Matrix<4, 4> a = {cos(angle), 0, sin(angle), 0,
						  0, 1, 0, 0,
						  -sin(angle), 0, cos(angle), 0,
						  0, 0, 0, 1};
		vertices[i] = MultiplyByMatrix(vertices[i], a);
	}
}

void Polygon::RotateZAxis(float angle)
{
	angle *= 3.14159/180;
	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		Matrix<4, 4> a = {cos(angle), -sin(angle), 0, 0,
						  sin(angle), cos(angle), 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1};
		vertices[i] = MultiplyByMatrix(vertices[i], a);
	}
}


void Polygon::RotateArbitrary(float angle, Vector3 pointa, Vector3 pointb)
{
	//Setup values
	Vector3 v;
	v = pointa - pointb;
	v = v.Normalized();
	float d = sqrt(pow(v.y, 2) + pow(v.z, 2));

	//Step 1
	Translate(-pointa);

	//Step 2
	Matrix<4, 4> a = {1, 0, 0, 0,
					  0, v.z/d, -v.y/d, 0,
					  0, v.y/d, v.z/d, 0,
					  0, 0, 0, 1};

	Matrix<4, 4> b = {d, 0, -v.x, 0,
					  0, 1, 0, 0,
					  v.x, 0, d, 0,
					  0, 0, 0, 1};
	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		vertices[i] = MultiplyByMatrix(vertices[i], a);
		vertices[i] = MultiplyByMatrix(vertices[i], b);
	}

	//Step 3
	RotateZAxis(angle);

	//Step 4
	Matrix<4, 4> c = {1, 0, 0, 0,
					  0, v.z/d, v.y/d, 0,
					  0, -v.y/d, v.z/d, 0,
					  0, 0, 0, 1};

	Matrix<4, 4> e = {d, 0, v.x, 0,
					  0, 1, 0, 0,
					  -v.x, 0, d, 0,
					  0, 0, 0, 1};
	for(int i=0; (unsigned) i<vertices.size(); i++)
	{
		vertices[i] = MultiplyByMatrix(vertices[i], e);
		vertices[i] = MultiplyByMatrix(vertices[i], c);
	}

	//Step 5
	Translate(pointa);
}

Vector3 Polygon::MultiplyByMatrix(Vector3 v, Matrix<4,4> m)
{
	Matrix<4, 1> a = {v.x, v.y, v.z, 1};
	Matrix<4, 1> r = {0, 0, 0, 0};
	a.mult(m, a, r);
	return Vector3(r.m[0][0], r.m[1][0], r.m[2][0]);
}
