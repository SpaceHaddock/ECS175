#include "pixelbuffer.h"
#include "algorithm"
#include "global.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

PixelBuffer::PixelBuffer()
{
	srand (time(NULL));
}

PixelBuffer::~PixelBuffer()
{
	delete[] pixels;
}

int PixelBuffer::height() {return pixel_height;}
int PixelBuffer::width() {return pixel_width;}

//Colors one pixel, results don't show up until next paintGL is called
void PixelBuffer::ColorPixel(const int &x, const int &y)
{
	ColorPixel(x, y, Color(0, 0, 0));
}

void PixelBuffer::ColorPixel(const int &x, const int &y, const Color &color)
{
	if(x > width() - 1 || x < 0 || y > height() - 1 || y < 0)
		return;

	int first_index = PixelFirstIndex(x, y);
	Color use_color = color;
	pixels[first_index] = use_color.r;
	pixels[first_index + 1] = use_color.g;
	pixels[first_index + 2] = use_color.b;
}

int PixelBuffer::PixelFirstIndex(const int &x, const int &y)
{
	return (x + y * pixel_width) * 3;
}

//Line drawing algorithms, copied from the book
void PixelBuffer::LineDDA(const int &x_start, const int &y_start, const int &x_end, const int &y_end, const Color &start_color, const Color &end_color)
{
	int dx = x_end - x_start;
	int dy = y_end - y_start;

	float x_pos = x_start;
	float y_pos = y_start;

	int steps = abs(dx) > abs(dy) ?
				abs(dx) : abs(dy);

	float x_increment = dx / (float) steps;
	float y_increment = dy / (float) steps;

	for(int i=0; i < steps + 1; i++)
	{
		float amt = ((float)i) / (steps + 1);
		Color set_color = Color::Lerp(start_color, end_color, amt);
		ColorPixel(round(x_pos), round(y_pos), set_color);
		x_pos += x_increment;
		y_pos += y_increment;
	}
}

//Converts worldspace location to pixelspace location by normalizing
Vector2 PixelBuffer::ConvertToPixel(const Vector2 *input)
{
	Vector2 result;

	//Normalize world coordinates
	float dx = max_window.x - min_window.x;
	float dy = max_window.y - min_window.y;

	float max_d = max(dx, dy);
	float wx = dx/max_d;
	float wy = dy/max_d;

	result.x = (input->x - min_window.x) / max_d;
	result.y = (input->y - min_window.y) / max_d;

	//Convert into pixel coordinates using "fit"
	float s_max_d = max(width(), height());
	float sx = (width() - 1) / s_max_d;
	float sy = (height() - 1) / s_max_d;
	float s_min = min(sx/wx, sy/wy);

	result.x *= s_min * s_max_d;
	result.y *= s_min * s_max_d;

	//Round
	result.x = round(result.x);
	result.y = round(result.y);

	return result;
}
