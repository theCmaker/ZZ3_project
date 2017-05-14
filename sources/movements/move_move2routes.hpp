/**
 * @file move_move2routes.hpp
 * @brief Local Search optimisation movement. Try to move a mobile from one route to another.
 */

#ifndef __MOVE_MOVE2ROUTES_HPP__
#define __MOVE_MOVE2ROUTES_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to extract a mobile from a route and insert it into another one.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveMove2Routes : public Move
{
private:
	const Mobile * 			_best_mobile_candidate; ///< Best mobile to move.
	const Mobile * 			_best_mobile_insertion_prev; ///< Insertion after this mobile.
	const Interceptor *		_best_interceptor_insertion; ///< Interceptor of the route where the mobile will be inserted.
	Time 					_best_interception_date; ///< Best interception date found.

public:
	/**
	 * @brief Constructor.
	 */
	MoveMove2Routes();

	/**
	 * @brief Destructor.
	 */
	virtual ~MoveMove2Routes();

	/**
	 * @brief Scans if the extraction and insertion will improve the solution.
	 * @param solution The solution where the movement will be scanned.
	 * @return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the movement after a scan.
	 * @param solution The solution where the movement will be scanned.
	 */
	virtual void commit(Solution &);
};

#include "move_move2routes.cxx"

#endif // __MOVE_MOVE2ROUTES_HPP__
