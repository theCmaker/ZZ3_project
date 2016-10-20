#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__

#include <vector>
#include <iostream>
#include "interceptor.hpp"
#include "mobile.hpp"
#include "depot.hpp"

class Problem
{
	private:
		std::vector<Interceptor> 	_interceptors;
		std::vector<Mobile> 		_mobiles;
		std::vector<Depot>			_depots;
	
	public:
		Problem();
		Problem(std::string);
		Problem(Problem &);
		~Problem();
		
		
};

#endif
