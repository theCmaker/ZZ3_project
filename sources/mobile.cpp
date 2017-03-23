#include "mobile.hpp"
#include <cmath>

Mobile::Mobile(unsigned id) :
	_id(id),
	_position(0.,0.),
	_direction(0.,0.)
{}

Mobile::Mobile(Location & p, Direction & d, unsigned id) :
	_id(id),
	_position(p),
	_direction(d)
{}

Mobile::Mobile(Distance x, Distance y, Distance dx, Distance dy, unsigned id) :
	_id(id),
	_position(x,y),
	_direction(dx,dy)
{}

Mobile::~Mobile() {}

//Getters
const unsigned & Mobile::id() const
{
	return _id;
}

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
	o << "[" << m.id() << " pos" << m.position() << ";dir" << m.direction() << ";spd(" << m.speed() << ")]";
	return o;
}
 
