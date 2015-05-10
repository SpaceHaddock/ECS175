#ifndef GLOBAL_H
#define GLOBAL_H

#include "polygon.h"
#include "vector3.h"
#include "glwidget.h"
#include <vector>
#include <string>

class Global
{
public:
	Global();

	static void Setup();
	static void Save();

	static std::vector<Polygon> polygons;
	static std::vector<Vector3> lines;
	static std::vector<GLWidget*> screens;

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);
};

#endif // GLOBAL_H
