#include "triangle.h"
#include "algorithm"

int Triangle::sort_priority;

Triangle::Triangle()
{
}

Triangle::Triangle(int a, int b, int c)
{
	indices[0] = a;
	indices[1] = b;
	indices[2] = c;

	edges[0] = Edge(a, b);
	edges[1] = Edge(b, c);
	edges[2] = Edge(c, a);
}

Vector3 Triangle::Normal()
{
	Vector3 v1 = *edges[0].end - *edges[0].start;
	Vector3 v2 = *edges[2].end - *edges[2].start;
	return v1.CrossProduct(v2);
}


std::vector<Vector2> Triangle::MaximaOrMinima(int* view)
{
	std::vector<Vector2> result;

	Vector2 a = Vector2(edges[0].start->GetValue(view[0]), edges[0].start->GetValue(view[1]));
	Vector2 b = Vector2(edges[1].start->GetValue(view[0]), edges[1].start->GetValue(view[1]));
	Vector2 c = Vector2(edges[2].start->GetValue(view[0]), edges[2].start->GetValue(view[1]));

	if((a.y < b.y && a.y < c.y) || (a.y > b.y && a.y > c.y))
		result.push_back(a);

	if((b.y < a.y && b.y < c.y) || (b.y > a.y && b.y > c.y))
		result.push_back(b);

	if((c.y < b.y && c.y < a.y) || (c.y > b.y && c.y > a.y))
		result.push_back(c);

	return result;
}

float Triangle::SortMin() const
{
	float m = edges[0].start->GetValue(sort_priority);
	m = std::min(m, edges[1].start->GetValue(sort_priority));
	m = std::min(m, edges[2].start->GetValue(sort_priority));
	return m;
}

float Triangle::SortMax() const
{
	float m = edges[0].start->GetValue(sort_priority);
	m = std::max(m, edges[1].start->GetValue(sort_priority));
	m = std::max(m, edges[2].start->GetValue(sort_priority));
	return m;
}

float Triangle::SortAverage() const
{
	float m = edges[0].start->GetValue(sort_priority);
	m += edges[1].start->GetValue(sort_priority);
	m += edges[2].start->GetValue(sort_priority);
	return m/3;
}
