/**
 * @file problem.hpp
 * @brief Implementation of a Problem.
 */

#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__

#include <vector>
#include <iostream>
#include "interceptor.hpp"
#include "mobile.hpp"
#include "depot.hpp"

/** @brief Interceptors vector */
typedef std::vector<Interceptor> VInterceptors;

/** @brief Mobiles vector */
typedef std::vector<Mobile> VMobiles;

/** @brief Depots vector */
typedef std::vector<Depot> VDepots;

/**
 * @brief Describe a problem (data).
 */
class Problem
{
	private:
		VInterceptors	_interceptors;	///< Interceptors
		VMobiles		_mobiles;		///< Mobiles
		VDepots			_depots;		///< Depots
	public:
		/**
		 * @brief Constructor.
		 */
		Problem();

		/**
		 * @brief Constructor. (Randomized)
		 * @param nb_interceptors Number of interceptors
		 * @param nb_mobiles Number of mobiles
		 * @param nb_depots Number of depots
		 */
		Problem(unsigned nb_interceptors, unsigned nb_mobiles, unsigned nb_depots, bool=false);
		/**
		 * @brief Constructor.
		 * @param filename Input file name
		 */
		Problem(const char * filename);
		/**
		 * @brief Copy constructor.
		 * @param p Other problem
		 */
		Problem(Problem & p);
		/**
		 * @brief Destructor.
		 */
		~Problem();

		//Getters
		/**
		 * @brief Get the number of interceptors.
		 */
		unsigned nbInterceptors() const;
		/**
		 * @brief Get the number of mobiles.
		 */
		unsigned nbMobiles() const;
		/**
		 * @brief Get the number of depots.
		 */
		unsigned nbDepots() const;

		/**
		 * @brief Get the mobiles.
		 */
		const VMobiles & mobiles() const;
		/**
		 * @brief Get the interceptors.
		 */
		const VInterceptors & interceptors() const;
		/**
		 * @brief Get the depots.
		 */
		const VDepots & depots() const;

		/**
		 * @brief Write the data into a file.
		 * @param filename output file name
		 */
		void write(const char *) const;
};

/**
 * @brief Append a problem to a stream.
 * @param o the output stream
 * @param p the problem
 * @return the output stream (chained instructions)
 */
std::ostream & operator<< (std::ostream &, const Problem &);

#endif
