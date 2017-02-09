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
	virtual ~Move() {};

	/**
	 * @brief Scans if the movement improves the solution.
	 * @param solution The solution where the movement will be scanned.
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
	const Mobile *			_best_mobile_candidate;	///< Mobile to insert
	int						_best_mobile_prev_index;///< Insertion of the mobile will be after this one
	const Interceptor * 	_best_interceptor;		///< Interceptor for the wanted route
	Time					_best_interception_date;///< Interception time of _mobile_in

public:
	/**
		 * @brief Constructor.
		 * @param p The routing problem the move refers to.
		 * @param m_in The mobile to insert in the route.
		 * @param m_prev The mobile which will be before the mobile to insert.
		 * @param i The interceptor of the route.
		 */
	MoveInsert();
	/**
		 * @brief Destructor.
		 */
	virtual ~MoveInsert();

	/**
	 * @brief Scans if the insertion of the mobile improves the solution.
	 * @param solution The solution where the insertion will be scanned.
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
	const Mobile * 			_best_mobile_candidate;
	Time 					_best_interception_date;
	const Mobile * 			_best_recompute_from;

public:
	MoveExtract();
	virtual ~MoveExtract();

	virtual bool scan(const Solution &);
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
	const Mobile *			_best_mobile_candidate_first;
	const Mobile *			_best_mobile_candidate_second;
	const Interceptor * 	_best_interceptor_first;
	const Interceptor * 	_best_interceptor_second;
	Time					_best_interception_date;

public:
	Move2Opt();
	virtual ~Move2Opt();

	virtual bool scan(const Solution &);
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
	const Mobile * 			_best_mobile_in;
	const Mobile * 			_best_mobile_out;
	const Interceptor *		_best_interceptor;
	Time 					_best_interception_date;

public:
	MoveReplace();
	virtual ~MoveReplace();

	virtual bool scan(const Solution &);
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
	const Mobile * 			_best_mobile_candidate;
	const Mobile * 			_best_mobile_insertion_prev;
	const Interceptor *		_best_interceptor_insertion;
	Time 					_best_interception_date;

public:
	MoveMove2Routes();
	virtual ~MoveMove2Routes();

	virtual bool scan(const Solution &);
	virtual void commit(Solution &);
};

#include "move.cxx"
#endif
