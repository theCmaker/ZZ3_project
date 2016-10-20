#ifndef __INTERPETOR_HPP__
#define __INTERPETOR_HPP__

#include "units.hpp"
#include <iostream>

class Interceptor
{
	private:
		Location	_position;
		Speed		_speed;


	public:
		Interceptor();
		Interceptor(Location &, Speed);
		Interceptor(Distance, Distance, Speed);
		~Interceptor();

		// Getters
		const Location &	position() const;
			  Speed			speed() const;


		// Setters
		Interceptor &		position(const Location &);
		Interceptor &		speed(const Speed);

};

std::ostream & operator << (std::ostream &, const Interceptor &);

#endif
