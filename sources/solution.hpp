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
	struct MobileNode
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
		 */
		MobileNode(const Mobile &, const Time, const Interceptor *);
		/**
		 * @brief Default constructor.
		 * @param m the mobile
		 * @warning a mobile must be provided.
		 * @note _date defaults to -1.
		 */
		MobileNode(const Mobile &);
	};

	/**
	 * @brief Internal structure describing a route
	 * (interceptor, first and last mobile).
	 */
	struct InterceptorNode
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
		 * @warning an interceptor must be provided.
		 */
		InterceptorNode(const Interceptor &);
	};

	/**
	 * @brief Provide iterator over mobile nodes.
	 */
	class iterator
	{
	private:
		MobileNode * _solution; ///< Pointer to the current interception
	public:
		/**
		 * @brief Constructor.
		 * @param s the interception node
		 */
		iterator(MobileNode *);
		/**
		 * @brief Destructor.
		 */
		~iterator();
		/**
		 * @brief Indirection operator.
		 * @return Current interception
		 */
		MobileNode & operator* ();
		/**
		 * @brief Arrow operator.
		 * @return Pointer to current interception
		 */
		MobileNode * operator-> ();
		/**
		 * @brief Increment operator.
		 * @return Next interception node
		 */
		iterator & operator++ ();
		/**
		 * @brief Inequality operator.
		 * @param itr another iterator.
		 * @return true if this equals the given iterator.
		 */
		bool operator!= (iterator);
	};
	/**
	 * @brief Provide const_iterator over mobile nodes.
	 */
	class const_iterator
	{
	private:
		const MobileNode * _solution; ///< Pointer to the current interception
	public:
		/**
		 * @brief Constructor.
		 * @param s the interception node
		 */
		const_iterator(const MobileNode *);
		/**
		 * @brief Destructor.
		 */
		~const_iterator();
		/**
		 * @brief Indirection operator.
		 * @return Current interception
		 */
		const MobileNode & operator* () const;
		/**
		 * @brief Arrow operator.
		 * @return Pointer to current interception
		 */
		const MobileNode * operator-> () const;
		/**
		 * @brief Increment operator.
		 * @return Next interception node
		 */
		const_iterator & operator++ ();
		/**
		 * @brief Inequality operator.
		 * @param itr another iterator.
		 * @return true if this equals the given iterator.
		 */
		bool operator!= (const_iterator);
	};
private:
	const Problem				  & _problem;		///< Routing problem
	std::vector<MobileNode>			_sequence;		///< Interceptions
	std::vector<InterceptorNode>	_interceptors;	///< Routes

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
	 * @brief Destructor.
	 */
	~Solution();

	/**
	 * @brief Add an interception at the end of a route.
	 * @param interceptor_index id of the interceptor (route)
	 * @param mobile_index id of the intercepted mobile
	 * @param d interception date
	 */
	void append(unsigned interceptor_index, unsigned mobile_index, const Time & d);
	/**
	 * @brief Add an interception at the end of a route.
	 *
	 * Means mobile m has been caught by interceptor i at date d.
	 * @param i interceptor
	 * @param m intercepted mobile
	 * @param d interception date
	 */
	void append(const Interceptor & i, const Mobile & m, const Time & d);

	/**
	 * @brief Add an interception at the beginning of a route.
	 * @param interceptor_index id of the interceptor (route)
	 * @param mobile_index id of the intercepted mobile
	 * @param d interception date
	 */
	void prepend(unsigned interceptor_index, unsigned mobile_index, const Time & d);
	/**
	 * @brief Add an interception at the beginning of a route.
	 *
	 * Means mobile m has been caught by interceptor i at date d.
	 * @param i interceptor
	 * @param m intercepted mobile
	 * @param d interception date
	 */
	void prepend(const Interceptor & i, const Mobile & m, const Time & d);

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
	 * @brief Add an interception after a mobile in a route.
	 *
	 * Means mobile m has been caught by interceptor i at date d, after m_prev.
	 * @param m_prev mobile before the intercepted mobile
	 * @param i interceptor
	 * @param m intercepted mobile
	 * @param d interception date
	 */
	void insertAfter(const Mobile & m_prev, const Interceptor & i, const Mobile & m, const Time & d);

	/**
	 * @brief Remove a mobile from its route.
	 * @param m mobile to be removed
	 */
	void remove(const Mobile & m);

	/**
	 * @brief Remove a mobile from its route.
	 * @param mobile_index index of the mobile to be removed
	 */
	void remove(unsigned mobile_index);

	/**
	 * @brief Recompute the interception dates in a route, starting from the given mobile index.
	 * @param mobile_index index of the last mobile with a valid interception time
	 * @return last interception date for the recomputed route
	 */
	Time recomputeFrom(unsigned mobile_index);
	/**
	 * @brief Recompute the interception dates in a route, starting from the given mobile.
	 * @param m last mobile with a valid interception time
	 * @return last interception date for the recomputed route
	 */
	Time recomputeFrom(const Mobile & m);

	/**
	 * @brief Get the last interception date for all routes.
	 * @return The last interception date for all routes
	 */
	Time worstInterceptionTime() const;
	/**
	 * @brief Get the last interception date for the given interceptor (route) id.
	 * @param interceptor_index the route or interceptor id.
	 * @return last interception date for interceptor id interceptor_index
	 */
	Time lastInterceptionTime(int interceptor_index) const;
	/**
	 * @brief Get the last interception date for the given interceptor.
	 * @param i interceptor
	 * @return last interception date for interceptor i
	 */
	Time lastInterceptionTime(const Interceptor & i) const;

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
	 * @brief Subscript operator to get a route.
	 * @param i route index (or interceptor index)
	 * @return route
	 */
	InterceptorNode operator[] (unsigned);
	/**
	 * @brief Subscript operator to get a route (const).
	 * @param i route index (or interceptor index)
	 * @return route
	 */
	const InterceptorNode operator[] (unsigned) const;

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
	MobileNode & mobile(unsigned);
	/**
	 * @brief Get interception info on a mobile (const).
	 * @param i mobile index
	 * @return interception parameters
	 */
	const MobileNode & mobile(unsigned) const;

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
};

/**
 * @brief Appends to the stream the string representation of the Solution
 * @param o output stream
 * @param s solution to be appended to o
 * @return stream to chain calls
 */
std::ostream & operator<< (std::ostream &, const Solution &);

#endif
