#ifndef __MOVE_EXTRACT_HPP__
#define __MOVE_EXTRACT_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to remove a mobile from a given route.
 */
template <typename Policy = FirstAvailablePolicy>
class MoveExtract : public Move
{
private:
	const Mobile * 			_best_mobile_candidate; ///< Mobile to extract.
	Time 					_best_interception_date; ///< New interception date after the extraction.
	const Mobile * 			_best_recompute_from; ///< Best position to begin de recomputation of the interceptions in the route.

public:
	/**
	 * @brief Constructor.
	 */
	MoveExtract();

	/**
	 * @brief Destructor.
	 */
	virtual ~MoveExtract();

	/**
	 * @brief Scans if the extraction of the mobile improves the solution.
	 * @param solution The solution where the extraction will be scanned.
	 * return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the deletion after a scan.
	 * @param solution The solution where the extraction will be done.
	 */
	virtual void commit(Solution &);
};

#include "move_extract.cxx"

#endif // __MOVE_EXTRACT_HPP__
