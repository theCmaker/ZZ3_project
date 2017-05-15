/**
 * @file solution.hpp
 * @brief Implementation of a Solution. Gears the interceptions.
 */

#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__

#include <list>
#include "problem.hpp"
/**
 * @brief Describe an interception sequence, i.e. the order mobiles are
 * intercepted by the interceptors.
 */
class Solution
{
public:
	/**
	 * @brief Internal structure describing an interception
	 * (mobile,interceptor,date).
	 */
	struct InterceptionNode
	{
		//Metadata
		Time				_date;			///< Interception date
		const Mobile	  & _mobile;		///< Intercepted mobile
		const Interceptor * _interceptor;	///< Interceptor that caught the _mobile

		//Mobiles chained list
		int _next;			///< Next mobile index
		int _prev;			///< Previous mobile index

		//Constructors
		/**
		 * @brief Constructor.
		 * @param m the intercepted mobile
		 * @param d the interception date
		 * @param i the interceptor
		 * @param prev previous mobile index
		 * @param next next mobile index
		 */
		InterceptionNode(const Mobile &, const Time, const Interceptor *, int prev = -1, int next = -1);
		/**
		 * @brief Default constructor.
		 * @param m the mobile
		 * @warning a mobile must be provided.
		 * @note _date defaults to -1.
		 */
		InterceptionNode(const Mobile &);
	};

	/**
	 * @brief Internal structure describing a route
	 * (interceptor, first and last mobile).
	 */
	struct RouteNode
	{
		//Metadata
		const Interceptor & _interceptor; ///< Concerned interceptor

		//Mobiles
		int _first;		///< First intercepted mobile index
		int _last;		///< Last intercepted mobile index

		//Interceptors
		int _next;		///< Next route (for another interceptor)
		int _prev;		///< Previous route (for another interceptor)

		//Constructors
		/**
		 * @brief Constructor.
		 * @param i the interceptor that realizes the route
		 * @param first first intercepted mobile id
		 * @param last last intercepted mobile id
		 * @param prev previous route id
		 * @param next next route id
		 * @warning an interceptor must be provided.
		 */
		RouteNode(const Interceptor &, int first = -1, int last = -1, int prev = -1, int next = -1);
	};

	/**
	 * @brief Provide iterator over mobile nodes.
	 */
	class iterator
	{
	private:
		InterceptionNode * _solution; ///< Pointer to the current interception
	public:
		/**
		 * @brief Constructor.
		 * @param s the interception node
		 */
		inline iterator(InterceptionNode * s) :
			_solution(s)
		{}
		/**
		 * @brief Destructor.
		 */
		inline ~iterator() {}
		/**
		 * @brief Indirection operator.
		 * @return Current interception
		 */
		inline InterceptionNode & operator* ()
		{
			return *_solution;
		}
		/**
		 * @brief Arrow operator.
		 * @return Pointer to current interception
		 */
		inline InterceptionNode * operator-> ()
		{
			return _solution;
		}
		/**
		 * @brief Increment operator.
		 * @return Next interception node
		 */
		inline iterator & operator++ ()
		{
			_solution = _solution - int(_solution->_mobile.id()) + _solution->_next;
			return *this;
		}
		/**
		 * @brief Inequality operator.
		 * @param itr another iterator.
		 * @return true if this equals the given iterator.
		 */
		inline bool operator!= (iterator itr)
		{
			return _solution != itr._solution;
		}
	};
	/**
	 * @brief Provide const_iterator over mobile nodes.
	 */
	class const_iterator
	{
	private:
		const InterceptionNode * _solution; ///< Pointer to the current interception
	public:
		/**
		 * @brief Constructor.
		 * @param s the interception node
		 */
		inline const_iterator(const InterceptionNode * s) :
			_solution(s)
		{}
		/**
		 * @brief Destructor.
		 */
		inline ~const_iterator() {}
		/**
		 * @brief Indirection operator.
		 * @return Current interception
		 */
		inline const InterceptionNode & operator* () const
		{
			return *_solution;
		}
		/**
		 * @brief Arrow operator.
		 * @return Pointer to current interception
		 */
		inline const InterceptionNode * operator-> () const
		{
			return _solution;
		}
		/**
		 * @brief Increment operator.
		 * @return Next interception node
		 */
		inline const_iterator & operator++ ()
		{
			_solution = _solution - int(_solution->_mobile.id()) + _solution->_next;
			return *this;
		}
		/**
		 * @brief Inequality operator.
		 * @param itr another iterator.
		 * @return true if this equals the given iterator.
		 */
		inline bool operator!= (const_iterator itr)
		{
			return _solution != itr._solution;
		}
	};
private:
	const Problem				  & _problem;		///< Routing problem
	std::vector<InterceptionNode>			_sequence;		///< Interceptions
	std::vector<RouteNode>	_interceptors;	///< Routes

	//Interceptors
	int _first;		///< First route
	int _last;		///< Last route

public:
	/**
	 * @brief Constructor.
	 * @param p the routing problem the solution refers to
	 */
	Solution(const Problem &);

	/**
	 * @brief Copy Constructor.
	 * @param s the other solution
	 */
	Solution(const Solution & s);

	/**
	 * @brief Destructor.
	 */
	~Solution();

	/**
	 * @brief Affectation operator
	 * @param other The solution values to be copied in this.
	 */
	Solution & operator= (const Solution & other);

	/**
	 * @brief Get the relative problem.
	 * @return Problem relative to the current solution
	 */
	const Problem & problem() const;

	/**
	 * @brief Shake a solution and apply the insertion heuristic on the new sequence.
	 */
	void shake();

	/**
	 * @brief Get a random Solution for a Problem
	 * @param p the problem
	 * @return a random Solution to Problem p
	 */
	static Solution random(const Problem & p);

	/**
	 * @brief Add an interception at the end of a route.
	 * @param interceptor_index id of the interceptor (route)
	 * @param mobile_index id of the intercepted mobile
	 * @param d interception date
	 */
	void append(unsigned interceptor_index, unsigned mobile_index, const Time & d);

	/**
	 * @brief Add an interception at the beginning of a route.
	 * @param interceptor_index id of the interceptor (route)
	 * @param mobile_index id of the intercepted mobile
	 * @param d interception date
	 */
	void prepend(unsigned interceptor_index, unsigned mobile_index, const Time & d);

	/**
	 * @brief Add an interception after a mobile in a route.
	 *
	 * @param prev_mobile_index id of the previous mobile
	 * @param interceptor_index id of the interceptor (route)
	 * @param mobile_index id of the intercepted mobile
	 * @param d interception date
	 */
	void insertAfter(unsigned prev_mobile_index, unsigned interceptor_index, unsigned mobile_index, const Time & d);

	/**
	  * @brief Insertion of a sequence of mobiles at the end of a route. Called after removeSeqFrom.
	  * @param i interceptor of the route to insert
	  * @param m first mobile in the sequence to insert
	  */
	void appendSeq(const Interceptor & i, const Mobile & m);

	/**
	 * @brief Remove a mobile from its route.
	 * @param mobile_index index of the mobile to be removed
	 */
	void remove(unsigned mobile_index);

	/**
	  * @brief Removes a sequence of nodes in a route. Datas in MobileNode are NOT change as the sequence will be inserted as given in an other route with appendSeq() (used in Move2Opt).
	  * @param m first mobile of the sequence to remove.
	  */
	void removeSeqFrom(const Mobile & m);

	/**
	 * @brief Recompute the interception dates in a route, starting from the given mobile index.
	 * @param mobile_index index of the last mobile with a valid interception time
	 * @return last interception date for the recomputed route
	 */
	Time recomputeFrom(unsigned mobile_index);

	/**
	 * @brief Evaluate the time needed by an interceptor to go from a mobile to
	 * another one of the same route, starting from a given position at a given
	 * date.
	 *
	 * @param start_pos Start position of the interceptor
	 * @param start_date Start date of the interceptor
	 * @param interceptor Interceptor
	 * @param first_mobile First mobile to catch
	 * @param last_mobile Last mobile to catch (same route as first_mobile)
	 *
	 * @warning first_mobile and last_mobile must belong to the same route, and
	 * first_mobile must be before last_mobile.
	 *
	 * @return Time needed by the interceptor (/!\ != end date)
	 */
	Time evaluate(const Location & start_pos, const Time start_date, const Interceptor & interceptor,
				  const Mobile & first_mobile, const Mobile & last_mobile) const;

	/**
	 * @brief Get the last interception date for all routes.
	 * @return The last interception date for all routes
	 */
	Time lastInterceptionTime() const;
	/**
	 * @brief Get the longest route in time.
	 * @return The interceptor for the longest route in time
	 */
	const Interceptor & worstRoute() const;
	/**
	 * @brief Get the last interception date for the given interceptor (route) id.
	 * @param interceptor_index the route or interceptor id.
	 * @return last interception date for interceptor id interceptor_index
	 */
	Time lastInterceptionTime(int interceptor_index) const;

	/**
	 * @brief Get the highest amount of caught mobiles (by route)
	 */
	unsigned bestInterceptionCount() const;

	/**
	 * @brief Get the number of caught mobiles (all routes)
	 */
	unsigned totalInterceptionCount() const;
	/**
	 * @brief Get first route index.
	 * @return First route index (interceptor)
	 */
	int first() const;
	/**
	 * @brief Get last route index.
	 * @return last route index (interceptor)
	 */
	int last() const;

	/**
	 * @brief Get the first interception info for the given route.
	 * @param i Interceptor attached to the route
	 * @return First interception info
	 */
	const InterceptionNode & firstOfRoute(const Interceptor & i) const;

	/**
	 * @brief Get the last interception info for the given route.
	 * @param i Interceptor attached to the route
	 * @return Last interception info
	 */
	const InterceptionNode & lastOfRoute(const Interceptor & i) const;

	/**
	 * @brief Subscript operator to get a route.
	 * @param i route index (or interceptor index)
	 * @return route
	 */
	RouteNode operator[] (unsigned);
	/**
	 * @brief Subscript operator to get a route (const).
	 * @param i route index (or interceptor index)
	 * @return route
	 */
	const RouteNode operator[] (unsigned) const;

	/**
	 * @brief Check if a mobile is intercepted.
	 * @param m mobile
	 * @return true if the given mobile has been intercepted
	 */
	bool isCaught (const Mobile &) const;
	/**
	 * @brief Get the interception position of a mobile.
	 * @param m mobile
	 * @return interception position
	 * @warning answer is right if the mobile has been intercepted
	 */
	Location catchPosition (const Mobile &) const;

	/**
	 * @brief Get interception info on a mobile.
	 * @param i mobile index
	 * @return interception parameters
	 */
	InterceptionNode & mobile(unsigned);
	/**
	 * @brief Get interception info on a mobile (const).
	 * @param i mobile index
	 * @return interception parameters
	 */
	const InterceptionNode & mobile(unsigned) const;

	/**
	 * @brief Get all the mobiles that have not been caught.
	 * @return A vector of const pointers to all the uncaught mobiles in this solution
	 */
	std::vector<const Mobile *> uncaughtMobiles() const;

	/**
	 * @brief Get an iterator to the first intercepted mobile of a route.
	 * @param i interceptor that performed the route
	 * @return begin iterator
	 */
	Solution::iterator begin(const Interceptor & i);

	/**
	 * @brief Get an iterator to the end of a route (after the last mobile).
	 * @param i interceptor that performed the route
	 * @return end iterator
	 */
	Solution::iterator end(const Interceptor & i);

	/**
	 * @brief Get a const iterator to the first intercepted mobile of a route.
	 * @param i interceptor that performed the route
	 * @return begin const_iterator
	 */
	Solution::const_iterator begin(const Interceptor & i) const;

	/**
	 * @brief Get a const iterator to the end of a route (after the last mobile).
	 * @param i interceptor that performed the route
	 * @return end const iterator
	 */
	Solution::const_iterator end(const Interceptor & i) const;
	/**
	 * @brief Check if a route is empty.
	 * @param i interceptor that performed the route
	 * @return true if route is empty, false otherwise
	 */
	bool isEmpty(const Interceptor &) const;

	/**
	 * @brief Check solution integrity
	 * @return true when current solution is consistent.
	 */
	bool check() const;

	/**
	 * @brief Operator less than
	 * @param other compared solution
	 * @return true when solution is better in terms of Time
	 */
	bool operator< (const Solution & other) const;
};

/**
 * @brief Appends to the stream the string representation of the Solution
 * @param o output stream
 * @param s solution to be appended to o
 * @return stream to chain calls
 */
std::ostream & operator<< (std::ostream &, const Solution &);

#endif
