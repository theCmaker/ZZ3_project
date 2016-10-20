#ifndef __MOBILE_H__
#define __MOBILE_H__

#include "units.hpp"
#include <iostream>

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
		const Location 	& position() const;
		const Direction & direction() const;
			  Speed		  speed() const;

		//Setters
		Mobile & position(const Location &);
		Mobile & direction(const Direction &);
};

std::ostream & operator<< (std::ostream &, const Mobile &);
#endif
