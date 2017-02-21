/**
 * @file mobile.hpp
 * @brief Implementation of a Mobile.
 */
#ifndef __MOBILE_H__
#define __MOBILE_H__

#include "units.hpp"
#include <iostream>

/**
 * @brief Describe a mobile i.e. a mobile unit running at  constant speed into a defined direction.
 */
class Mobile
{
	private:
		unsigned 	_id;		///< The id of the mobile
		Location	_position;	///< The initial position of a mobile
		Direction	_direction;	///< The moving direction

	public:
		/**
		 * @brief Constructor.
		 * @param id Mobile id
		 */
		Mobile(unsigned id = 0);
		/**
		 * @brief Constructor.
		 * @param l Mobile initial position
		 * @param d Mobile direction
		 * @param id Mobile id
		 */
		Mobile(Location & l, Direction & d, unsigned id = 0);
		/**
		 * @brief Constructor.
		 * @param x Initial X position
		 * @param y Initial Y position
		 * @param dx X speed
		 * @param dy Y speed
		 * @param id Mobile id
		 */
		Mobile(Distance x, Distance y, Distance dx, Distance dy, unsigned id = 0);
		/**
		 * @brief Destructor.
		 */
		~Mobile();

		//Getters
		/**
		 * @brief Get the mobile id.
		 */
		const unsigned & id() const;
		/**
		 * @brief Get the initial position.
		 */
		const Location 	& position() const;
		/**
		 * @brief Get the mobile position at a defined date.
		 * @param t Date
		 */
		inline Location position(Time t) const
		{
			return Location(_position._x + _direction._sx * t, _position._y + _direction._sy * t);
		}
		/**
		 * @brief Get the mobile direction.
		 */
		const Direction & direction() const;
		/**
		 * @brief Get the mobile speed.
		 */
			  Speed		  speed() const;

		//Setters
		/**
		 * @brief Define the mobile initial position
		 * @param p initial position
		 */
		Mobile & position(const Location & p);
		/**
		 * @brief Define the mobile direction
		 * @param d direction
		 */
		Mobile & direction(const Direction & d);
};

/**
 * @brief Display the values of a mobile.
 * @param o the output stream
 * @param m the mobile
 * @return the output stream (chained instructions)
 */
std::ostream & operator<< (std::ostream & o, const Mobile & m);
#endif
