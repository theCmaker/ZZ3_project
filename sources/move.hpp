/**
 * @file move.hpp
 * @brief Declaration of an abstract class Move and its implementations (Insert, Delete, Cross, Replace).
 */

#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "solution.hpp"

/**
 * @brief Abstract class to make a movement in a route.
 */
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

/**
 * @brief Movement to insert a mobile in a given route.
 */
class InsertMove : public Move
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
		InsertMove(Problem & p, const Mobile & m_in, int m_prev, const Interceptor & i);
		/**
		 * @brief Destructor.
		 */
		virtual ~InsertMove();
		
		/**
		 * @brief Scans if the insertion of the mobile improves the solution.
		 * @param solution The solution where the insertion will be scanned.
		 */
		bool scan(const Solution &);
		
		/**
		 * @brief Commits the insertion after a scan.
		 * @param solution The solution where the insertion will be done.
		 */
		virtual void commit(Solution &);
};

/**
 * @brief Movement to delete a mobile in a given route.
 */
class DeleteMove : public Move
{
	private:
		const Mobile & 			_mobile_out;
		int						_mobile_prev;
		const Interceptor & 	_interceptor;
		
	public:
		DeleteMove(Problem &, const Mobile & m_out, int m_prev, const Interceptor & i);
		virtual ~DeleteMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

/**
 * @brief Movement to cross two routes.
 */
class CrossMove : public Move //TODO not implemented
{
	private:
		
	public:
		CrossMove(Problem &);
		virtual ~CrossMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

/**
 * @brief Movement to replace a mobile by another in a given route.
 */
class ReplaceMove : public Move //TODO not implemented
{
	private:
		
	public:
		ReplaceMove(Problem &);
		virtual ~ReplaceMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

#endif
