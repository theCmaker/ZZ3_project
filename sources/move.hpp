/**
 * @file move.hpp
 * @brief Declaration of an abstract class Move and its implementations (Insert, Extract, 2Opt, Replace, Move, Swap).
 */

#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "solution.hpp"
#include "move_policies.hpp"

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


// ---------------------------------------------------------------------------------------------------------------------
// INSERT MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// EXTRACT MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// TWO-OPT MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// REPLACE MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// MOVE 1 ROUTE MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// MOVE 2 ROUTES MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// SWAP1ROUTE MOVE
// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// SWAP2ROUTES MOVE
// ---------------------------------------------------------------------------------------------------------------------
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

#include "move.cxx"
#endif
