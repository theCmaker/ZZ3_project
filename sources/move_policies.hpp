#ifndef __MOVE_POLICIES_HPP__
#define __MOVE_POLICIES_HPP__

#include "units.hpp"

class BestAvailablePolicy
{
	static const bool _keepOn = true;
public:
	static constexpr bool keepOn (Time value) { return _keepOn; }
	static void reset() {}
	static bool update (Time value, Time & record) {
		bool res = (value < 0. && value < record);
		if (res) { record = value; }
		return res;
	}
};

class FirstAvailablePolicy
{
	static bool _keepOn;
public:
	static bool keepOn (Time value) { return _keepOn; }
	static void reset() { _keepOn = true; }
	static bool update (Time value, Time & record) {
		_keepOn = (value >= 0.);
		bool res = (value < 0. && value < record);
		if (res) { record = value; }
		return res;
	}
};

#endif // __MOVE_POLICIES_HPP__
