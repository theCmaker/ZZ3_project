#ifndef __INTERPETOR_HPP__
#define __INTERPETOR_HPP__

#include "units.hpp"
#include "mobile.hpp"
#include <iostream>

class Interceptor
{
	private:
		unsigned	_id;
		Location	_position;
		Speed		_speed;

		static double compute_alpha(double, double, double);


	public:
		Interceptor(unsigned = 0);
		Interceptor(Location &, Speed, unsigned = 0);
		Interceptor(Distance, Distance, Speed, unsigned = 0);
		~Interceptor();

		// Getters
		const unsigned & id() const;
		const Location &	position() const;
			  Speed			speed() const;


		// Setters
		Interceptor &		position(const Location &);
		Interceptor &		speed(const Speed);


		// Methods
		void compute_position(double, Location &, Time) const;
		Time compute_interception(Location, const Mobile &, Time, double &) const;

};

std::ostream & operator << (std::ostream &, const Interceptor &);

#endif
