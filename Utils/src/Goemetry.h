#pragma once

#include <string>
#include <Windows.h>

class PointF
{
public:
	PointF(float x = 0, float y = 0);
	PointF(POINT point);
	void Display(std::string name);

	float x;
	float y;
	
};

class Rect
{
public:
	Rect(int left = 0, int top = 0, int right = 0, int bottom = 0);
	Rect(RECT rect);

	void Display(std::string name);
	int GetWidth();
	void SetWidth(int width);
	int GetHeight();
	void SetHeight(int height);

	int left;
	int top;
	int right;
	int bottom;
};