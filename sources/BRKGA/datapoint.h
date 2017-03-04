#ifndef __DATAPOINT_H__
#define __DATAPOINT_H__

#include <string>
#include <iostream>

/*!
 * \file datapoint.h
 * \brief 2D Point with id and info data
 * \author Pierre-Loup Pissavy
 */

template<typename T1, typename T2, typename INFO = std::string>
/*!
 * \brief 2D point with id and info data
 */
class Point {
private:
	T1 x_;               //!< First coordinate
	T2 y_;               //!< Second coordinate
	unsigned    id_;    //!< Id of the point
	INFO info_;			//!< Comment on the point

public:

	Point (T1=T1(), T2=T2(), INFO=INFO(),unsigned=0);
	Point (const Point<T1,T2,INFO> &);
	~Point ();

	//getters
	const T1          & getX    () const;
	const T2          & getY    () const;
	const unsigned    & getId   () const;
	const INFO		  & getInfo () const;

	//setters
	void setX       (T1 &);
	void setY       (T2 &);
	void setInfo    (std::string &);

	//interrogators
	bool isIn (Point<T1, T2, INFO> &,Point<T1, T2, INFO> &) const;

	//comparators
	bool dominates (Point<T1, T2, INFO> &);
	bool operator< (const Point<T1, T2, INFO> &) const;

	//transformers
	std::string toString() const;

};




template <typename T1, typename T2, typename INFO>
/*!
 * \brief operator <<
 * \param o output stream
 * \param p point to be displayed
 * \return the output stream
 */
std::ostream & operator<< (std::ostream & o, const Point<T1, T2, INFO> &p) {
	o << '(' << p.getX() << ',' << p.getY() << ") " << p.getInfo();
	return o;
}

//Constructors
template <typename T1, typename T2, typename INFO>
/*!
 * \brief Constructor
 * \param id ID
 * \param x First coordinate
 * \param y Second coordinate
 * \param s Comment
 */
Point<T1,T2,INFO>::Point (T1 x, T2 y, INFO s, unsigned id) :
	x_(x),
	y_(y),
	id_(id),
	info_(s)
{
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Copy constructor
 * \param p other point
 */
Point<T1,T2,INFO>::Point (const Point<T1, T2, INFO> &p) :
	x_(p.x_),
	y_(p.y_),
	id_(p.id_),
	info_(p.info_)
{
}

//Destructor
template <typename T1, typename T2, typename INFO>
/*!
 * \brief Destructor
 */
Point<T1,T2,INFO>::~Point () {}

//getters
template <typename T1, typename T2, typename INFO>
/*!
 * \brief Get first coordinate
 * \return first coordinate
 */
const T1 & Point<T1,T2,INFO>::getX () const {
	return x_;
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Get second coordinate
 * \return second coordinate
 */
const T2 & Point<T1,T2,INFO>::getY() const {
	return y_;
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Get id
 * \return id
 */
const unsigned & Point<T1,T2,INFO>::getId() const {
	return id_;
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Get comment
 * \return comment
 */
const INFO & Point<T1,T2,INFO>::getInfo() const {
	return info_;
}

//setters
template <typename T1, typename T2, typename INFO>
/*!
 * \brief Set first coordinate
 * \param x first coordinate
 */
void Point<T1,T2,INFO>::setX (T1 & x) {
	x_ = x;
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Set second coordinate
 * \param y second coordinate
 */
void Point<T1,T2,INFO>::setY (T2 & y) {
	y_ = y;
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Set comment
 * \param info comment
 */
void Point<T1,T2,INFO>::setInfo(std::string & info) {
	info_ = info;
}

//interrogators
template <typename T1, typename T2, typename INFO>
/*!
 * \brief Check if this point is in the rectangle defined by \a top_left and \a bottom_right
 * \param top_left Top left coordinates of the rectangle
 * \param bottom_right Bottom right coordinates of the rectangle
 * \return true if this point is in the rectangle, false otherwise
 */
inline bool Point<T1,T2,INFO>::isIn (Point<T1, T2, INFO> &top_left, Point<T1, T2, INFO> &bottom_right) const {
	return ((this->x_ >= top_left.x_) && (this->x_ <= bottom_right.x_) && (this->y_ <= top_left.y_) && (this->y_ >= bottom_right.y_));
}

//comparators
template <typename T1, typename T2, typename INFO>
/*!
 * \brief Check if this point dominates \a p
 * \param p other point
 * \return true if this point dominates \a p, false otherwise
 */
inline bool Point<T1,T2,INFO>::dominates (Point<T1, T2, INFO> &p) {
	return (this->x_ > p.x_ && this->y_ < p.y_);
}

template <typename T1, typename T2, typename INFO>
/*!
 * \brief Comparator
 * \param p other point
 * \return true if this point has lower first coordinate and higher or same second coordinate than \a p
 */
inline bool Point<T1,T2,INFO>::operator< (const Point<T1, T2, INFO> &p) const {
	return (this->x_ < p.x_ && this->y_ >= p.y_);
}

//transformers
template <typename T1, typename T2, typename INFO>
/*!
 * \brief String equivalent of this point
 * \return string formatted point
 */
std::string Point<T1,T2,INFO>::toString () const {
	char buf[128];
	sprintf(&buf[0],"(%f,%f) %s",(float) x_, (float) y_, info_.c_str());
	return std::string(buf);
}


#endif // __DATAPOINT_H__
