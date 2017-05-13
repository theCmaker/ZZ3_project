#ifndef __MOVE_MOVE1ROUTE_HPP__
#define __MOVE_MOVE1ROUTE_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to move a mobile in another place in its route.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveMove1Route : public Move
{
private:
	const Mobile * 			_best_mobile_move; ///< Best mobile to move.
	int						_best_mobile_position; ///< Insertion after this mobile.
	const Interceptor *		_best_interceptor; ///< Interceptor of the route to modify.
	Time 					_best_interception_date; ///< Best interception date found.

public:
	/**
	 * @brief Constructor.
	 */
	MoveMove1Route();

	/**
	 * @brief Destructor.
	 */
	virtual ~MoveMove1Route();

	/**
	 * @brief Scans if the movement of the mobile in the route improves the solution.
	 * @param solution The solution where the movement will be scanned.
	 * @return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the movement after a scan.
	 * @param solution The solution where the movement will be done.
	 */
	virtual void commit(Solution &);
};

#include "move_move1route.cxx"

#endif // __MOVE_MOVE1ROUTE_HPP__
