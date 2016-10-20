#ifndef __DEPOT_HPP__
#define __DEPOT_HPP__

#include "units.hpp"
#include "interceptor.hpp"
#include <vector>

class Depot
{
	private:
		Location					_position;
		std::vector<Interceptor>	_interceptors;
	
	public:
		Depot();
		Depot(Distance, Distance);
		Depot(Location &, Interceptor &);
		Depot(Location &);
		~Depot();

		//Getters
		const Location & position() const;

		//Setters
		Depot & position(const Location &);
};

std::ostream & operator<< (std::ostream &, const Depot &);

#endif
