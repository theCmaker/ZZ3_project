/**
 * @file vnd.hpp
 * @brief Vertical Neighbourhood Descent on Solutions.
 */

#ifndef __VND_HPP__
#define __VND_HPP__

#include <movements/move.hpp>
#include <vector>

/**
 * @brief Vertical Neighbourhood Descent Algorithm.
 * @tparam max_itr maximum number of iterations (Default 30)
 */
template <unsigned max_itr = 30>
class VND
{
	std::vector<Move *> _list;
	bool _donotclean;
public:
	/**
	 * @brief Constructor
	 */
	VND();
	/**
	 * @brief Constructor
	 * @param list Movement list
	 */
	VND(const std::vector<Move *> & list);
	/**
	 * @brief Destructor
	 */
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
