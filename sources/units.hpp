#ifndef __UNITS_HPP__
#define __UNITS_HPP__

typedef double Speed;
typedef double Time;
typedef double Distance;

typedef struct _Location
{
	Distance _x;
	Distance _y;
} Location;

typedef struct _Direction
{
	Speed _sx;
	Speed _sy;
} Direction;


#endif
