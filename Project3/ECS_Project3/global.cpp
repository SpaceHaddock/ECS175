#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "stdlib.h"
#include "global.h"

using namespace std;

//Static variable declaration
std::vector<Polygon> Global::polygons;
std::vector<Vector3> Global::lines;
std::vector<GLWidget*> Global::screens;
float Global::ambient_coefficient = .1;
float Global::diffuse_coefficient = .1;
float Global::specular_coefficient = .1;
float Global::ambient_intensity = .1;
float Global::lightsource_intensity = .1;
float Global::lightsource_distance = 100;
Vector3 Global::lightsource_position = Vector3(1, 1, 1);
int Global::phong_constant = 3;
int Global::half_tone_size = 1;
float Global::color_r = 1;
float Global::color_g = 0;
float Global::color_b = 0;

Global::Global(){}

void Global::Setup()
{
	//Read in polygons
	string line;
	ifstream myfile("input.txt");
	if(myfile.is_open())
	{
		bool on_polygon = false;
		while(getline(myfile, line))
		{
			if(line[0] == '\\')
			{
				on_polygon = !on_polygon;
				if(on_polygon)
				{
					polygons.push_back(Polygon());
					polygons.back().name = line.substr(1, line.length() - 1);
				}
			}
			else
			{
				if(on_polygon) //read vertex
				{
					vector<string> result = split(line, ' ');

					polygons.back().vertices.push_back(
								Vector3(
									atof(result[0].c_str()),
								atof(result[1].c_str()),
							atof(result[2].c_str())
							));
				}
				else //read edge
				{
					if(line != "")
					{
						vector<string> result = split(line, ' ');
						int a, b, c;
						std::istringstream(result[0]) >> a;
						std::istringstream(result[1]) >> b;
						std::istringstream(result[2]) >> c;
						polygons.back().AddTriangle(a, b, c);
					}
				}
			}
		}
		myfile.close();
	}
}

void Global::Save()
{
	ofstream myfile ("input.txt");
	if (myfile.is_open())
	{
		for(int i=0; (unsigned) i<polygons.size(); i++)
		{
			Polygon* poly = &polygons[i];
			myfile << "\\" << poly->name << "\n";
			for(int j=0; (unsigned) j<poly->vertices.size(); j++)
			{
				myfile << poly->vertices[j].x << " " << poly->vertices[j].y << " " << poly->vertices[j].z << "\n";
			}
			myfile << "\\Edges\n";
			for(int j=0; (unsigned) j<poly->triangles.size(); j++)
			{
				myfile << poly->triangles[j].indices[0] << " " << poly->triangles[j].indices[1] << " " << poly->triangles[j].indices[2] << "\n";
			}
		}
	}
	myfile.close();
}

vector<string> &Global::split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> Global::split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

//Useful functions
float Global::Clamp(const float &clamp_me)
{
	float z = 0;
	float o = 1;
	return max(z, min(o, clamp_me));
}

float Global::Lerp(const float &start, const float &end, const float &position)
{
	float dif = end - start;
	return dif * Clamp(position) + start;
}

float Global::Lerp01(const float &start, const float &end, const float &position)
{
	float result = position - start;
	result /= end - start;
	return Clamp(result);
}

//Returns bool for whether or not the two lines intersect, saves point of intersection in result vector
bool Global::GetIntersect(const Vector2 &a_start, const Vector2 &a_end, const Vector2 &b_start, const Vector2 &b_end, Vector2* result)
{
	Vector2 direction1(a_start.x - a_end.x, a_start.y - a_end.y);
	Vector2 direction2(b_start.x - b_end.x, b_start.y - b_end.y);
	float dotPerp = (direction1.x * direction2.y) - (direction1.y * direction2.x);

	// If it's 0, it means the lines are parallel so have infinite intersection points
	if (!(dotPerp > .0001 || dotPerp < -.0001))
	{
		return false;
	}

	Vector2 c(b_start.x - a_start.x, b_start.y - a_start.y);
	float t = (c.x * direction2.y - c.y * direction2.x) / dotPerp;

	//	Return the intersection point
	result->x = a_start.x + (t * direction1.x);
	result->y = a_start.y + (t * direction1.y);


	return (InRange(result->x, a_start.x, a_end.x) && InRange(result->y, a_start.y, a_end.y));
}

bool Global::InRange(const float &val, const float &a, const float &b)
{
	return (val <= a || val <= b) && (val >= a || val >= b);
}
