#include "move_policies.hpp"

//----------------------------------------------------------------------------------------------------------------------
// CLASS MEMBERS
//----------------------------------------------------------------------------------------------------------------------
Time AvailablePolicy::_max_acceptable_time;

unsigned AvailablePolicy::_min_acceptable_count;

bool FirstAvailablePolicy::_keepOn = true;


//----------------------------------------------------------------------------------------------------------------------
// CLASS METHODS
//----------------------------------------------------------------------------------------------------------------------
Time & AvailablePolicy::maxAcceptableTime() { return _max_acceptable_time; }

unsigned & AvailablePolicy::minAcceptableCount() { return _min_acceptable_count; }


//----------------------------------------------------------------------------------------------------------------------
// BEST AVAILABLE
//----------------------------------------------------------------------------------------------------------------------
void BestAvailablePolicy::reset()
{}

bool BestAvailablePolicy::update(Time value, Time & record)
{
	bool res = (value >= 0. && value <= record);
	if (res) {
		record = value;
	}
	return res;
}

bool BestAvailablePolicy::update(Time value, Time & record, unsigned count, unsigned & count_record)
{
	bool res = (value >= 0. && value <= record && count >= count_record);
	if (res) {
		record = value;
		count_record = count;
	}
	return res;
}


//----------------------------------------------------------------------------------------------------------------------
// FIRST AVAILABLE
//----------------------------------------------------------------------------------------------------------------------
bool FirstAvailablePolicy::keepOn()
{
	return _keepOn;
}

void FirstAvailablePolicy::reset()
{
	_keepOn = true;
}

bool FirstAvailablePolicy::update(Time value, Time & record)
{
	_keepOn = (value < 0. || value > _max_acceptable_time);
	if (! _keepOn) {
		record = value;
	}
	return ! _keepOn;
}

bool FirstAvailablePolicy::update(Time value, Time & record, unsigned count, unsigned & count_record)
{
	_keepOn = (value < 0. || value > _max_acceptable_time || count < _min_acceptable_count);
	if (! _keepOn) {
		record = value;
		count_record = count;
	}
	return ! _keepOn;
}
