#include "depot.hpp"

Depot::Depot()
{
	_position._x = 0.;
	_position._y = 0.;
}

Depot::Depot(Distance x, Distance y)
{
	_position._x = x;
	_position._y = y;
}

Depot::Depot(Location & l, Interceptor & i)
{
	_position = l;
	_interceptors.push_back(i);
}

Depot::Depot(Location & l)
{
	_position = l;
}

Depot::~Depot() {}

const Location & Depot::position() const
{
	return _position;
}

Depot & Depot::position(const Location & l)
{
	_position = l;
	return *this;
}

std::ostream & operator<< (std::ostream & o, const Depot & d)
{
	o << "[pos(" << d.position()._x << "," << d.position()._y << ")]";
	return o;
}