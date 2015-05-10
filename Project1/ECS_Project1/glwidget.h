#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <vector>
#include "polygon.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    static GLWidget* screen;

    void initializeGL();
	void BuildWindow();
	void DrawPolygons();
    void paintGL();
    void resizeGL(int w, int h);
	void ColorPixel(int x, int y);

	float* pixels; //represents the colors on the screen
    int width;
	int height;
	float x_min;
	float x_max;
	float y_min;
	float y_max;

	std::vector<Polygon> polygons; //probably change to a vector

	//Algorithms
	void LineDDA(int xa, int ya, int xb, int yb);
	void LineBresenham(int xa, int ya, int xb, int yb);
	Vector2 ConvertToPixel(Vector2* input);
	bool GetIntersect(Vector2, Vector2, Vector2, Vector2, Vector2*);
	bool InRange(float val, float a, float b);

	//All this stuff is for Cohen-Sutherland
	unsigned char encode(Vector2 pt);
	void swapPts(Vector2* p1, Vector2* p2);
	void swapCodes(unsigned char* c1, unsigned char* c2);
	void clipLine(Vector2 p1, Vector2 p2);
};

#endif // GLWIDGET_H
