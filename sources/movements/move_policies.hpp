#ifndef __MOVE_POLICIES_HPP__
#define __MOVE_POLICIES_HPP__

#include <core/units.hpp>

class AvailablePolicy
{
protected:
	static Time _max_acceptable_time;
	static unsigned _min_acceptable_count;
public:
	static Time & maxAcceptableTime();
	static unsigned & minAcceptableCount();
};

class BestAvailablePolicy : public AvailablePolicy
{
	static const bool _keepOn = true;
public:
	static constexpr bool keepOn () { return _keepOn; }
	static void reset();
	static bool update (Time value, Time & record);
	static bool update (Time value, Time & record, unsigned count, unsigned & count_record);
};

class FirstAvailablePolicy : public AvailablePolicy
{
	static bool _keepOn;
public:
	static bool keepOn ();
	static void reset();
	static bool update (Time value, Time & record);
	static bool update (Time value, Time & record, unsigned count, unsigned & count_record);
};

#endif // __MOVE_POLICIES_HPP__
