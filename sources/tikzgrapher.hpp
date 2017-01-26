/**
 * @file tikzgrapher.hpp
 * @brief Utility to draw graphs from Solutions (Time/Caught mobiles)
 */

#ifndef __TIKZGRAPHER_HPP__
#define __TIKZGRAPHER_HPP__

#include "tikzifyer.hpp"

/**
 * @brief Draw a temporal graph for one or more solutions
 */
class TikzGrapher : public Tikzifyer
{
public:
	/**
	 * @brief TikzGrapher constructor.
	 */
	TikzGrapher();

	/**
	 * @brief Add a solution.
	 * @param s solution
	 *
	 * @note Overrides the default behaviour (see Tikzifyer)
	 * @warning We don't make a copy of the solution, so s must be available till the death of this object.
	 */
	void operator() (const Solution & s);
	/**
	 * @brief Add a problem.
	 * @param p problem
	 *
	 * @note Overrides the default behaviour (see Tikzifyer)
	 * @warning We don't make a copy of the problem, so p must be available till the death of this object.
	 */
	void operator() (const Problem & p);

	/**
	 * @brief Get the style string for a route / solution.
	 * @param index route or solution index
	 * @return Tikz line style parameters string
	 */
	static const std::string & style(unsigned index);
	/**
	 * @brief Add a new style.
	 * @param style Tikz line style parameters string
	 */
	static void addStyle(const std::string & style);

private:
	// Line styles
	static std::vector<std::string> _styles; ///< Line styles (Tikz line style strings)
};

/**
 * @brief Append a temporal graph to a stream.
 * @param o the output stream
 * @param t the temporal graph
 * @return the output stream (chained instructions)
 */
std::ostream & operator<< (std::ostream & o, const TikzGrapher & t);

#endif // TIKZGRAPHER_HPP
