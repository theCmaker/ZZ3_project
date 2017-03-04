
/*!
 * \brief Default solution constructor
 */
template <typename DATA>
ParetoFrontSolver<DATA>::ParetoFrontSolver() : nb_pts_(0) {}

/*!
 * \brief Solution constructor from a given file
 * \param name Name of the solutions description file
 */
//template <typename DATA>
//ParetoFrontSolver<DATA>::ParetoFrontSolver(const char *name) :
//	nb_pts_(0),
//	x_min_(std::numeric_limits<float>::max()),
//	x_max_(std::numeric_limits<float>::min()),
//	y_min_(std::numeric_limits<float>::max()),
//	y_max_(std::numeric_limits<float>::min()),
//	filename_(name)
//{
//	double x, y;
//	std::string buf;
//	std::ifstream ifs (name,std::ifstream::in);
//	if (!ifs.is_open()) {
//		std::cout << "Could not read file." << std::endl;
//	} else {
//		if (ifs.eof()) {
//			std::cout << "File is empty!" << std::endl;
//		} else {
//			while (std::getline(ifs,buf)) {
//				if (buf == "NB SOLUTIONS") {
//					ifs >> nb_pts_;
//				} else if (buf == "FIRST OBJECTIVE") {
//					std::getline(ifs,abscissa_);
//				} else if (buf == "SECOND OBJECTIVE") {
//					std::getline(ifs,ordinate_);
//				} else if (buf == "SOLUTIONS") {
//					unsigned i = 0;
//					while (i < nb_pts_ && ifs >> x >> y && std::getline(ifs,buf)) {
//						++i;
//						if (x_min_ > x) {
//							x_min_ = x;
//						} else if (x_max_ < x) {
//							x_max_ = x;
//						}
//						if (y_min_ > y) {
//							y_min_ = y;
//						} else if (y_max_ < y) {
//							y_max_ = y;
//						}
//						pts_.push_back(FPoint(i,x,y,buf.substr(0,buf.find('#'))));
//					}
//					if (i < nb_pts_) {
//						std::cout << "NB POINTS set to " << nb_pts_ << ", but only " << i << " provided. Keeping " << i << " as NB POINTS." << std::endl;
//						nb_pts_ = i;
//					} else {
//						std::cout << "Loaded " << nb_pts_ << " points." << std::endl;
//					}
//				}
//			}
//		}
//		ifs.close();
//	}
//}

template <typename DATA>
ParetoFrontSolver<DATA>::~ParetoFrontSolver() {}

/*!
 * \brief Highest (and worst) value on first objective
 * \return worst value for the first objective
 */
template <typename DATA>
const float & ParetoFrontSolver<DATA>::getMaxX() const
{
	return x_max_;
}

/*!
 * \brief Highest (and worst) value on second objective
 * \return worst value for the second objective
 */
template <typename DATA>
const float & ParetoFrontSolver<DATA>::getMaxY() const
{
	return y_max_;
}

/*!
 * \brief Lowest (and best) value on first objective
 * \return best value for the first objective
 */
template <typename DATA>
const float & ParetoFrontSolver<DATA>::getMinX() const
{
	return x_min_;
}

/*!
 * \brief Lowest (and best) value on second objective
 * \return best value for the second objective
 */
template <typename DATA>
const float & ParetoFrontSolver<DATA>::getMinY() const
{
	return y_min_;
}

/*!
 * \brief All the solutions read from the file
 * \return vector containing all the points
 */
template <typename DATA>
const typename ParetoFrontSolver<DATA>::FPointv & ParetoFrontSolver<DATA>::getPts() const
{
	return pts_;
}

/*!
 * \brief All the solutions but in a sorted fashion.
 * \return a sorted map indexed on first objective values containing sorted maps of pointers to the solutions, indexed on second objective values.
 * \warning compute_frontiers() must have been called at least one time before calling this method
 */
template <typename DATA>
const typename ParetoFrontSolver<DATA>::FPointPtrMMap & ParetoFrontSolver<DATA>::getPtsMap() const
{
	return pts_map_;
}

/*!
 * \brief Name of the first objective
 * \return abscissa label
 */
template <typename DATA>
const std::string & ParetoFrontSolver<DATA>::getAbscissa() const
{
	return abscissa_;
}

/*!
 * \brief Name of the second objective
 * \return ordinate label
 */
template <typename DATA>
const std::string & ParetoFrontSolver<DATA>::getOrdinate() const
{
	return ordinate_;
}

/*!
 * \brief Name and path of the original file
 * \return name and path of the original file
 */
template <typename DATA>
const std::string & ParetoFrontSolver<DATA>::getFilename() const
{
	return filename_;
}

/*!
 * \brief Number of solutions stored in pts_
 * \return number of solutions
 */
template <typename DATA>
const unsigned & ParetoFrontSolver<DATA>::getNbPts() const
{
	return nb_pts_;
}

/*!
 * \brief List of Pareto frontiers, from the best to the worst.
 * \return all the computed frontiers
 * \see compute_frontiers
 */
template <typename DATA>
typename ParetoFrontSolver<DATA>::ParetoFrontv & ParetoFrontSolver<DATA>::getPFrontiers()
{
	return pFrontiers_;
}

/*!
 * \brief Set the points set
 * \warning if fronts had been computed before, then it needs to be run again
 * \note nbPts is updated
 */
template <typename DATA>
void ParetoFrontSolver<DATA>::setPts(FPointv &v)
{
	pts_ = v;
	nb_pts_ = v.size();
	x_min_ = v.front().getX();
	x_max_ = v.front().getX();
	y_min_ = v.front().getY();
	y_max_ = v.front().getY();
	for (unsigned i =  0; i < nb_pts_; ++i) {
		if (v[i].getX() < x_min_) {
			x_min_ = v[i].getX();
		} else if (v[i].getX() > x_max_) {
			x_max_ = v[i].getX();
		}
		if (v[i].getY() < y_min_) {
			y_min_ = v[i].getY();
		} else if (v[i].getY() > y_max_) {
			y_max_ = v[i].getY();
		}
	}
}

/*!
 * \brief Builds a list of all the points into the rectangle defined by \a top_left and \a bottom_right.
 * \param top_left     Coordinates of the top left hand corner of the rectangle.
 * \param bottom_right Coordinates of the bottom right hand corner of the rectangle.
 * \return vector of pointers on the points that are effectively in the rectangle.
 */
template <typename DATA>
typename ParetoFrontSolver<DATA>::FPointPtrv
ParetoFrontSolver<DATA>::findPointsInArea(
		typename pareto_front::POINT &top_left,
		typename pareto_front::POINT &bottom_right) const
{
	FPointPtrv in_area = FPointPtrv();

	typename FPointPtrMMap::const_iterator border_left_itr;
	typename FPointPtrMMap::const_iterator border_right_itr;
	typename FPointPtrMap::const_iterator  border_top_itr;
	typename FPointPtrMap::const_iterator  border_bottom_itr;
	if (! pts_map_.empty()) {
		border_left_itr = pts_map_.lower_bound(top_left.getX());
		border_right_itr = pts_map_.upper_bound(bottom_right.getX());
		if (border_left_itr != pts_map_.end()) {
			for (; border_left_itr != border_right_itr; ++border_left_itr) {
				border_bottom_itr = border_left_itr->second.lower_bound(bottom_right.getY());
				border_top_itr = border_left_itr->second.upper_bound(top_left.getY());
				if (border_bottom_itr != border_left_itr->second.end()) {
					for (; border_bottom_itr != border_top_itr; ++border_bottom_itr) {
						in_area.push_back(border_bottom_itr->second);
					}
				}
			}
		}
	}
	return in_area;
}

/*!
 * \brief Computes the Pareto Frontiers of the whole file.
 *
 * The pts_map_ is (re)built and the pFrontiers_ are generated.
 */
template <typename DATA>
void ParetoFrontSolver<DATA>::compute_frontiers() {
	//Build Map
	typename FPointPtrMMap::iterator x_itr;
	for (typename FPointv::iterator i = pts_.begin(); i != pts_.end(); ++i) { //For each point
		x_itr = pts_map_.find(i->getX()); //Look for the abscissa (if submap exists)
		if (x_itr != pts_map_.end()) { //Submap exists
			x_itr->second.insert(std::pair<float,DATA *>(i->getY(),&(*i)));
		} else { //Submap does not exist
			FPointPtrMap ymap;  //Create the submap
			ymap.insert(std::pair<float,DATA *>(i->getY(),&(*i))); //Add the point
			pts_map_.insert(std::pair<float,std::map<float,DATA *> >(i->getX(),ymap)); //Insert the submap
		}
	}
	//Compute the frontiers using the previously built map
	if (pFrontiers_.size() > 0) {
		//If computing is called again, then clean all the old pareto fronts.
		pFrontiers_.clear();
	}
	FPointPtrMMap pts_map_copy(pts_map_);
	int i = 0; //Number of fronts
	while (!pts_map_copy.empty()) { //While there are remaining points
		++i;
		pFrontiers_.push_back(ParetoFront<DATA>()); //New empty front
		pFrontiers_.back().pts().push_back(pts_map_copy.begin()->second.begin()->second); //Add first point to the front
		DATA *current = pFrontiers_.back().pts().front();  //Set current point
		pts_map_copy.begin()->second.erase(current->getY()); //Remove the point in the map
		if (pts_map_copy.begin()->second.empty()) {          //Remove the submap if empty
			pts_map_copy.erase(current->getX());
		}
		if (!pts_map_copy.empty()) {        //If there are some remaining points in the map
			x_itr = pts_map_copy.begin();   //Init the iterator over the map
			while (x_itr != pts_map_copy.end()) { //Some points have not been tested for insertion
				if (x_itr->second.begin()->second->dominates(*current)) { //If the last inserted point is dominated
					pFrontiers_.back().pts().push_back(x_itr->second.begin()->second);  //Insertion
					current = x_itr->second.begin()->second;    //Update the last inserted point
					x_itr->second.erase(current->getY());       //Remove the point in the map
					if (x_itr->second.empty()) {                //If submap is empty
						x_itr = pts_map_copy.erase(x_itr);          //Remove the submap
					} else {                                    //Else
						++x_itr;                                    //Jump to the next point
					}
				} else {        //Last inserted point is not dominated
					++x_itr;    //Jump to the next point
				}
			}
		}
	}
	//std::cout << "Computed " << i << " fronts." << std::endl;
}

/*!
 * \brief Saves the set of solutions into \a name file.
 * \param name Name of the output file
 */
template <typename DATA>
void ParetoFrontSolver<DATA>::saveToFile(const char * name) const {
	std::ofstream output;
	output.open (name);

	output << "Solution file generated by ParetoViewer." << std::endl << std::endl;
	output << "NB SOLUTIONS" << std::endl;
	output << nb_pts_ << std::endl;
	output << "FIRST OBJECTIVE" << std::endl;
	output << abscissa_ << std::endl;
	output << "SECOND OBJECTIVE" << std::endl;
	output << ordinate_ << std::endl;
	output << "SOLUTIONS" << std::endl;
	for (typename FPointv::const_iterator itr = pts_.begin(); itr != pts_.end(); ++itr) {
		output << itr->getX() << '\t' << itr->getY() << '\t' << itr->getInfo() << std::endl;
	}
	output.close();
}
