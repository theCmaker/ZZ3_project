/**
 *	@file ms_els.hpp
 *	@brief Multi-Start - Evolutionary Local Search Algorithm
 */
#ifndef __MS_ELS_HPP__
#define __MS_ELS_HPP__

#include <vector>

template <unsigned max_it_ms,
		  unsigned max_it_els,
		  unsigned max_cp,
		  class LS,
		  class PROBLEM,
		  class SOLUTION,
		  class CMP = std::less<SOLUTION> >
/**
 * @brief The MS_ELS class
 * @tparam max_it_ms Multi-Start iterations number
 * @tparam max_it_els ELS iterations number
 * @tparam max_cp number of copies
 * @tparam LS Local Search Algorithm Class
 * @tparam PROBLEM Problem Data Class
 * @tparam SOLUTION Solution Class
 * @tparam CMP Solution comparator (Default std::less<SOLUTION>)
 */
class MS_ELS
{
public:
	/**
	 * @brief Constructor
	 */
	MS_ELS();

	/**
	 * @brief Destructor
	 */
	~MS_ELS();

	/**
	 * @brief Execute the MS_ELS
	 * @param pb Source Problem
	 * @return Solution to the given problem
	 */
	SOLUTION operator() (const PROBLEM & pb);
};

#include "ms_els.cxx"
#endif
