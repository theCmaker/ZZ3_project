/**
 * @file move_swap2routes.hpp
 * @brief Local Search optimisation movement. Try to exchange 2 mobiles between 2 routes.
 */

#ifndef __MOVE_SWAP2ROUTES_HPP__
#define __MOVE_SWAP2ROUTES_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to swap 2 mobiles in 2 routes.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveSwap2Routes : public Move
{
private:
	const Mobile * 			_best_mobile_swap1; ///< Mobile to swap in the first route.
	const Mobile * 			_best_mobile_swap2; ///< Mobile to swap in the second route.
	const Interceptor *		_best_interceptor1; ///< Interceptor of the first route.
	const Interceptor *		_best_interceptor2; ///< Interceptor of the second route.
	Time 					_best_interception_date; ///< Best interception time found.

public:
	/**
	 * @brief Constructor.
	 */
	MoveSwap2Routes();

	/**
	 * @brief Destructor.
	 */
	virtual ~MoveSwap2Routes();

	/**
	 * @brief Scans if the exchange of two mobiles in different routes will improve the solution.
	 * @param solution The solution where the swap will be scanned.
	 * @return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the swap after a scan.
	 * @param solution The solution where the swap will be done.
	 */
	virtual void commit(Solution &);
};

#include "move_swap2routes.cxx"

#endif // __MOVE_SWAP2ROUTES_HPP__
