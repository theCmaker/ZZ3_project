/**
 * @file heuristic_fastest.hpp
 * @brief Provide a good solution with the lowest interception time.
 */
#ifndef __HEURISTIC_FASTEST_HPP__
#define __HEURISTIC_FASTEST_HPP__

#include "heuristic.hpp"
#include "cache_policy.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

/**
 *	@brief Fastest heuristic. The mobile to be caught is the one that needs the
 * shortest time to be intercepted by any interceptor.
 *
 * A cache policy can be used to get results faster.
 */
template <class CachePolicy = NoCachePolicy>
class Heuristic_fastest : public Heuristic
{
public:
	/**
	 * @brief Constructor.
	 * @param p Problem data
	 */
	Heuristic_fastest(const Problem & p);
	/**
	 * @brief Destructor.
	 */
	virtual ~Heuristic_fastest();

	void run();

	template <bool = CachePolicy::isCached>
	void run();
	
};

#include "heuristic_fastest.cxx"
#endif
