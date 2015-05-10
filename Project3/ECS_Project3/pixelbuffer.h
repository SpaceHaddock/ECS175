#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include "vector2.h"
#include "color.h"

class PixelBuffer
{
public:
	PixelBuffer();
	~PixelBuffer();

	int pixel_height;
	int pixel_width;

	float* pixels;
	//int half_tone_size;

	Vector2 min_window;
	Vector2 max_window;
	Vector2 intensity_extremes;

	void ColorPixel(const int &x, const int &y);
	void ColorPixel(const int &x, const int &y, const Color&);
	void LineDDA(const int &xa, const int &ya, const int &xb, const int &yb, const Color&, const Color&);
	Vector2 ConvertToPixel(const Vector2* input);

	int PixelFirstIndex(const int &x, const int &y);

	int height();
	int width();
};

#endif // PIXELBUFFER_H
