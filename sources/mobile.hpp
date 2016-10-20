#ifndef __MOBILE_H__
#define __MOBILE_H__

#include "units.hpp"

class Mobile
{
	private:
		Location	_position;
		Direction	_direction;

	public:
		Mobile();
		Mobile(Location &, Direction &);
		Mobile(Distance, Distance, Distance, Distance);
		~Mobile();

		//Getters
		const Location 	& position();
		const Direction & direction();

		//Setters
		Mobile & position(const Location &);
		Mobile & direction(const Direction &);
};
#endif
