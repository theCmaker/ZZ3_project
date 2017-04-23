#ifndef __PARETOFRONTSOLVER_H__
#define __PARETOFRONTSOLVER_H__

/*!
 * \file paretofrontsolver.h
 * \brief Set of solutions read from a file, has Pareto front processing abilities
 * \author Pierre-Loup Pissavy
 */

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <map>
#include "paretofront.h"

template <typename POINT>
/*!
 * \brief Set of solutions and pareto frontiers on these solutions
 */
class ParetoFrontSolver
{
public:
	typedef ParetoFront<POINT> pareto_front;
	typedef std::vector<pareto_front> ParetoFrontv;
	typedef std::vector<POINT> FPointv;
	typedef std::vector<POINT *> FPointPtrv;
	typedef std::multimap<float,POINT *> FPointPtrMap;
	typedef std::map<float,FPointPtrMap > FPointPtrMMap;
protected:
	FPointv         pts_;           //!< Points read in data file
	FPointPtrMMap   pts_map_;       //!< Points in a sorted fashion
	unsigned        nb_pts_;        //!< Number of points
	float           x_min_;         //!< Lowest x coordinate
	float           x_max_;         //!< Highest x coordinate
	float           y_min_;         //!< Lowest y coordinate
	float           y_max_;         //!< Highest y coordinate
	std::string     abscissa_;      //!< Name of the x axis
	std::string     ordinate_;      //!< Name of the y axis
	std::string     filename_;      //!< File used to get the data
	ParetoFrontv    pFrontiers_;    //!< Pareto frontiers

public:
	ParetoFrontSolver();
//	ParetoFrontSolver(const char *);
	~ParetoFrontSolver();

	const float & getMaxX() const;
	const float & getMaxY() const;
	const float & getMinX() const;
	const float & getMinY() const;
	const std::string & getAbscissa() const;
	const std::string & getOrdinate() const;
	const std::string & getFilename() const;
	const unsigned      &  getNbPts            ()                      const;
	const FPointv       &  getPts              ()                      const;
	const FPointPtrMMap &  getPtsMap           ()                      const;
	ParetoFrontv  &  getPFrontiers       ();
	void             setPts              (FPointv &);
	FPointPtrv       findPointsInArea    (POINT &, POINT &)    const;
	void             compute_frontiers   ();
	void             compute_inclusive_frontiers();
	void             saveToFile          (const char *)          const;
};

#include "paretofrontsolver.cxx"

#endif /* end of include guard: __PARETOFRONTSOLVER_H__ */
