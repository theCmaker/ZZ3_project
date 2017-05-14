/**
 * @file solver.h
 * @brief Compute the Pareto front of a set of points and its hypervolume
 */

#pragma once

#include <vector>
#include <limits>
#include <Pareto/datapoint.h>
#include <core/problem.hpp>

/**
 * @brief Bi-criteria optimisation tool
 * @tparam C1 first criteria
 * @tparam C2 second criteria
 * @tparam DT Attached data
 */
template <typename C1, typename C2, typename DT>
struct Solver
{
	static C1 minC1;	///< Min value on first criterium
	static C1 maxC1;	///< Max value on first criterium
	static C2 minC2;	///< Min value on second criterium
	static C2 maxC2;	///< Max value on second criterium
	std::vector< DataPoint<C1,C2,DT> > _data;	///< Original data (points) to compute Pareto front

	double hypervolume;	///< Hypervolume of the best Pareto front

	/**
	 * @brief Compute the hypervolume of the first Pareto front
	 * @return hypervolume value.
	 */
	double computeHypervolume ();

	/**
	 * @brief Add a data point.
	 * @param c1 first criterium value
	 * @param c2 second criterium value
	 * @param dt attached data
	 */
	void add (const C1 & c1, const C2 & c2, const DT & dt) {
		if (c1 < minC1) {
			minC1 = c1;
		}
		if (c1 > maxC1) {
			maxC1 = c1;
		}
		if (c2 < minC2) {
			minC2 = c2;
		}
		if (c2 > maxC2) {
			maxC2 = c2;
		}
		_data.emplace_back(c1,c2,dt);
	}

	/**
	 * @brief Reset the data, limits, and computed values.
	 */
	void clear () {
		minC1 = 0;
		maxC1 = std::numeric_limits<C1>::min();
		minC2 = 0;
		maxC2 = std::numeric_limits<C2>::min();
		_data.clear(); hypervolume = std::numeric_limits<double>::infinity();
	}
};

/**
 * @brief Append the string representation of the solver results to an output stream
 * @param os output stream
 * @param f solver instance
 */
template <typename C1, typename C2, typename DT>
std::ostream & operator<< (std::ostream & os, const Solver<C1,C2,DT> & f);

#include "solver.cxx"
