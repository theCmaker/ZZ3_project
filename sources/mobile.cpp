#include "mobile.hpp"

Mobile::Mobile()
{
	_position._x = 0.;
	_position._y = 0.;
	_direction._sx = 0.;
	_direction._sy = 0.;
}

Mobile::Mobile(Location & p, Direction & d) :
	_position(p),
	_direction(d)
{}

Mobile::Mobile(Distance x, Distance y, Distance dx, Distance dy)
{
	_position._x = x;
	_position._y = y;
	_direction._sx = dx;
	_direction._sy = dy;
}

Mobile::~Mobile() {}

//Getters
const Location & Mobile::position()
{
	return _position;
}

const Direction & Mobile::direction()
{
	return _direction;
}

//Setters
Mobile & Mobile::position(const Location & p)
{
	_position = p;
	return *this;
}

Mobile & Mobile::direction(const Direction & d)
{
	_direction = d;
	return *this;
}
