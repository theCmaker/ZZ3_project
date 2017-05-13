/**
 * @file heuristic.hpp
 * @brief Heuristic handling.
 * Compute a Solution to a Problem.
 */
#ifndef __HEURISTIC_HPP__
#define __HEURISTIC_HPP__

#include <core/solution.hpp>
#include <core/problem.hpp>

/**
 * @brief Heuristic computing, abstract class.
 */
class Heuristic
{
protected:
	const 	Problem 	& _problem;		///< Problem data
			Solution 	  _solution;	///< Solution values
	std::string 		  _name;		///< Heuristic name

public:
	/**
	 * @brief Constructor.
	 * @param p Problem data
	 */
	Heuristic(const Problem & p);
	/**
	 * @brief Destructor.
	 */
	virtual ~Heuristic();

	/**
	 * @brief Get the solution (immutable form).
	 * @return Solution to the Problem
	 */
	const Solution & solution() const;
	/**
	 * @brief Get the solution (mutable form).
	 * @return Solution to the Problem
	 */
		  Solution & solution();
	/**
	 * @brief Get the problem (immutable).
	 * @return Problem data
	 */
	const Problem & problem() const;
	/**
	 * @brief Get the name of the heuristic.
	 * @return Name of the Heuristic
	 */
	const std::string name() const;

	/**
	 * @brief Compute the solution.
	 */
	virtual void run() = 0;
};

/**
 * @brief Append a heuristic to a stream.
 * @param o the output stream
 * @param h the heuristic
 * @return the output stream (chained instructions)
 */
std::ostream & operator<< (std::ostream & o, const Heuristic & h);

#endif
