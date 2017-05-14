/**
 * @file move.hpp
 * @brief Declaration of an abstract class for Local Search optimisation movements.
 */

#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include <core/solution.hpp>

/**
 * @brief Abstract class to make a movement in a route.
 */
class Move
{
public:
	/**
	 * @brief Constructor.
	 */
	Move() {}

	/**
	 * @brief Destructor.
	 */
	virtual ~Move() {}

	/**
	 * @brief Scans if the movement improves the solution.
	 * @param solution The solution where the movement will be scanned.
	 * return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution & solution) = 0;

	/**
	 * @brief Commits the movement after a scan.
	 * @param solution The solution where the movement will be done.
	 */
	virtual void commit(Solution & solution) = 0;
};

#endif
