#include "units.hpp"

//******************************************************
// Location
//******************************************************

Location::_Location(Distance x, Distance y) :
	_x(x),
	_y(y)
{}

bool Location::equals(const struct _Location & other, Distance epsilon) const
{
	return (other._x - epsilon) <= _x
		&& (other._x + epsilon) >= _x
		&& (other._y - epsilon) <= _y
		&& (other._y + epsilon) >= _y;
}

std::ostream & operator<< (std::ostream & o, const Location & l)
{
	o << "(" << l._x << "," << l._y << ")";
	return o;
}


//******************************************************
// Direction
//******************************************************

Direction::_Direction(Speed sx, Speed sy) :
	_sx(sx),
	_sy(sy)
{}

std::ostream & operator<< (std::ostream & o, const Direction & d)
{
	o << "(" << d._sx << "," << d._sy << ")";
	return o;
}
