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
const Location & Interceptor::position()
{
	return _position;
}

const Speed Interceptor::speed()
{
	return _speed;
}
			
// Setters
Interceptor & Interceptor::position(const Location & l)
{
	_position = l;
}

Interceptor & Interceptor::speed(const Speed s)
{
	_speed = s;
}
