#include "StdAfx.h"
#include "Goemetry.h"
#include <iostream>

/////////////////////////////////////////////////////////
// PointF
/////////////////////////////////////////////////////////
PointF::PointF(float _x, float _y)
{
	x = _x;
	y = _y;
}

PointF::PointF(POINT _point)
{
	x = _point.x;
	y = _point.y;
}

void PointF::Display(std::string _name)
{
	std::cout << "Displaying " << _name.c_str() << std::endl;
	std::cout << "(" << x << "; " << y	<< ")" << std::endl << std::endl;
}

/////////////////////////////////////////////////////////
// Rect
/////////////////////////////////////////////////////////
Rect::Rect(int _left, int _top, int _right, int _bottom)
{
	left	= _left;
	top	= _top;
	right	= _right;
	bottom = _bottom;
}

Rect::Rect(RECT _rect)
{
	left	= _rect.left;
	top	= _rect.top;
	right	= _rect.right;
	bottom = _rect.bottom;
}

void Rect::Display(std::string _name)
{
	std::cout << "Displaying " << _name.c_str() << std::endl;
	std::cout << " - Position : "	<< left	<< ", "	<< top << ", ";
	std::cout << right << ", "	<< bottom << std::endl;
	std::cout << " - Dimensions : "	<< width << "x"	<< height << std::endl << std::endl;
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
