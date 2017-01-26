/**
 * @file tikzifyer.hpp
 * @brief Utility to draw graphs from Solutions (Interception positions)
 */

#ifndef __TIKZIFYER_HPP__
#define __TIKZIFYER_HPP__
#include "solution.hpp"

/**
 * @brief Solution pointers vector.
 */
typedef std::vector<const Solution *> VPSolutions;

/**
 * @brief Draw a spatial graph for one or more solutions.
 */
class Tikzifyer
{
public:
	/**
	 * @brief Tikzifyer constructor.
	 */
	Tikzifyer();
	/**
	 * @brief Tikzifyer destructor.
	 */
	~Tikzifyer();

	/**
	 * @brief Add a solution.
	 * @param s solution
	 */
	void operator() (const Solution & s);
	/**
	 * @brief Add a problem.
	 * @param p problem
	 */
	void operator() (const Problem & p);

	/**
	 * @brief Get the associated Problem.
	 * @return the associated Problem
	 */
	const Problem & problem() const;
	/**
	 * @brief Get the solutions to be drawn.
	 * @return a Solution pointers vector
	 */
	const VPSolutions & solutions() const;

	/**
	 * @brief Get the mobiles associated to the problem.
	 * @return a Mobiles vector
	 */
	const VMobiles & mobiles() const;
	/**
	 * @brief Get the interceptors associated to the problem.
	 * @return an Interceptors vector
	 */
	const VInterceptors & interceptors() const;

	/**
	 * @brief Get the lowest x bound.
	 * @return lowest x bound
	 */
	Distance xmin() const;
	/**
	 * @brief Get the highest x bound.
	 * @return highest x bound
	 */
	Distance xmax() const;
	/**
	 * @brief Get the lowest y bound.
	 * @return lowest y bound
	 */
	Distance ymin() const;
	/**
	 * @brief Get the highest y bound.
	 * @return highest y bound
	 */
	Distance ymax() const;

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

	/**
	 * @brief Clear the object (problem, solution, bounds).
	 */
	void clear();

protected:
	// Viewport Bounds
	Location _top_left;					///< Top left corner coordinates
	Location _bottom_right;				///< Bottom right corner coordinates
	/**
	 * @brief Add a point into the viewport
	 * @param l point coordinates
	 */
	void addToBounds(const Location & l);

	// Data
	const Problem * _problem;			///< Problem data
	VPSolutions _solutions;				///< Solutions added, relative to the problem

	// Line styles
	static std::vector<std::string> _styles; ///< Line styles (Tikz line style strings)
};

/**
 * @brief Append a spatial graph to a stream.
 * @param o the output stream
 * @param t the spatial graph
 * @return the output stream (chained instructions)
 */
std::ostream & operator<< (std::ostream & o, const Tikzifyer & t);

#endif
