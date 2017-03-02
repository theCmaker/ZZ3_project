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
	VND(const std::vector<Move *> & list);
	~VND();

	/**
	 * @brief Run the VND on a Solution.
	 * @param sol Solution to be improved
	 */
	void run(Solution & sol);

	/**
	 * @brief Run the VND on a Solution.
	 * @param sol Solution to be improved
	 */
	void operator() (Solution & sol);

	/**
	 * @brief Get the movements list.
	 * @warning Returned list is editable.
	 */
	std::vector<Move *> & movements();

	/**
	 * @brief Do stuff before running VND.
	 * Initialize the Policies
	 * @param s Solution
	 */
	static void before(const Solution & s);

	/**
	 * @brief Do stuff after running VND
	 * @param s Solution
	 */
	static void after(const Solution & s);
};

#include "vnd.cxx"
#endif // __VND_HPP__
