#ifndef __HEURISTIC_FASTEST_HPP__
#define __HEURISTIC_FASTEST_HPP__

#include "heuristic.hpp"

class Heuristic_fastest : public Heuristic
{
public:
	Heuristic_fastest(const Problem &);
	virtual ~Heuristic_fastest();

	void run();
	
};

#endif