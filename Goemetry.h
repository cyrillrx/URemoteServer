#pragma once

#include <string>
#include <Windows.h>

struct PointF
{
public:
	PointF(float _x = 0, float _y = 0);
	PointF(POINT _point);
	void Display(std::string _name);

	float x;
	float y;
	
};

struct Rect
{
	int GetWidth();
	void SetWidth(int _width);
	int GetHeight();
	void SetHeight(int _height);

public:
	Rect(int _left = 0, int _top = 0, int _right = 0, int _bottom = 0);
	Rect(RECT _rect);
	void Display(std::string _name);

	int left;
	int top;
	int right;
	int bottom;

   __declspec(property(get = GetWidth, put = SetWidth)) int width;
   __declspec(property(get = GetHeight, put = SetHeight)) int height;
	
};

