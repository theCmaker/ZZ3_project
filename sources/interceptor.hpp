#ifndef __INTERPETOR_HPP__
#define __INTERPETOR_HPP__

#include "units.hpp"
#include "mobile.hpp"
#include <iostream>

class Depot;

class Interceptor
{
	private:
		unsigned	_id;
		Location	_position;
		Speed		_speed;
		const Depot *_depot;

		static double computeAlpha(double, double, double);


	public:
		Interceptor(unsigned = 0);
		Interceptor(Location &, Speed, unsigned = 0);
		Interceptor(Distance, Distance, Speed, unsigned = 0);
		Interceptor(const Depot &, Speed, unsigned = 0);
		~Interceptor();

		// Getters
		const unsigned & id() 		const;
		const Location & position()	const;
			  Speed		 speed()	const;
		const Depot	   * depot()	const;


		// Setters
		Interceptor &	position(const Location &);
		Interceptor &	speed(const Speed);


		// Methods
		void computePosition(double, Location &, Time)	const;
		Time computeInterception(Location, const Mobile &, Time, double &)	const;

};

std::ostream & operator << (std::ostream &, const Interceptor &);

#endif
