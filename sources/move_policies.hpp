#ifndef __MOVE_POLICIES_HPP__
#define __MOVE_POLICIES_HPP__

#include "units.hpp"

class BestAvailablePolicy
{
	static const bool _keepOn = true;
public:
	static constexpr bool keepOn () { return _keepOn; }
	static void reset() {}
	static bool update (Time value, Time & record) {
		bool res = (value >= 0. && value <= record);
		if (res) { record = value; }
		//TODO: double check
		return res;
	}
};

class FirstAvailablePolicy
{
	static bool _keepOn;
public:
	static bool keepOn () { return _keepOn; }
	static void reset() { _keepOn = true; }
	static bool update (Time value, Time & record) {
		//std::cerr << "New value: " << value << " Record: " << record << std::endl;
		_keepOn = (value < 0. || value > record);
		if (! _keepOn) { record = value; }
		return ! _keepOn;
	}
};

bool FirstAvailablePolicy::_keepOn = true;
#endif // __MOVE_POLICIES_HPP__
