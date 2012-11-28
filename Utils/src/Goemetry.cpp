#include "StdAfx.h"
#include "Goemetry.h"
#include <iostream>

using namespace std;

/////////////////////////////////////////////////////////
// PointF
/////////////////////////////////////////////////////////
PointF::PointF(float _x, float _y) : x(_x), y(_y) 
{
}

PointF::PointF(POINT point) :  x((float)point.x), y((float)point.y)
{
}

void PointF::Display(string _name)
{
	cout << "Displaying " << _name.c_str() << endl;
	cout << "(" << x << "; " << y	<< ")" << endl << endl;
}

/////////////////////////////////////////////////////////
// Rect
/////////////////////////////////////////////////////////
Rect::Rect(int _left, int _top, int _right, int _bottom) : left(_left), top(_top), right(_right), bottom(_bottom)
{
}

Rect::Rect(RECT rect) : left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom)
{
}

void Rect::Display(string _name)
{
	cout << "Displaying " << _name.c_str() << endl;
	cout << " - Position : "	<< left	<< ", "	<< top << ", ";
	cout << right << ", "	<< bottom << endl;
	cout << " - Dimensions : "	<< GetWidth() << "x" << GetHeight() << endl << endl;
}

int Rect::GetWidth()
{
	return right - left;
}

void Rect::SetWidth(int _width)
{
	right = left + _width;
}

int Rect::GetHeight()
{
	return bottom - top;
}

void Rect::SetHeight(int _height)
{
	bottom = top + _height;
}
