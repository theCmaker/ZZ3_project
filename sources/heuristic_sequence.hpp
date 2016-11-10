#ifndef __HEURISTIC_SEQUENCE_HPP__
#define __HEURISTIC_SEQUENCE_HPP__

#include "heuristic.hpp"

class Heuristic_sequence : public Heuristic
{
public:
	Heuristic_sequence(const Problem &);
	virtual ~Heuristic_sequence();

	void run();
	void run(const std::vector<unsigned> &);

};

#endif
