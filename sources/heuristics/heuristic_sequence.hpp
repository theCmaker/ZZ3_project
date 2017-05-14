/**
 * @file heuristic_sequence.hpp
 * @brief Provide a simple solution corresponding to a wanted sequence.
 */
#ifndef __HEURISTIC_SEQUENCE_HPP__
#define __HEURISTIC_SEQUENCE_HPP__

#include "heuristic.hpp"

/**
 *	@brief Sequence heuristic. The mobile catching order is defined by a
 * sequence. The interceptor to be chosen each time is the one that needs the
 * shortest time to intercept the mobile.
 */
class Heuristic_sequence : public Heuristic
{
public:
	/**
	 * @brief Constructor.
	 * @param p Problem data
	 */
	Heuristic_sequence(const Problem & p);
	/**
	 * @brief Destructor.
	 */
	virtual ~Heuristic_sequence();

	/**
	 * @brief Start the Heuristic
	 */
	void run();

	/**
	 * @brief Start the Heuristic (specific version)
	 * @param Id sequence
	 */
	void run(const std::vector<unsigned> &);

};

#endif
