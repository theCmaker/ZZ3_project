/*!
 * \brief Constructor
 *
 * Inits the front
 */
template <typename PT_DATATYPE>
ParetoFront<PT_DATATYPE>::ParetoFront() :
	hypervolumen_(0.),
	mean_spacing_(0.),
	max_spacing_(0.),
	length_(0.)
{}

/*!
 * \brief Destructor
 */
template <typename PT_DATATYPE>
ParetoFront<PT_DATATYPE>::~ParetoFront() {}

/*!
 * \brief points vector
 * \return the sorted points that compose the front
 * \note it is a vector of pointers to Points.
 */
template <typename PT_DATATYPE>
typename ParetoFront<PT_DATATYPE>::PolyLine & ParetoFront<PT_DATATYPE>::pts()
{
	return pts_;
}

/*!
 * \brief points vector
 * \return the sorted points that compose the front
 * \note it is a vector of pointers to Points.
 */
template <typename PT_DATATYPE>
const typename ParetoFront<PT_DATATYPE>::PolyLine & ParetoFront<PT_DATATYPE>::pts() const
{
	return pts_;
}

/*!
 * \brief updates the points
 * \param pts new points of the front
 * \note \a pts must be a vector of pointers to Points.
 */
template <typename PT_DATATYPE>
void ParetoFront<PT_DATATYPE>::setPts(const ParetoFront::PolyLine &pts)
{
	pts_ = pts;
}

/*!
 * \brief Iterator on the points vector
 * \return iterator on the points vector
 */
template <typename PT_DATATYPE>
typename ParetoFront<PT_DATATYPE>::PolyLine::iterator ParetoFront<PT_DATATYPE>::begin()
{
	return pts_.begin();
}

/*!
 * \brief End iterator on the points vector
 * \return end iterator of the points vector
 */
template <typename PT_DATATYPE>
typename ParetoFront<PT_DATATYPE>::PolyLine::iterator ParetoFront<PT_DATATYPE>::end()
{
	return pts_.end();
}

/*!
 * \brief Hypervolumen of the front
 * \return hypervolumen value
 * \warning compute_stats() must have been called before
 * \see compute_stats
 * \note The value is normalized. Between 0 and 1.
 */
template <typename PT_DATATYPE>
double ParetoFront<PT_DATATYPE>::hypervolumen() const
{
	return hypervolumen_;
}

/*!
 * \brief Max spacing of the front
 * \return max spacing value
 * \warning compute_stats() must have been called before
 * \see compute_stats
 * \note lengths are computed with taxicab geometry (Manhattan distance)
 */
template <typename PT_DATATYPE>
double ParetoFront<PT_DATATYPE>::max_spacing() const
{
	return max_spacing_;
}

/*!
 * \brief Length of the front
 * \return length value, i.e sum of all spacing values.
 * \note The value is normalized. Between 0 and 2.
 * \warning compute_stats() must have been called before
 * \note lengths are computed with taxicab geometry (Manhattan distance)
 * \see compute_stats
 */
template <typename PT_DATATYPE>
double ParetoFront<PT_DATATYPE>::length() const
{
	return length_;
}

/*!
 * \brief Mean spacing of the front
 * \return mean spacing value
 * \warning compute_stats() must have been called before
 * \note lengths are computed with taxicab geometry (Manhattan distance)
 * \see compute_stats
 */
template <typename PT_DATATYPE>
double ParetoFront<PT_DATATYPE>::mean_spacing() const
{
	return mean_spacing_;
}

/*!
 * \brief Compute all the stats of the front, i.e. hypervolumen, mean and max spacing, and length.
 * \param xmin lowest first objective value in the global graph
 * \param xmax highest first objective value in the global graph
 * \param ymin lowest second objective value in the global graph
 * \param ymax highest second objective value in the global graph
 * \note Parameters are provided to normalize the stats.
 * \note Lengths are computed with taxicab geometry (Manhattan distance)
 */
template <typename PT_DATATYPE>
void ParetoFront<PT_DATATYPE>::compute_stats(double xmin, double xmax, double ymin, double ymax)
{
	double delta_x = xmax - xmin;
	double delta_y = ymax - ymin;
	double dist_x;
	double dist_y;
	double sum_dist_x = 0.;
	double sum_dist_y = 0.;
	max_spacing_ = 0.;
	length_ = 0.;
	mean_spacing_ = 0.;
	hypervolumen_ = 0.;

	//Spacings
	if (pts_.size() > 1) {
		for (typename PolyLine::iterator itr = pts_.begin(); itr != pts_.end() - 1; ++itr) {
			dist_x = (*(itr+1))->getX() - (*itr)->getX();
			dist_y = (*itr)->getY() - (*(itr+1))->getY();
			sum_dist_x += dist_x;
			sum_dist_y += dist_y;
			//update the max_spacing_ if necessary
			if (dist_x/delta_x + dist_y/delta_y > max_spacing_) {
				max_spacing_ = dist_x/delta_x + dist_y/delta_y;
			}
		}
		length_ = sum_dist_x / delta_x + sum_dist_y / delta_y;
		mean_spacing_ = length_ / (double) (pts_.size() - 1);

	}

	//Hypervolumen
	hypervolumen_ = delta_x * delta_y;
	typename PolyLine::iterator i = pts_.begin();

	// Compute until penultimate point
	while (i != pts_.end()-1) {
		// Remove rectangle above current point till next point
		hypervolumen_ -= (double((*(i+1))->getX() - (*i)->getX())) * (ymax - double((*i)->getY()));
		++i;
	}

	// Last point -> rectangle computed with x_max_ as right side abscissa
	// (projection on line x = x_max_)
	hypervolumen_ -= (xmax - double((*i)->getX())) * (ymax - double((*i)->getY()));

	//Normalize
	hypervolumen_ = ((delta_x * delta_y) != 0) ? (hypervolumen_ / double(delta_x * delta_y)) : 0.;
}



