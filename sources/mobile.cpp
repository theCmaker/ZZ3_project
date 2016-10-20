#include "mobile.hpp"
#include <cmath>

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
const Location & Mobile::position() const
{
	return _position;
}

const Direction & Mobile::direction() const
{
	return _direction;
}

Speed Mobile::speed() const
{
	return std::sqrt(_direction._sx * _direction._sx + _direction._sy * _direction._sy);
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

std::ostream & operator<< (std::ostream & o, const Mobile & m)
{
	o << "[pos(" << m.position()._x << "," << m.position()._y << ");dir(" << m.direction()._sx << "," << m.direction()._sy << ");spd(" << m.speed() << ")]";
	return o;
}
