/**
 * @file depot.hpp
 * @brief Implementation of a Depot.
 */
#ifndef __DEPOT_HPP__
#define __DEPOT_HPP__

#include "units.hpp"
#include "interceptor.hpp"
#include <vector>

/**
 * @brief Describe a depot i.e. a fixed place an interceptor belongs to.
 */
class Depot
{
	public:
		typedef std::vector<const Interceptor*> VInterceptorPtrs; ///< Interceptor pointers vector
	private:
		unsigned			_id;			///< The id of the interceptor
		Location			_position;		///< Position
		VInterceptorPtrs	_interceptors;	///< Attached interceptors

	public:
		/**
		 * @brief Constructor.
		 * @param id Depot id
		 */
		Depot(unsigned id = 0);
		/**
		 * @brief Constructor.
		 * @param x Depot x position
		 * @param y Depot y position
		 * @param id Depot id
		 */
		Depot(Distance x, Distance y, unsigned id = 0);
		/**
		 * @brief Constructor.
		 * @param l Depot position
		 * @param id Depot id
		 */
		Depot(Location & l, unsigned id = 0);
		/**
		 * @brief Destructor.
		 */
		~Depot();

		//Getters
		/**
		 * @brief Get the depot id.
		 */
		const unsigned & id() const;
		/**
		 * @brief Get the depot position.
		 */
		const Location & position() const;
		/**
		 * @brief Get the attached interceptor pointers list.
		 */
		const VInterceptorPtrs & interceptors() const;

		//Setters
		/**
		 * @brief Define the position
		 */
		Depot & position(const Location &);

		//Methods
		/**
		 * @brief Add an Interceptor
		 */
		Depot & addInterceptor(const Interceptor &);
};

/**
 * @brief Display the values of a depot.
 * @param o the output stream
 * @param d the depot
 * @return the output stream (chained instructions)
 */
std::ostream & operator<< (std::ostream & o, const Depot & d);

#endif
