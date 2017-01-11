#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__

#include <vector>
#include <iostream>
#include "interceptor.hpp"
#include "mobile.hpp"
#include "depot.hpp"

typedef std::vector<Interceptor> VInterceptors;
typedef std::vector<Mobile> VMobiles;
typedef std::vector<Depot> VDepots;

class Problem
{
	private:
		VInterceptors	_interceptors;
		VMobiles		_mobiles;
		VDepots			_depots;
	public:
		Problem();
		Problem(unsigned,unsigned,unsigned,bool=false);
		Problem(const char *);
		Problem(Problem &);
		~Problem();

		//Getters
		unsigned nbInterceptors() const;
		unsigned nbMobiles() const;
		unsigned nbDepots() const;

		const VMobiles & mobiles() const;
		const VInterceptors & interceptors() const;
		const VDepots & depots() const;

		void write(const char *) const;
};

std::ostream & operator<< (std::ostream &, const Problem &);

#endif
