#ifndef __HEURISTIC_HPP__
#define __HEURISTIC_HPP__

#include "solution.hpp"
#include "problem.hpp"

class Heuristic
{
protected:
	const 	Problem 	& _problem;
			Solution 	  _solution;
	std::string 		  _name;

public:
	Heuristic(const Problem &);
	virtual ~Heuristic();

	const Solution & solution() const;
		  Solution & solution();
	const Problem & problem() const;
	const std::string name() const;


	virtual void run() = 0;
};

std::ostream & operator<< (std::ostream &, const Heuristic &);

#endif
