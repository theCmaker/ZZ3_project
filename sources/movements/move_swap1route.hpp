/**
 * @file move_swap1route.hpp
 * @brief Local Search optimisation movement. Try to exchange two mobiles inside a route.
 */

#ifndef __MOVE_SWAP1ROUTE_HPP__
#define __MOVE_SWAP1ROUTE_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to swap a mobile with another mobile in its route.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveSwap1Route : public Move
{
private:
	const Mobile * 			_best_mobile_swap1; ///< First mobile to swap.
	const Mobile * 			_best_mobile_swap2; ///< Second mobile to swap.
	const Interceptor *		_best_interceptor; ///< Interceptor of the route.
	Time 					_best_interception_date; ///< Best interception time found.

public:
	/**
	 * @brief Constructor.
	 */
	MoveSwap1Route();

	/**
	 * @brief Destructor.
	 */
	virtual ~MoveSwap1Route();

	/**
	 * @brief Scans if the exchange of two mobiles in a route will improve the solution.
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

#include "move_swap1route.cxx"

#endif // __MOVE_SWAP1ROUTE_HPP__
