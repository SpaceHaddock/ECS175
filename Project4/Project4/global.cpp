#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "stdlib.h"
#include "global.h"

using namespace std;

//Static variable declaration
std::vector<Bezier> Global::beziers;
std::vector<Spline> Global::splines;
std::vector<GLWidget*> Global::screens;
int Global::display_resolution = 10;

Global::Global(){}

void Global::Setup()
{
	//Read in curves
	string line;
	ifstream myfile("input.txt");
	if(myfile.is_open())
	{
		bool is_spline = false;
		while(getline(myfile, line))
		{
			if(line == "Bezier")
			{
				is_spline = false;
				beziers.push_back(Bezier());

				string temp;
				getline(myfile, temp);
				beziers.back().name = temp;
			}
			else if (line == "Spline")
			{
				is_spline = true;
				splines.push_back(Spline());

				string temp;
				getline(myfile, temp);
				splines.back().name = temp;

				getline(myfile, temp);
				splines.back().degree = atoi(temp.c_str());
			}
			else
			{
				vector<string> result = split(line, ' ');

				Vector2 push = Vector2(atof(result[0].c_str()), atof(result[1].c_str()));
				if(is_spline)
					splines.back().polygon_points.push_back(push);
				else
					beziers.back().polygon_points.push_back(push);
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
		if(!beziers.empty())
		{
			for(int i=0; (unsigned) i<beziers.size(); i++)
			{
				Bezier* b = &beziers[i];
				myfile << "Bezier\n";
				myfile << b->name << "\n";

				for(int j=0; (unsigned) j<b->polygon_points.size(); j++)
					myfile << b->polygon_points[j].x << " " << b->polygon_points[j].y << "\n";
			}
		}

		if(!splines.empty())
		{
			for(int i=0; (unsigned) i<splines.size(); i++)
			{
				Spline* b = &splines[i];
				myfile << "Spline\n";
				myfile << b->name << "\n";
				myfile << b->degree << "\n";

				for(int j=0; (unsigned) j<b->polygon_points.size(); j++)
					myfile << b->polygon_points[j].x << " " << b->polygon_points[j].y << "\n";
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
