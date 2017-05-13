#ifndef __MOVE_2OPT_HPP__
#define __MOVE_2OPT_HPP__

#include "move.hpp"
#include "move_policies.hpp"

/**
 * @brief Movement to cross two routes.
 */
template <typename Policy = FirstAvailablePolicy>
class Move2Opt : public Move
{
private:
	const Mobile *			_best_mobile_candidate_first; ///< Begin of the sequence in the first route.
	const Mobile *			_best_mobile_candidate_second; ///< Begin of the sequence in the second route.
	const Interceptor * 	_best_interceptor_first; ///< Interceptor of the first sequence.
	const Interceptor * 	_best_interceptor_second; ///< Interceptor of the second sequence.
	Time					_best_interception_date; ///< Best interception time found.

public:
	/**
	 * @brief Constructor.
	 */
	Move2Opt();

	/**
	 * @brief Destructor.
	 */
	virtual ~Move2Opt();

	/**
	 * @brief Scans if the inversion of two sequences in two routes improves the solution.
	 * @param solution The solution where the 2-Opt movement will be scanned.
	 * @return true if the scan improves the solution, false otherwise.
	 */
	virtual bool scan(const Solution &);

	/**
	 * @brief Commits the inversion of both sequences after a scan.
	 * @param solution The solution where the 2-Opt movement will be done.
	 */
	virtual void commit(Solution &);
};

#include "move_2opt.cxx"

#endif // __MOVE_2OPT_HPP__
