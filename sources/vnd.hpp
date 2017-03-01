/**
 * @file vnd.hpp
 * @brief Metaheuristic VND to run the Local Search on solutions.
 */

#ifndef __VND_HPP__
#define __VND_HPP__

#include "move.hpp"
#include <vector>

template <unsigned max_itr = 30>
class VND
{
	std::vector<Move *> _list;
	bool _donotclean;
public:
	VND();
	VND(std::vector<Move *> & list);
	~VND();

	/**
	 * @brief Run the VND on a Solution.
	 * @param sol Solution to be improved
	 */
	void run(Solution & sol);

	/**
	 * @brief Get the movements list.
	 * @warning Returned list is editable.
	 */
	std::vector<Move *> & movements();
};

#include "vnd.cxx"
#endif // __VND_HPP__
