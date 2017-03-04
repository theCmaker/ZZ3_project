#ifndef __FRONT_H__
#define __FRONT_H__

/*!
 * \file pfront.h
 * \brief Pareto Front data
 * \author Pierre-Loup Pissavy
 */

#include "datapoint.h"
#include <vector>

/*!
 * \brief Pareto Front
 */
template <typename PT_DATATYPE>
class ParetoFront
{
public:
	typedef PT_DATATYPE POINT;
	typedef std::vector<POINT *> PolyLine;

	ParetoFront();
	~ParetoFront();

	PolyLine & pts();
	const PolyLine & pts() const;
	void setPts(const PolyLine &pts);

	typename PolyLine::iterator begin();
	typename PolyLine::iterator end();

	double hypervolumen() const;
	void setHypervolumen(double hypervolumen);

	double max_spacing() const;
	void setMax_spacing(double max_spacing);

	double length() const;
	void setLength(double length);

	double mean_spacing() const;
	void setMean_spacing(double mean_spacing);

	void compute_stats(double xmin, double xmax, double ymin, double ymax);

private:
	PolyLine    pts_;           //!< the point(s) that make(s) the front
	double      hypervolumen_;  //!< hypervolumen value
	double      mean_spacing_;  //!< mean spacing value
	double      max_spacing_;   //!< max spacing value
	double      length_;        //!< length of the front
};


#include "paretofront.cxx"

#endif // __FRONT_H__
