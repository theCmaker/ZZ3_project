#include "depot.hpp"

Depot::Depot()
{
	_position._x = 0.;
	_position._y = 0.;
}

Depot::Depot(Location & l, Interceptor & i)
{
	_position = l;
	_interceptors.push_back(i);
}

Depot::~Depot() {}
