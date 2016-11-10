#include "depot.hpp"

Depot::Depot(unsigned id) :
	_id(id),
	_position(0.,0.)
{}

Depot::Depot(Distance x, Distance y, unsigned id) :
	_id(id),
	_position(x,y)
{}

Depot::Depot(Location & l, unsigned id) :
	_id(id),
	_position(l)
{}

Depot::~Depot() {}

const unsigned & Depot::id() const
{
	return _id;
}

const Location & Depot::position() const
{
	return _position;
}

Depot & Depot::position(const Location & l)
{
	_position = l;
	return *this;
}

Depot & Depot::addInterceptor(const Interceptor & i) {
	_interceptors.push_back(&i);
	return *this;
}

std::ostream & operator<< (std::ostream & o, const Depot & d)
{
	o << "[" << d.id() << " pos(" << d.position()._x << "," << d.position()._y << ")]";
	return o;
}
