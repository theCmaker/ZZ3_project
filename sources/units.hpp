#ifndef __UNITS_HPP__
#define __UNITS_HPP__

#include <iostream>

typedef double Speed;
typedef double Time;
typedef double Distance;

//******************************************************
// Location
//******************************************************

typedef struct _Location
{
	Distance _x;
	Distance _y;

	_Location(Distance x = 0., Distance y = 0.);

	bool equals(const struct _Location & other, Distance epsilon = 0.) const;

} Location;

std::ostream & operator<< (std::ostream &, const Location &);

bool operator== (const struct _Location &, const struct _Location &);


//******************************************************
// Direction
//******************************************************

typedef struct _Direction
{
	Speed _sx;
	Speed _sy;

	_Direction(Speed sx = 0., Speed sy = 0.);
} Direction;

std::ostream & operator<< (std::ostream &, const Direction &);

bool operator== (const struct _Direction &, const struct _Direction &);

#endif
