/**
 * @file decoder.h
 * @brief BRKGA chromosome decoder - Evaluate a chromosome
 */

#pragma once

#include <vector>
#include <core/problem.hpp>
#include <core/solution.hpp>
#include "solver.h"

/**
 * @brief Decoder to be used in BRKGA
 */
class Decoder
{
protected:
	Problem & data;							///< Instance of the problem
	Solver<double,Time,Solution> solver;	///< Evaluation tool

public:
	static unsigned VNDcounter;		///< Counter of VND calls

	/**
	 * @brief Constructor
	 * @param d Instance of the problem
	 */
	Decoder (Problem & d): data(d) {}

	/**
	 * @brief Decode (evaluate) a chromosome
	 * @param chromosome Representation of an order to intercept the mobiles (Sequence heuristic with intermediate solutions)
	 * @return The hypervolumen corresponding to the solutions (after VND execution on each one).
	 */
	double decode (std::vector<double> & chromosome);

	/**
	 * @brief Get the Result of the last decoding operation
	 * @return the solver instance with the underlying results
	 * @warning decode method must have been called before
	 */
	const Solver<double,Time,Solution> & getResult() const {return solver;}
};
