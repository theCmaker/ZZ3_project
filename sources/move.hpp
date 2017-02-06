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
template <typename Policy = FirstAvailablePolicy>
class Move
{
protected:
	Problem & _p; ///< Routing problem

public:
	/**
		 * @brief Constructor.
		 * @param p The routing problem the move refers to.
		 */
	Move(Problem & p);
	/**
		 * @brief Destructor.
		 */
	virtual ~Move();

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
class MoveInsert : public Move<Policy>
{
private:
	const Mobile & 			_mobile_in;		///< Mobile to insert
	int						_mobile_prev;	///< Insertion of the mobile will be after this one
	const Interceptor & 	_interceptor;	///< Interceptor for the wanted route
	Time					_interception_date; ///< Interception time of _mobile_in

public:
	/**
		 * @brief Constructor.
		 * @param p The routing problem the move refers to.
		 * @param m_in The mobile to insert in the route.
		 * @param m_prev The mobile which will be before the mobile to insert.
		 * @param i The interceptor of the route.
		 */
	MoveInsert(Problem & p, const Mobile & m_in, int m_prev, const Interceptor & i);
	/**
		 * @brief Destructor.
		 */
	virtual ~MoveInsert();

	/**
		 * @brief Scans if the insertion of the mobile improves the solution.
		 * @param solution The solution where the insertion will be scanned.
		 */
	using Move<Policy>::_p;
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
class MoveExtract : public Move<Policy>
{
private:
	const Mobile & 			_mobile_out;
	int						_mobile_prev;
	const Interceptor & 	_interceptor;

public:
	MoveExtract(Problem & p, const Mobile & m_out, int m_prev, const Interceptor & i);
	virtual ~MoveExtract();

	using Move<Policy>::_p;
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
class Move2Opt : public Move<Policy>
{
private:
	int _mobile[2];

public:
	Move2Opt(Problem & p, int m0, int m1);
	virtual ~Move2Opt();

	using Move<Policy>::_p;
	virtual bool scan(const Solution &);
	virtual void commit(Solution &);
};


#include "move.cxx"
#endif
