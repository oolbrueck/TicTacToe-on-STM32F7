/*
* @author Oliver Olbrück
*/

#include "touch_area.h"

TouchArea::TouchArea(){} //concept of toucharea is that every toucharea is a rectangle

TouchArea::TouchArea(int& xLowerBound, int& xUpperBound, int& yLowerBound, int& yUpperBound)
{
	this->xLowerBound = xLowerBound;
	this->xUpperBound = xUpperBound;
	this->yLowerBound = yLowerBound;
	this->yUpperBound = yUpperBound;
}

TouchArea::TouchArea(int xLowerBound, int xUpperBound, int yLowerBound, int yUpperBound)
{
	this->xLowerBound = xLowerBound;
	this->xUpperBound = xUpperBound;
	this->yLowerBound = yLowerBound;
	this->yUpperBound = yUpperBound;
}

TouchArea::~TouchArea() {}

bool TouchArea::containsPoint(int& x, int& y)
{
	return (x >= this->xLowerBound) && (x <= this->xUpperBound) && (y >= this->yLowerBound) && (y <= this->yUpperBound);
}
