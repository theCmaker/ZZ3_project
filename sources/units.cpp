#include "units.hpp"

//******************************************************
// Location
//******************************************************

Location::Location(Distance x, Distance y) :
	_x(x),
	_y(y)
{}

bool Location::equals(const Location & other, Distance epsilon) const
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

bool operator== (const Location & l1, const Location & l2)
{
	bool res = false;
	if(l1._x == l2._x && l1._y == l2._y)
	{
		res = true;
	}
	return res;
}


//******************************************************
// Direction
//******************************************************

Direction::Direction(Speed sx, Speed sy) :
	_sx(sx),
	_sy(sy)
{}

std::ostream & operator<< (std::ostream & o, const Direction & d)
{
	o << "(" << d._sx << "," << d._sy << ")";
	return o;
}

bool operator== (const Direction & d1, const Direction & d2)
{
	bool res = false;
	if(d1._sx == d2._sx && d1._sy == d2._sy)
	{
		res = true;
	}
	return res;
}
