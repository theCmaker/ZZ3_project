#ifndef __MOBILE_H__
#define __MOBILE_H__

#include "units.hpp"
#include <iostream>

class Mobile
{
	private:
		unsigned 	_id;
		Location	_position;
		Direction	_direction;

	public:
		Mobile(unsigned = 0);
		Mobile(Location &, Direction &, unsigned = 0);
		Mobile(Distance, Distance, Distance, Distance, unsigned = 0);
		~Mobile();

		//Getters
		const unsigned & id() const;
		const Location 	& position() const;
		Location position(Time) const;
		const Direction & direction() const;
			  Speed		  speed() const;

		//Setters
		Mobile & position(const Location &);
		Mobile & direction(const Direction &);
};

std::ostream & operator<< (std::ostream &, const Mobile &);
#endif
