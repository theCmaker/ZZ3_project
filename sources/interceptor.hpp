/**
 * @file interceptor.hpp
 * @brief Implementation of an Interceptor.
 */

#ifndef __INTERCEPTOR_HPP__
#define __INTERCEPTOR_HPP__

#include "units.hpp"
#include "mobile.hpp"
#include <iostream>
#include <limits>

class Depot;

/**
 * @brief Describes an interceptor i.e. a mobile unit with direction change
 * abilities.
 */
class Interceptor
{
	private:
		unsigned	_id;		///< The id of the interceptor
		Location	_position;	///< Start position
		Speed		_speed;		///< Velocity (fixed)
		Time		_range;		///< Interceptor range (in time units)
		const Depot *_depot;	///< Attached repository
		
		/**
		 * @brief Compute the angle to change the direction.
		 * @return The angle alpha.
		 */
		static double computeAlpha(double a, double b, double c);


	public:
		static constexpr double INTERCEPTION_TIME_NO_FUEL = std::numeric_limits<Time>::infinity(); ///< Interception date when tank becomes empty

		//Constructors
		/**
		 * @brief Default constructor
		 * @param id The id of the interceptor.
		 */
		Interceptor(unsigned = 0);
		
		/**
		 * @brief Constructor
		 * @param l Start position.
		 * @param s Velocity.
		 * @param id The id of the interceptor.
		 */
		Interceptor(Location &, Speed, unsigned = 0);
		
		/**
		 * @brief Constructor
		 * @param x x value of the start position (@see Location).
		 * @param y y value of the start position (@see Location).
		 * @param s Velocity.
		 * @param id The id of the interceptor.
		 */
		Interceptor(Distance, Distance, Speed, unsigned = 0);
		
		/**
		 * @brief Constructor
		 * @param d Attached repository.
		 * @param s Velocity.
		 * @param id The id of the interceptor.
		 */
		Interceptor(const Depot &, Speed, unsigned = 0);
		
		/**
		 * @brief Destructor
		 */
		~Interceptor();

		// Getters
		/**
		 * @brief Return the id.
		 */
		const unsigned & id() 		const;
		
		/**
		 * @brief Return the position.
		 */
		const Location & position()	const;
		
		/**
		 * @brief Return the velocity.
		 */
			  Speed		 speed()	const;
			  
		/**
		 * @brief Return the repository.
		 */
		const Depot	   * depot()	const;

		/**
		 * @brief Return the range
		 */
			  Time		 range() const;

		// Setters
		/**
		 * @brief Modify the position.
		 * @param l New position.
		 * @return A reference to the interceptor to chain calls.
		 */
		Interceptor &	position(const Location &);
		
		/**
		 * @brief Modify the velocity.
		 * @param l New velocity.
		 * @return A reference to the interceptor to chain calls.
		 */
		Interceptor &	speed(const Speed);

		/**
		 * @brief Modify the range.
		 * @param range New range.
		 * @return A reference to the interceptor to chain calls.
		 */
		Interceptor &	range(Time range);

		// Methods
		/**
		 * @brief Compute the position of the interceptor at a given location.
		 * @param alpha Angle for the direction.
		 * @param pos Current position. Updated by the method.
		 * @param t Travelling duration.
		 */
		void computePosition(double alpha, Location & pos, Time t)	const;
		
		/**
		 * @brief Compute the date when the mobile is catched.
		 * @param position Current position of the interceptor.
		 * @param m The mobile to catch.
		 * @param t Date before the interception.
		 * @return The needed time for the interception.
		 */
		Time computeInterception(Location, const Mobile &, Time)	const;

};

/**
 * @brief Display the values of an interceptor.
 * @param o the output stream
 * @param i the mobile
 * @return the output stream (chained instructions)
 */
std::ostream & operator << (std::ostream & o, const Interceptor & i);

#endif
