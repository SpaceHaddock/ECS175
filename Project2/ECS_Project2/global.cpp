#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "stdlib.h"
#include "global.h"

using namespace std;

std::vector<Polygon> Global::polygons;
std::vector<Vector3> Global::lines;
std::vector<GLWidget*> Global::screens;

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
					vector<string> result = split(line, ' ');
					int a, b;
					std::istringstream(result[0]) >> a;
					std::istringstream(result[1]) >> b;
					polygons.back().AddEdge(a, b);
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
			for(int j=0; (unsigned) j<poly->edges.size(); j++)
			{
				myfile << poly->edges[j].start_index << " " << poly->edges[j].end_index << "\n";
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
