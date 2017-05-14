/**
 * @file move_policies.hpp
 * @brief Local Search optimisation policies. Determine wether to continue or stop looking for a better result.
 */

#ifndef __MOVE_POLICIES_HPP__
#define __MOVE_POLICIES_HPP__

#include <core/units.hpp>

/**
 * @brief Basic class to handle optimisation limits (bi-criteria)
 */
class AvailablePolicy
{
protected:
	static Time _max_acceptable_time;		///< Maximum time for a route
	static unsigned _min_acceptable_count;	///< Minimum number of mobiles to be intercepted

public:
	/**
	 * @brief Access the maximum allowed time
	 * @return Reference to copy or change the maximum allowed time for a route
	 */
	static Time & maxAcceptableTime();

	/**
	 * @brief Access the minimum needed intercepted mobiles
	 * @return Reference to copy or change the minimum needed intercepted mobiles
	 */
	static unsigned & minAcceptableCount();
};

/**
 * @brief Keep on looking for the best optimisation result
 */
class BestAvailablePolicy : public AvailablePolicy
{
	static const bool _keepOn = true; ///< keep on indicator. Always true
public:
	/**
	 * @brief Know if research can go on
	 * @return true
	 */
	static constexpr bool keepOn () { return _keepOn; }

	/**
	 * @brief Reset the policy. Does nothing here
	 */
	static void reset();

	/**
	 * @brief Single criterium test
	 * @param value  Value to be tested
	 * @param record Reference value
	 * @return true if value is better
	 */
	static bool update (Time value, Time & record);

	/**
	 * @brief Double criteria test
	 * @param value  Value to be tested
	 * @param record Reference value
	 * @param count  Second criterium value
	 * @param count_record Reference value for the second criterium
	 * @return true if (value,count) is better
	 */
	static bool update (Time value, Time & record, unsigned count, unsigned & count_record);
};

/**
 * @brief Stop looking for better solutions after the first one
 */
class FirstAvailablePolicy : public AvailablePolicy
{
	static bool _keepOn;	///< keep on indicator. Always true
public:
	/**
	 * @brief Know if research can go on
	 * @return true
	 */
	static bool keepOn ();

	/**
	 * @brief Reset the policy.
	 */
	static void reset();

	/**
	 * @brief Single criterium test
	 * @param value  Value to be tested
	 * @param record Reference value
	 * @return true if value is better
	 */
	static bool update (Time value, Time & record);

	/**
	 * @brief Double criteria test
	 * @param value  Value to be tested
	 * @param record Reference value
	 * @param count  Second criterium value
	 * @param count_record Reference value for the second criterium
	 * @return true if (value,count) is better
	 */
	static bool update (Time value, Time & record, unsigned count, unsigned & count_record);
};

#endif // __MOVE_POLICIES_HPP__
