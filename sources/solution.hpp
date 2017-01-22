/**
 * @file solution.hpp
 * @brief Implementation of a solution. Gears the interceptions.
 */

#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__

#include <list>
#include "problem.hpp"
/**
 * @brief Describes an interception sequence, i.e. the order mobiles are
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

		//Constructors
		/**
		 * @brief Constructor.
		 * @param i the interceptor that realizes the route
		 * @warning an interceptor must be provided.
		 */
		InterceptorNode(const Interceptor &);
	};

	/**
	 * @brief Provides iterator over mobile nodes.
	 */
	class iterator
	{
	private:
		const MobileNode * _solution; ///< Pointer to the current interception
	public:
		/**
		 * @brief Constructor.
		 * @param s the interception node
		 */
		iterator(const MobileNode *);
		/**
		 * @brief Destructor.
		 */
		~iterator();
		/**
		 * @brief Indirection operator.
		 * @return Current interception
		 */
		MobileNode operator* ();
		/**
		 * @brief Arrow operator.
		 * @return Pointer to current interception
		 */
		const MobileNode * operator-> ();
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
	 * @brief Adds an interception at the end of a route.
	 * @param interceptor_index id of the interceptor (route)
	 * @param mobile_index id of the intercepted mobile
	 * @param d interception date
	 */
	void append(unsigned, unsigned, const Time &);
	/**
	 * @brief Adds an interception at the end of a route.
	 *
	 * Means mobile m has been caught by interceptor i at date d.
	 * @param i interceptor
	 * @param m intercepted mobile
	 * @param d interception date
	 */
	void append(const Interceptor &, const Mobile &, const Time &);

	/**
	 * @brief Get the last interception date for all routes.
	 * @return The last interception date for all routes
	 */
	Time worst_interception_time() const;
	/**
	 * @brief Get the last interception date for the given interceptor (route) id.
	 * @param interceptor_index the route or interceptor id.
	 * @return last interception date for interceptor id interceptor_index
	 */
	Time last_interception_time(int) const;
	/**
	 * @brief Get the last interception date for the given interceptor.
	 * @param i interceptor
	 * @return last interception date for interceptor i
	 */
	Time last_interception_time(const Interceptor &) const;

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
	bool is_caught (const Mobile &) const;
	/**
	 * @brief Get the interception position of a mobile.
	 * @param m mobile
	 * @return interception position
	 * @warning answer is right if the mobile has been intercepted
	 */
	Location catch_position (const Mobile &) const;

	/**
	 * @brief Get interception info on a mobile.
	 * @param i mobile index
	 * @return interception parameters
	 */
	MobileNode mobile(unsigned);
	/**
	 * @brief Get interception info on a mobile (const).
	 * @param i mobile index
	 * @return interception parameters
	 */
	const MobileNode mobile(unsigned) const;

	/**
	 * @brief Get an iterator to the first intercepted mobile of a route.
	 * @param i interceptor that performed the route
	 * @return begin iterator
	 */
	Solution::iterator begin(const Interceptor &) const;

	/**
	 * @brief Get an iterator to the end of a route (after the last mobile).
	 * @param i interceptor that performed the route
	 * @return end iterator
	 */
	Solution::iterator end(const Interceptor &) const;
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
