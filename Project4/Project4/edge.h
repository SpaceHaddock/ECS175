#ifndef EDGE_H
#define EDGE_H

#include "vector3.h"

class Edge
{
public:
	Edge();
	Edge(int in_start, int in_end);

	int start_index;
	int end_index;
	Vector3* start;
	Vector3* end;
};

#endif // EDGE_H
