#ifndef GLOBAL_H
#define GLOBAL_H

#include "vector3.h"
#include "glwidget.h"
#include <vector>
#include <string>
#include "bezier.h"
#include "spline.h"

class Global
{
public:
	Global();

	static void Setup();
	static void Save();

	static std::vector<Bezier> beziers;
	static std::vector<Spline> splines;
	static std::vector<GLWidget*> screens;

	static int display_resolution;

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);

	static float Clamp(const float &clamp_me);
	static float Lerp(const float &start, const float &end, const float &position);
	static float Lerp01(const float& start, const float& end, const float& position);
	static bool GetIntersect(const Vector2&, const Vector2&, const Vector2&, const Vector2&, Vector2*);
	static bool InRange(const float& val, const float &a, const float &b);
};

#endif // GLOBAL_H
