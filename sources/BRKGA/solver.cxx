#include "paretofrontsolver.h"

template <typename C1, typename C2, typename DT>
C1 Solver<C1,C2,DT>::minC1 = std::numeric_limits<C1>::max();
template <typename C1, typename C2, typename DT>
C1 Solver<C1,C2,DT>::maxC1 = std::numeric_limits<C1>::min();
template <typename C1, typename C2, typename DT>
C2 Solver<C1,C2,DT>::minC2 = std::numeric_limits<C2>::max();
template <typename C1, typename C2, typename DT>
C2 Solver<C1,C2,DT>::maxC2 = std::numeric_limits<C2>::min();

template <typename C1, typename C2, typename DT>
double Solver<C1,C2,DT>::computeHypervolume () {
	ParetoFrontSolver<Point<C1,C2,DT>> s;
	s.setPts(_data);
	s.compute_frontiers();
	s.getPFrontiers()[0].compute_stats(double(minC2), double(maxC2), double(minC1), double(maxC1));
	hypervolume = s.getPFrontiers()[0].hypervolumen();
	return hypervolume;
}

template <typename C1, typename C2, typename DT>
std::ostream & operator<< (std::ostream & os, const Solver<C1,C2,DT> & f)
{
	os << f._data.size() << " elements, " << f.hypervolume << " hypervolume" << std::endl;
	for (size_t i = 0; i < f._data.size(); ++i)
	{
		os << i << ": " << f._data[i] << std::endl;
	}
	return os;
}
