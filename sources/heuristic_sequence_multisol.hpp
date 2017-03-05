/**
 * @file heuristic_sequence_multisol.hpp
 * @brief Provide a set of successive solutions corresponding to a given sequence.
 */
#ifndef __HEURISTIC_SEQUENCE_MULTISOL_HPP__
#define __HEURISTIC_SEQUENCE_MULTISOL_HPP__

#include "heuristic.hpp"

/**
 *	@brief Sequence heuristic with successive states. The mobile catching order is defined by a
 * sequence. The interceptor to be chosen each time is the one that needs the
 * shortest time to intercept the mobile.
 * @see Heuristic_Sequence
 */
class Heuristic_Sequence_MultiSol : public Heuristic
{
	std::vector<Solution> _solutions;
public:
	/**
	 * @brief Constructor.
	 * @param p Problem data
	 */
	Heuristic_Sequence_MultiSol(const Problem & p);
	/**
	 * @brief Destructor.
	 */
	virtual ~Heuristic_Sequence_MultiSol();

	void run();
	void run(const std::vector<unsigned> &);

	/**
	 * @brief Get the solutions (mutable form).
	 * @return Successive solutions to the Problem
	 */
	std::vector<Solution> & solutions();

	/**
	 * @brief Get the solutions (immutable form).
	 * @return Successive solutions to the Problem
	 */
	const std::vector<Solution> & solutions() const;


};

#endif // __HEURISTIC_SEQUENCE_MULTISOL_HPP__
