#ifndef __MOVE_INSERT_HPP__
#define __MOVE_INSERT_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to insert a mobile in a given route.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveInsert : public Move
{
private:
	const Mobile *			_best_mobile_candidate;	///< Mobile to insert.
	int						_best_mobile_prev_index;///< Insertion of the mobile will be after this one.
	const Interceptor * 	_best_interceptor;		///< Interceptor for the wanted route.
	Time					_best_interception_date;///< Interception time of _mobile_in.

public:
	/**
	 * @brief Constructor.
	 */
	MoveInsert();
	/**
	 * @brief Destructor.
	 */
	virtual ~MoveInsert();

	/**
	 * @brief Scans if the insertion of the mobile improves the solution.
	 * @param solution The solution where the insertion will be scanned.
	 * return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the insertion after a scan.
	 * @param solution The solution where the insertion will be done.
	 */
	virtual void commit(Solution &);
};

#include "move_insert.cxx"

#endif // __MOVE_INSERT_HPP__
