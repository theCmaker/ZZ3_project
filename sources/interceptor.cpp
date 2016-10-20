#include "interceptor.hpp"

Interceptor::Interceptor()
{
	_position._x = 0.;
	_position._y = 0.;
	_speed = 0.;
}

Interceptor::Interceptor(Location & l, Speed s)
{
	_position = l;
	_speed = s;
}

Interceptor::Interceptor(Distance dx, Distance dy, Speed s)
{
	_position._x = dx;
	_position._y = dy;
	_speed = s;
}

Interceptor::~Interceptor() {}

// Getters
const Location & Interceptor::position() const
{
	return _position;
}

Speed Interceptor::speed() const
{
	return _speed;
}

// Setters
Interceptor & Interceptor::position(const Location & l)
{
	_position = l;
	return *this;
}

Interceptor & Interceptor::speed(const Speed s)
{
	_speed = s;
	return *this;
}

std::ostream & operator << (std::ostream & o, const Interceptor & i) {
	o << "[pos(" << i.position()._x << "," << i.position()._y << ");spd(" << i.speed() << ")]";
	return o;
}
