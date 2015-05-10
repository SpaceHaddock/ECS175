#include "bezier.h"
#include "global.h"

Bezier::Bezier()
{
	is_spline = false;
}

std::vector<Vector2> Bezier::CalculatedPoints()
{
	std::vector<Vector2> result;
	int n = polygon_points.size();

	//Calculate array values
	for(int q=0; q<=Global::display_resolution; q++)
	{
		Vector2* arr = new Vector2[(int) pow(n, 2)];

		//Populate array
		for(int i=0; i<n; i++)
			arr[i] = polygon_points[i];

		for(int i=1; i<n; i++)
		{
			for(int j=0; j<n-i; j++)
			{
				int index = i*n + j;
				arr[index] = Vector2::Lerp(arr[index - n], arr[index - n + 1], q * ((float) 1)/Global::display_resolution);
			}
		}
		result.push_back(arr[n*(n-1)]);
	}
	return result;
}

void Bezier::DeletePoint(int index)
{
	polygon_points.erase(polygon_points.begin() + index);
}

void Bezier::AddPoint(int index)
{
	polygon_points.insert(polygon_points.begin() + index + 1, Vector2(.5, .5));
}
