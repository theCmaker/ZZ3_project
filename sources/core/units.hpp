/**
 * @file units.hpp
 * @brief Definitions of simple units and structures.
 */
#ifndef __UNITS_HPP__
#define __UNITS_HPP__

#include <iostream>
/**
 * @brief Speed unit
 */
typedef double Speed;

/**
 * @brief Time unit
 */
typedef double Time;

/**
 * @brief Distance unit
 */
typedef double Distance;

//******************************************************
// Location
//******************************************************

/**
 * @brief Describes a location (in 2D cartesian coordinates)
 */
struct Location
{
	Distance _x;	///< Abscissa value (distance from origin)
	Distance _y;	///< Ordinate value (distance from origin)

	/**
	 * @brief Constructor.
	 * @param x Abscissa
	 * @param y Ordinate
	 */
	inline Location(Distance x = 0., Distance y = 0.) :
		_x(x),
		_y(y)
	{}

	/**
	 * @brief Equality test with optional precision parameter.
	 * @param other Other location to compare
	 * @param epsilon Precision
	 * @return true when locations are the same at more or minus epsilon
	 */
	inline bool equals(const Location & other, Distance epsilon = 0.) const
	{
		return (other._x - epsilon) <= _x
				&& (other._x + epsilon) >= _x
				&& (other._y - epsilon) <= _y
				&& (other._y + epsilon) >= _y;
	}
};

/**
 * @brief Appends to the stream the string representation of the Location.
 * @param o output stream
 * @param l location to be appended to o
 * @return stream to chain calls
 */
std::ostream & operator<< (std::ostream &, const Location &);

/**
 * @brief Equality operator between two locations.
 * @param l1 first location
 * @param l2 second location
 * @return true when l1 and l2 are exactly equal (i.e. precision = 0.)
 */
bool operator== (const Location &, const Location &);


//******************************************************
// Direction
//******************************************************

/**
 * @brief Describes a direction (movement direction in 2D cartesian system).
 */
struct Direction
{
	Speed _sx;	///< Abscissa speed
	Speed _sy;	///< Ordinate speed

	/**
	 * @brief Constructor
	 * @param sx abscissa speed
	 * @param sy ordinate speed
	 */
	Direction(Speed sx = 0., Speed sy = 0.);
};

/**
 * @brief Appends to the stream the string representation of the Direction.
 * @param o output stream
 * @param d direction to be appended to o
 * @return stream to chain calls
 */
std::ostream & operator<< (std::ostream &, const Direction &);

/**
 * @brief Equality operator between two directions.
 * @param d1 first direction
 * @param d2 second direction
 * @return true when d1 and d2 are equal
 */
bool operator== (const Direction &, const Direction &);

#endif
