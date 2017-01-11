#ifndef __HEURISTIC_FASTEST_HPP__
#define __HEURISTIC_FASTEST_HPP__

#include "heuristic.hpp"
#include "cache_policy.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

template <class CachePolicy = NoCachePolicy>
class Heuristic_fastest : public Heuristic
{
public:
	Heuristic_fastest(const Problem &);
	virtual ~Heuristic_fastest();

	void run();

	template <bool = CachePolicy::isCached>
	void run();
	
};

#include "heuristic_fastest.cxx"
#endif
