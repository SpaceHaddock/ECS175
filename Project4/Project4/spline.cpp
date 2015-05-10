#include "spline.h"
#include "global.h"

Spline::Spline()
{
	is_spline = true;
	degree = 3;
}

void Spline::SetDegree(int value)
{
	degree = value;
	SetupKnots();
}

void Spline::SetupKnots()
{
	knots.clear();

	for(int i=0; i < degree; i++)
		knots.push_back(0);

	int num_segments = polygon_points.size() - degree;
	if(num_segments == 0)
		knots.push_back(0);
	else
	{
		for(int i=0; i <= num_segments; i++)
			knots.push_back(((float) i)/num_segments);
	}

	for(int i=0; i < degree; i++)
		knots.push_back(1);
}

std::vector<Vector2> Spline::CalculatedPoints()
{
	if(knots.empty()) SetupKnots();

	std::vector<Vector2> result;

	int n = degree;
	for(int q=0; q<Global::display_resolution; q++)
	{
		float x = ((float) q)/Global::display_resolution;

		//Find L value
		int l = -1;
		for(int i=0; (unsigned) i<knots.size() - 1; i++)
		{
			if(knots[i] <= x && x < knots[i+1])
			{
				l = i;
				break;
			}
		}

		//Populate the array
		std::vector<Vector2> arr;
		arr.resize((int) ((degree + 1) * (polygon_points.size())) + 1);
		for(int i=l-n; i <= l; i++)
			arr[i] = polygon_points[i];

		//Calculate the point
		for(int k=1; k<=n; k++)
		{
			for(int i=l-n+k; i<=l; i++)
			{
				float a_top = x - knots[i];
				float a_bot = knots[i+n+1-k] - knots[i];

				float alpha = 0;
				if(a_bot > 0)
					alpha = a_top/a_bot;

				arr[Index(k, i)] = arr[Index(k-1, i-1)] * (1 - alpha) + arr[Index(k-1, i)] * alpha;
			}
		}

		//Add value to result
		result.push_back(arr[Index(n, l)]);
	}

	result.push_back(polygon_points.back()); //add last point since we won't calculate at 1

	return result;
}

int Spline::Index(int x, int y)
{
	return x * polygon_points.size() + y;
}

void Spline::DeletePoint(int index)
{
	polygon_points.erase(polygon_points.begin() + index);
	SetupKnots();
}

void Spline::AddPoint(int index)
{
	polygon_points.insert(polygon_points.begin() + index + 1, Vector2(.5, .5));
	SetupKnots();
}
