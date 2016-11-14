#ifndef __DEPOT_HPP__
#define __DEPOT_HPP__

#include "units.hpp"
#include "interceptor.hpp"
#include <vector>

class Depot
{
	public:
		typedef std::vector<const Interceptor*> VInterceptorPtrs;
	private:
		unsigned			_id;
		Location			_position;
		VInterceptorPtrs	_interceptors;

	public:
		Depot(unsigned = 0);
		Depot(Distance, Distance, unsigned = 0);
		Depot(Location &, unsigned = 0);
		~Depot();

		//Getters
		const unsigned & id() const;
		const Location & position() const;
		const VInterceptorPtrs & interceptors() const;

		//Setters
		Depot & position(const Location &);

		//Methods
		Depot & addInterceptor(const Interceptor &);
};

std::ostream & operator<< (std::ostream &, const Depot &);

#endif
