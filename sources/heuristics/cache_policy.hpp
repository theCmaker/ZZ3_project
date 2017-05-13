/**
 * @file cache_policy.hpp
 * @brief Cache policy definitions.
 */
#ifndef __CACHE_POLICY_HPP__
#define __CACHE_POLICY_HPP__

#include <core/units.hpp>
#include <vector>

/**
 * @brief Cache policy without cache.
 */
class NoCachePolicy
{
public:
	static const bool isCached = false;		///< Cached status (enabled/disabled)
	/**
	 * @brief Initialize the cache.
	 * @param x X size
	 * @param y Y size
	 */
	static void init(unsigned x, unsigned y) {}
	/**
	 * @brief Define a value.
	 * @param x X position
	 * @param y Y position
	 * @param value time associated
	 */
	static void set(unsigned x, unsigned y, Time value) {}
	/**
	 * @brief Get a value.
	 * @param x X position
	 * @param y Y position
	 * @return 0 (default)
	 */
	static Time get(unsigned x, unsigned y) { return 0.; }
};

/**
 * @brief Cache policy for a solution.
 * Keeps a matrix of the interception durations.
 */
class SimpleCachePolicy
{
	// The time cache
	static std::vector<std::vector<Time> > _cache; ///< Cache matrix
public:
	static const bool isCached = true;		///< Cached status (enabled/disabled)
	/**
	 * @brief Initialize the cache.
	 * @param x X size
	 * @param y Y size
	 */
	inline static void init(unsigned x, unsigned y) {
		_cache.resize(x);
		for (auto & vec : _cache) {
			vec.resize(y);
		}
	}
	/**
	 * @brief Define a value.
	 * @param x X position
	 * @param y Y position
	 * @param value time associated
	 */
	inline static void set(unsigned x, unsigned y, Time value) {
		_cache[x][y] = value;
	}
	/**
	 * @brief Get a value.
	 * @param x X position
	 * @param y Y position
	 * @return 0 (default)
	 */
	inline static Time get(unsigned x, unsigned y) {
		return _cache[x][y];
	}
};

#endif // __CACHE_POLICY_HPP__
