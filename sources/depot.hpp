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
		Depot(Location &, Interceptor &);
		~Depot();
	
};

#endif
