#ifndef __CACHE_POLICY_HPP__
#define __CACHE_POLICY_HPP__

#include "units.hpp"
#include <vector>

class NoCachePolicy
{
public:
	static const bool isCached = false;
	static void init(unsigned,unsigned) {}
	static void set(unsigned,unsigned,Time) {}
	static Time get(unsigned,unsigned) { return 0.; }
};

class SimpleCachePolicy
{
	// The time cache
	static std::vector<std::vector<Time> > _cache;
public:
	static const bool isCached = true;
	inline static void init(unsigned x, unsigned y) {
		_cache.resize(x);
		for (auto & vec : _cache) {
			vec.resize(y);
		}
	}

	inline static void set(unsigned x, unsigned y, Time value) {
		_cache[x][y] = value;
	}
	inline static Time get(unsigned x, unsigned y) {
		return _cache[x][y];
	}
};

#endif // __CACHE_POLICY_HPP__
