#pragma once

#include <vector>
#include <limits>
#include <Pareto/datapoint.h>
#include <core/problem.hpp>

template <typename C1, typename C2, typename DT>
struct Solver
{
	static C1 minC1;
	static C1 maxC1;
	static C2 minC2;
	static C2 maxC2;
	std::vector< DataPoint<C1,C2,DT> > _data;

	double hypervolume;

	double computeHypervolume ();
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
	void clear () {
		minC1 = 0;
		maxC1 = std::numeric_limits<C1>::min();
		minC2 = 0;
		maxC2 = std::numeric_limits<C2>::min();
		_data.clear(); hypervolume = std::numeric_limits<double>::infinity();
	}
};

template <typename C1, typename C2, typename DT>
std::ostream & operator<< (std::ostream & os, const Solver<C1,C2,DT> & f);

#include "solver.cxx"
