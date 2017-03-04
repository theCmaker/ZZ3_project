#pragma once

#include <vector>
#include <problem.hpp>
#include <solution.hpp>
#include "solver.h"

// the decoder to be used in BRKGA
class Decoder
{
protected:
	Problem & data;
	Solver<double,Time,Solution> solver;

public:
	static unsigned VNDcounter;
	Decoder (Problem & d): data(d) {}
	double decode (std::vector<double> & chromosome);
	const Solver<double,Time,Solution> & getResult() const {return solver;}
};
