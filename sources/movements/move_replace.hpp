/**
 * @file move_replace.hpp
 * @brief Local Search optimisation movement. Try to replace a caught mobile by an uncaught one.
 */

#ifndef __MOVE_REPLACE_HPP__
#define __MOVE_REPLACE_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to replace a mobile into a single route.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveReplace : public Move
{
private:
	const Mobile * 			_best_mobile_in; ///< Mobile to insert in the route.
	const Mobile * 			_best_mobile_out; ///< Mobile to extract in the route.
	const Interceptor *		_best_interceptor; ///< Interceptor of the route to modify.
	Time 					_best_interception_date; ///< Best interception time found.

public:
	/**
	 * @brief Constructor.
	 */
	MoveReplace();

	/**
	 * @brief Destructor.
	 */
	virtual ~MoveReplace();

	/**
	 * @brief Scans if the replacement of a mobile by another improves the solution.
	 * @param solution The solution where the replacement will be scanned.
	 * @return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the replacement after a scan.
	 * @param solution The solution where the replacement will be done.
	 */
	virtual void commit(Solution &);
};

#include "move_replace.cxx"

#endif // __MOVE_REPLACE_HPP__
