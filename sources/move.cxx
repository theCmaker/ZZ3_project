#include <cmath>

// ---------------------------------------------------------------------------------------------------------------------
// INSERT MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveInsert<Policy>::MoveInsert() :
	_best_mobile_candidate(nullptr),
	_best_mobile_prev_index(-1),
	_best_interceptor(nullptr),
	_best_interception_date(-1) {}

template <typename Policy>
MoveInsert<Policy>::~MoveInsert() {}

template <typename Policy>
bool MoveInsert<Policy>::scan(const Solution & solution)
{
	const Problem & _p = solution.problem();
	//TODO: comment it!
	Policy::reset();
	bool improved = false;
	const Interceptor * interceptor;
	Time interception_date;
	_best_interception_date = solution.worstInterceptionTime();
	//_best_interception_date = Policy::maxAcceptableTime();
	Location interceptor_position;
	unsigned interceptor_id = 0;

	std::vector<const Mobile *> uncaught_mobiles = solution.uncaughtMobiles();

	std::vector<const Mobile *>::iterator uncaught_mobile_itr = uncaught_mobiles.begin();

	// Find a mobile
	while (uncaught_mobile_itr != uncaught_mobiles.end() && Policy::keepOn()) {
		// Find a route
		while (interceptor_id < _p.nbInterceptors() && Policy::keepOn()) {
			interception_date = 0.;
			interceptor = &(_p.interceptors()[interceptor_id]);
			interceptor_position = interceptor->position();
			Solution::const_iterator position_it = solution.begin(*interceptor);

			// Try to insert in first position
			interception_date += interceptor->computeInterception(interceptor_position, **uncaught_mobile_itr, interception_date);
			if (std::isfinite(interception_date)) {
				interceptor_position = (*uncaught_mobile_itr)->position(interception_date);
				if (position_it != solution.end(*interceptor)) {
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, position_it->_mobile, solution.lastOfRoute(*interceptor)._mobile);
				}
				if (Policy::update(interception_date, _best_interception_date)) {
					// Update results
					_best_mobile_candidate = *uncaught_mobile_itr;
					_best_mobile_prev_index = -1;
					_best_interceptor = interceptor;
					improved = true;
				}
			}

			// Find an insertion position if insertion ahead was unsuccessful.
			while (position_it != solution.end(*interceptor) && Policy::keepOn()) {
				// Get the interception position of the current intercepted (and already in the route) mobile
				interception_date = position_it->_date;
				interceptor_position = position_it->_mobile.position(interception_date);

				// Get the time to catch the uncaught mobile.
				interception_date += interceptor->computeInterception(interceptor_position, **uncaught_mobile_itr, interception_date);
				interceptor_position = (*uncaught_mobile_itr)->position(interception_date);

				// Get the time to catch the rest of the route (same order).
				if (position_it->_next >= 0) { // Insertion inside the route (not at the end)
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, _p.mobiles()[position_it->_next], solution.lastOfRoute(*interceptor)._mobile);
				}
				if (std::isfinite(interception_date) && Policy::update(interception_date,_best_interception_date)) {
					// Update results
					_best_mobile_candidate = *uncaught_mobile_itr;
					_best_mobile_prev_index = position_it->_mobile.id();
					_best_interceptor = interceptor;
					improved = true;
				}
				++position_it;
			}
			++interceptor_id;
		}
		++uncaught_mobile_itr;
	}
	return improved;
}

template <typename Policy>
void MoveInsert<Policy>::commit(Solution & solution)
{
	// insertion ahead or creation of a route
	if(_best_mobile_prev_index < 0)
	{
		solution.prepend(*_best_interceptor, *_best_mobile_candidate, _best_interception_date);
	}
	// if insertion at end
	else if(solution.mobile(_best_mobile_prev_index)._next < 0)
	{
		solution.append(*_best_interceptor, *_best_mobile_candidate, _best_interception_date);
	}
	else
	{
		solution.insertAfter(_best_mobile_prev_index, *_best_interceptor, *_best_mobile_candidate, _best_interception_date);
	}
	solution.recomputeFrom(*_best_mobile_candidate);
}


// ---------------------------------------------------------------------------------------------------------------------
// EXTRACT MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveExtract<Policy>::MoveExtract() :
	_best_mobile_candidate(nullptr),
	_best_interception_date(-1.),
	_best_recompute_from(nullptr)
{}

template <typename Policy>
MoveExtract<Policy>::~MoveExtract() {}

template <typename Policy>
bool MoveExtract<Policy>::scan(const Solution & solution)
{
	bool improved = false;
	if (solution.first() >= 0) {
		// If there is at least one route.
		const Problem & _p = solution.problem();
		//TODO: comment it!
		Policy::reset();
		const Solution::MobileNode * previousNode;
		const Interceptor & interceptor = solution.worstRoute();
		Time interception_date;
		//unsigned best_mobile_number = solution.totalInterceptionCount();
		unsigned best_mobile_number = Policy::minAcceptableCount();
		unsigned possible_mobile_number = solution.totalInterceptionCount() - 1;
		//TODO: check all possible positions in other routes
		Location interceptor_position;
		_best_interception_date = solution.lastInterceptionTime(interceptor); // Worst time is the time for the worst route

		// Mobile to be extracted
		Solution::const_iterator position_it = solution.begin(interceptor);

		// Find an extraction position
		while (position_it != solution.end(interceptor) && Policy::keepOn()) {
			if (position_it->_prev < 0) {
				// First of route
				interception_date = 0.;
				interceptor_position = interceptor.position();
			} else {
				// Into the route
				previousNode = &(solution.mobile((unsigned) position_it->_prev));

				interception_date = previousNode->_date;
				interceptor_position = previousNode->_mobile.position(interception_date);
			}

			// Get the time to catch the rest of the route (same order), if any.
			if (position_it->_next >= 0) {
				// Not end of route
				interception_date += solution.evaluate(interceptor_position, interception_date, interceptor, _p.mobiles()[position_it->_next], solution.lastOfRoute(interceptor)._mobile);
			}
			if (std::isfinite(interception_date) && Policy::update(interception_date, _best_interception_date, possible_mobile_number, best_mobile_number)) {
				// Update results
				_best_mobile_candidate = &(position_it->_mobile);
				if (position_it->_next >= 0) {
					_best_recompute_from = &(solution.mobile(position_it->_next)._mobile);
				} else {
					_best_recompute_from = nullptr;
				}
				improved = true;
			}
			++position_it;
		}
	}
	return improved;
}

template <typename Policy>
void MoveExtract<Policy>::commit(Solution & solution)
{
	solution.remove(*_best_mobile_candidate);
	if (_best_recompute_from != nullptr)
		solution.recomputeFrom(*_best_recompute_from);
}


// ---------------------------------------------------------------------------------------------------------------------
// TWO-OPT MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
Move2Opt<Policy>::Move2Opt() : _best_mobile_candidate_first(nullptr),
							   _best_mobile_candidate_second(nullptr),
							   _best_interceptor_first(nullptr),
							   _best_interceptor_second(nullptr),
							   _best_interception_date(-1)
{}

template <typename Policy>
Move2Opt<Policy>::~Move2Opt() {}

template <typename Policy>
bool Move2Opt<Policy>::scan(const Solution & solution)
{
	const Problem & _p = solution.problem();
	Policy::reset();
	bool improved = false;
	//_best_interception_date = solution.worstInterceptionTime();
	_best_interception_date = Policy::maxAcceptableTime();
	unsigned interceptor_id_first = 0;
	unsigned interceptor_id_second = 1;
	Solution::const_iterator mobile_it_first = NULL;
	Solution::const_iterator mobile_it_second = NULL;
	Time interception_time_first = 0;
	Time interception_time_second = 0;
	Location interceptor_position_first;
	Location interceptor_position_second;
	
	int i = -1;
	int j = -1; 

	// chooses an interceptor
	while(interceptor_id_first < _p.nbInterceptors()-1 && Policy::keepOn())
	{
		std::cout << "int_first: " << interceptor_id_first << std::endl;
		interceptor_id_second = interceptor_id_first+1;
		// chooses an interceptor after interceptor_id_first
		while(interceptor_id_second < _p.nbInterceptors() && Policy::keepOn())
		{
			std::cout << "int_second: " << interceptor_id_second << std::endl;

			// for each mobile+1 in the route of interceptor_id_first
			mobile_it_first = solution.begin(_p.interceptors()[interceptor_id_first]);
			while(((i == -1 && mobile_it_first != solution.end(_p.interceptors()[interceptor_id_first])) || i != -1) && Policy::keepOn())
			//while(mobile_it_first != solution.end(_p.interceptors()[interceptor_id_first]) && Policy::keepOn())
			{
				std::cout << "mob_first: " << i << std::endl;

				// for each mobile+1 in the route of interceptor_id_second
				mobile_it_second = solution.begin(_p.interceptors()[interceptor_id_second]);
				while(((j == -1 && mobile_it_second != solution.end(_p.interceptors()[interceptor_id_second])) || j != -1) && Policy::keepOn())
				//while(mobile_it_second != solution.end(_p.interceptors()[interceptor_id_second]) && Policy::keepOn())
				{
					std::cout << "mob_second: " << j << std::endl;

					// computes interception time of the mobiles previous mobile_it_first and second
					//if(mobile_it_first->_prev == -1)
					if(i == -1)
					{
						interception_time_first = 0;
						interceptor_position_first = _p.interceptors()[interceptor_id_first].position();
					}
					else
					{
						//interception_time_first = solution.mobile(mobile_it_first->_prev)._date;
						interception_time_first = mobile_it_first->_date;
						//interceptor_position_first = _p.mobiles()[mobile_it_first->_prev].position(interception_time_first);
						interceptor_position_first = mobile_it_first->_mobile.position(interception_time_first);
					}
					//if(mobile_it_second->_prev == -1)
					if(j == -1)
					{
						interception_time_second = 0;
						interceptor_position_second = _p.interceptors()[interceptor_id_second].position();
					}
					else
					{
						//interception_time_second = solution.mobile(mobile_it_second->_prev)._date;
						interception_time_second = mobile_it_second->_date;
						//interceptor_position_second = _p.mobiles()[mobile_it_second->_prev].position(interception_time_second);
						interceptor_position_second = mobile_it_second->_mobile.position(interception_time_second);
					}

					// evaluates the final interception time of each route after crossing the mobiles

					//if (mobile_it_first->_mobile.id() >= 0)
					//if(i >= 0)
					//{
						interception_time_first += solution.evaluate(interceptor_position_first, interception_time_first, _p.interceptors()[interceptor_id_first], mobile_it_second->_mobile, solution.lastOfRoute(_p.interceptors()[interceptor_id_second])._mobile);
					//}
					//if (mobile_it_second->_mobile.id() >= 0) {
						interception_time_second += solution.evaluate(interceptor_position_second, interception_time_second, _p.interceptors()[interceptor_id_second], mobile_it_first->_mobile, solution.lastOfRoute(_p.interceptors()[interceptor_id_first])._mobile);
					//}

					std::cout << "res: " << std::max(interception_time_first, interception_time_second) << std::endl;

					// tests if the move improves the results
					if(std::isfinite(interception_time_first)
							&& std::isfinite(interception_time_second)
							&& (interception_time_first + interception_time_second) < (solution.lastInterceptionTime(interceptor_id_first) + solution.lastInterceptionTime(interceptor_id_second))
							&& Policy::update(std::max(interception_time_first, interception_time_second), _best_interception_date))
					{
						_best_mobile_candidate_first = &(mobile_it_first->_mobile);
						_best_mobile_candidate_second = &(mobile_it_second->_mobile);
						_best_interceptor_first = &(_p.interceptors()[interceptor_id_first]);
						_best_interceptor_second = &(_p.interceptors()[interceptor_id_second]);
						improved = true;
					}

					if(j != -1)
					{
						++mobile_it_second;
					}
					if(mobile_it_second != solution.end(_p.interceptors()[interceptor_id_second]))
					{
						j = mobile_it_second->_mobile.id();
					}
					else
					{
						j = -1;
					}
				}
				if(i != -1)
				{
					++mobile_it_first;
				}
				if(mobile_it_first != solution.end(_p.interceptors()[interceptor_id_first]))
				{
					i = mobile_it_first->_mobile.id();
				}
				else
				{
					i = -1;
				}
			}

			++interceptor_id_second;
		}
		++interceptor_id_first;
	}

	std::cout << "scan finished" << std::endl;

	return improved;
}

template <typename Policy>
void Move2Opt<Policy>::commit(Solution & solution)
{
	// removes the sequences in the routes
	// checks if each route is not empty
	if(_best_mobile_candidate_first)
	{
		solution.removeSeqFrom(*_best_mobile_candidate_first);
	}
	if(_best_mobile_candidate_second)
	{
		solution.removeSeqFrom(*_best_mobile_candidate_second);
	}

	// append the sequences in the other route
	solution.appendSeq(*_best_interceptor_first,*_best_mobile_candidate_second);
	solution.appendSeq(*_best_interceptor_second,*_best_mobile_candidate_first);

	// updates dates in the nodes
	solution.recomputeFrom(*_best_mobile_candidate_first);
	solution.recomputeFrom(*_best_mobile_candidate_second);
}


// ---------------------------------------------------------------------------------------------------------------------
// REPLACE MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveReplace<Policy>::MoveReplace() :
	_best_mobile_in(nullptr),
	_best_mobile_out(nullptr),
	_best_interceptor(nullptr),
	_best_interception_date(-1.)
{}

template <typename Policy>
MoveReplace<Policy>::~MoveReplace()
{}

template <typename Policy>
bool MoveReplace<Policy>::scan(const Solution & solution)
{
	//TODO: comment it!
	const Problem & problem = solution.problem();
	Policy::reset();
	bool improved = false;
	const Interceptor * interceptor;
	const Solution::MobileNode * previousNode;
	Time interception_date;
	_best_interception_date = Policy::maxAcceptableTime();
	Location interceptor_position;
	unsigned interceptor_id = 0;

	std::vector<const Mobile *> uncaught_mobiles = solution.uncaughtMobiles();

	std::vector<const Mobile *>::iterator uncaught_mobile_itr = uncaught_mobiles.begin();

	// Find a mobile to add
	while (uncaught_mobile_itr != uncaught_mobiles.end() && Policy::keepOn()) {
		// Find a route
		while (interceptor_id < problem.nbInterceptors() && Policy::keepOn()) {
			interceptor = &(problem.interceptors()[interceptor_id]);

			Solution::const_iterator position_it = solution.begin(*interceptor);

			// Find a replace position
			while (position_it != solution.end(*interceptor) && Policy::keepOn()) {
				if (position_it->_prev < 0) {
					// First of route
					interception_date = 0.;
					interceptor_position = interceptor->position();
				} else {
					// Into the route
					previousNode = &(solution.mobile((unsigned) position_it->_prev));

					interception_date = previousNode->_date;
					interceptor_position = previousNode->_mobile.position(interception_date);
				}

				// Get the time to catch the uncaught mobile.
				interception_date += interceptor->computeInterception(interceptor_position, **uncaught_mobile_itr, interception_date);
				interceptor_position = (*uncaught_mobile_itr)->position(interception_date);

				// Get the time to catch the rest of the route (same order), if any.
				if (position_it->_next >= 0) {
					// Not end of route
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[position_it->_next], solution.lastOfRoute(*interceptor)._mobile);
				}
				if (std::isfinite(interception_date)
						&& interception_date <= solution.lastInterceptionTime(*interceptor)
						&& Policy::update(interception_date,_best_interception_date)) {
					// Update results
					_best_mobile_out = &(position_it->_mobile);
					_best_mobile_in = *uncaught_mobile_itr;
					_best_interceptor = interceptor;
					improved = true;
				}
				++position_it;
			}
			++interceptor_id;
		}
		++uncaught_mobile_itr;
	}
	return improved;
}

template <typename Policy>
void MoveReplace<Policy>::commit(Solution & solution)
{
	solution.insertAfter(*_best_mobile_out, *_best_interceptor, *_best_mobile_in, _best_interception_date);
	solution.remove(*_best_mobile_out);
	solution.recomputeFrom(*_best_mobile_in);
}


// ---------------------------------------------------------------------------------------------------------------------
// MOVE 1 ROUTE MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveMove1Route<Policy>::MoveMove1Route() :	_best_mobile_move(nullptr),
											_best_mobile_position(-1),
											_best_interceptor(nullptr),
											_best_interception_date(-1)
{}

template <typename Policy>
MoveMove1Route<Policy>::~MoveMove1Route()
{}

template <typename Policy>
bool MoveMove1Route<Policy>::scan(const Solution & solution)
{
	const Problem & problem = solution.problem();
	bool improved = false;
	Policy::reset();
	const Interceptor * interceptor;
	Time interception_date;
	//_best_interception_date = solution.worstInterceptionTime();
	_best_interception_date = Policy::maxAcceptableTime();
	Location interceptor_position;
	unsigned interceptor_id = 0;


	// For each interceptors
	while (interceptor_id < problem.nbInterceptors() && Policy::keepOn())
	{
		interceptor = &(problem.interceptors()[interceptor_id]);
		Solution::const_iterator position_it = solution.begin(*interceptor);

		// Find a position
		while (position_it != solution.end(*interceptor) && Policy::keepOn())
		{
			Solution::const_iterator mobile_it = position_it; //solution.begin(*interceptor);
			++mobile_it;
			// For each mobile
			while(mobile_it != solution.end(*interceptor) && Policy::keepOn())
			{
				interception_date = 0;
				interceptor_position = interceptor->position();

				// if the mobile can be moved
				if(mobile_it != position_it && mobile_it->_prev != -1)
				{
					// mobiles before position_it
					interception_date += interceptor->computeInterception(interceptor_position, problem.mobiles()[position_it->_prev], interception_date);
					interceptor_position = problem.mobiles()[position_it->_prev].position(interception_date);

					// interception of mobile_it
					interception_date += interceptor->computeInterception(interceptor_position, mobile_it->_mobile, interception_date);
					interceptor_position = mobile_it->_mobile.position(interception_date);

					// mobiles between position_it and mobile_it->_prev
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, position_it->_mobile, problem.mobiles()[mobile_it->_prev]);
					interceptor_position = problem.mobiles()[mobile_it->_prev].position(interception_date);

					// mobiles after mobile_it
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[mobile_it->_next], solution.lastOfRoute(*interceptor)._mobile);

					// compare
					if (std::isfinite(interception_date)
							&& interception_date < solution.lastInterceptionTime(*interceptor)
							&& Policy::update(interception_date,_best_interception_date)) {
						// Update results
						_best_mobile_move = &(mobile_it->_mobile);
						_best_mobile_position = position_it->_prev;
						_best_interceptor = interceptor;
						improved = true;
					}

				}

				// case: move at end
				if(mobile_it != position_it && position_it->_next == -1)
				{
					interception_date = 0;
					interceptor_position = interceptor->position();

					// mobiles before mobile_it
					if(mobile_it->_prev != -1)
					{
						interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.begin(*interceptor)->_mobile, problem.mobiles()[mobile_it->_prev]);
						interceptor_position = problem.mobiles()[mobile_it->_prev].position(interception_date);
					}

					// mobiles after mobile_it
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[mobile_it->_next], solution.lastOfRoute(*interceptor)._mobile);
					interceptor_position = problem.mobiles()[mobile_it->_next].position(interception_date);

					// mobile_it
					interception_date+= interceptor->computeInterception(interceptor_position, mobile_it->_mobile, interception_date);

					// compare
					if (std::isfinite(interception_date)
							&& interception_date < solution.lastInterceptionTime(*interceptor)
							&& Policy::update(interception_date,_best_interception_date)) {
						// Update results
						_best_mobile_move = &(mobile_it->_mobile);
						_best_mobile_position = position_it->_prev;
						_best_interceptor = interceptor;
						improved = true;
					}
				}
				++mobile_it;
			}
			++position_it;
		}
		++interceptor_id;
	}
	return improved;
}

template <typename Policy>
void MoveMove1Route<Policy>::commit(Solution & solution)
{
	solution.remove(*_best_mobile_move);
	if(_best_mobile_position == -1)
	{
		solution.prepend(*_best_interceptor,*_best_mobile_move,_best_interception_date);
	}
	else
	{
		solution.insertAfter(_best_mobile_position, _best_interceptor->id(), _best_mobile_move->id(), _best_interception_date);
	}
	solution.recomputeFrom(*_best_mobile_move);
}


// ---------------------------------------------------------------------------------------------------------------------
// MOVE 2 ROUTES MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveMove2Routes<Policy>::MoveMove2Routes() :
	_best_mobile_candidate(nullptr),
	_best_mobile_insertion_prev(nullptr),
	_best_interceptor_insertion(nullptr),
	_best_interception_date(-1.)
{}

template<typename Policy>
MoveMove2Routes<Policy>::~MoveMove2Routes()
{}

template<typename Policy>
bool MoveMove2Routes<Policy>::scan(const Solution & solution)
{
	const Problem & problem = solution.problem();
	bool improved = false;
	Policy::reset();
	const Interceptor * interceptor_extraction;
	const Interceptor * interceptor_insertion;
	Time extraction_interception_date;
	Time insertion_interception_date;
	//_best_interception_date = solution.worstInterceptionTime();
	_best_interception_date = Policy::maxAcceptableTime();
	Location extraction_interceptor_position;
	Location insertion_interceptor_position;
	unsigned interceptor_extraction_id = 0;
	unsigned interceptor_insertion_id;
	const Solution::MobileNode * previousNode;

	// Find a route
	while (interceptor_extraction_id < problem.nbInterceptors() && Policy::keepOn()) {
		interceptor_extraction = &(problem.interceptors()[interceptor_extraction_id]);
		Solution::const_iterator position_extraction_it = solution.begin(*interceptor_extraction);
		// Find a mobile for extraction
		while (position_extraction_it != solution.end(*interceptor_extraction) && Policy::keepOn()) {
			interceptor_insertion_id = 0;
			// Find an insertion route
			//Extraction
			if (position_extraction_it->_prev < 0) {
				// First of route
				extraction_interception_date = 0.;
				extraction_interceptor_position = interceptor_extraction->position();
			} else {
				// Into the route
				previousNode = &(solution.mobile((unsigned) position_extraction_it->_prev));

				extraction_interception_date = previousNode->_date;
				extraction_interceptor_position = previousNode->_mobile.position(extraction_interception_date);
			}
			// Get the time to catch the rest of the route (same order), if any.
			if (position_extraction_it->_next >= 0) {
				// Not end of route
				extraction_interception_date += solution.evaluate(extraction_interceptor_position, extraction_interception_date, *interceptor_extraction, problem.mobiles()[position_extraction_it->_next], solution.lastOfRoute(*interceptor_extraction)._mobile);
			}
			while (interceptor_insertion_id < problem.nbInterceptors() && Policy::keepOn() && std::isfinite(extraction_interception_date)) {
				if (interceptor_extraction_id != interceptor_insertion_id) {
					interceptor_insertion = &(problem.interceptors()[interceptor_insertion_id]);
					Solution::const_iterator position_insertion_it = solution.begin(*interceptor_insertion);
					// Find an insertion position

					//Try first the insertion after the end of the route.
					if (position_insertion_it != solution.end(*interceptor_insertion)) {
						// Get last values of the route
						insertion_interception_date = solution.lastOfRoute(*interceptor_insertion)._date;
						insertion_interceptor_position = solution.lastOfRoute(*interceptor_insertion)._mobile.position(insertion_interception_date);

						//Catch the mobile
						insertion_interception_date += interceptor_insertion->computeInterception(insertion_interceptor_position, position_extraction_it->_mobile, insertion_interception_date);
						insertion_interceptor_position = position_extraction_it->_mobile.position(insertion_interception_date);

						//Compare
						if (std::isfinite(insertion_interception_date)
								&& ((extraction_interception_date + insertion_interception_date) < solution.lastInterceptionTime(*interceptor_extraction) + solution.lastInterceptionTime(*interceptor_insertion))
								&& Policy::update(std::max(extraction_interception_date, insertion_interception_date), _best_interception_date)) {
							// Update results
							_best_mobile_candidate = &(position_extraction_it->_mobile);
							_best_interceptor_insertion = interceptor_insertion;
							_best_mobile_insertion_prev = &(solution.lastOfRoute(*interceptor_insertion)._mobile);
							improved = true;
						}
					}

					// Let's try all possible positions before a mobile.
					while (position_insertion_it != solution.end(*interceptor_insertion) && Policy::keepOn()) {
						//Insertion
						if (position_insertion_it->_prev < 0) {
							// First of route
							insertion_interception_date = 0.;
							insertion_interceptor_position = interceptor_insertion->position();
						} else {
							// Into the route
							previousNode = &(solution.mobile((unsigned) position_insertion_it->_prev));

							insertion_interception_date = position_insertion_it->_date;
							insertion_interceptor_position = position_insertion_it->_mobile.position(insertion_interception_date);
						}
						// Catch the mobile
						insertion_interception_date += interceptor_insertion->computeInterception(insertion_interceptor_position, position_extraction_it->_mobile, insertion_interception_date);
						insertion_interceptor_position = position_extraction_it->_mobile.position(insertion_interception_date);

						// Get the time to catch the rest of the route (same order), if any.
						insertion_interception_date += solution.evaluate(insertion_interceptor_position, insertion_interception_date, *interceptor_insertion, position_insertion_it->_mobile, solution.lastOfRoute(*interceptor_insertion)._mobile);
						//Compare
						if (std::isfinite(insertion_interception_date)
								&& ((extraction_interception_date + insertion_interception_date) < (solution.lastInterceptionTime(*interceptor_extraction) + solution.lastInterceptionTime(*interceptor_insertion)))
								&& Policy::update(std::max(extraction_interception_date,insertion_interception_date),_best_interception_date)) {
							// Update results
							_best_mobile_candidate = &(position_extraction_it->_mobile);
							_best_interceptor_insertion = interceptor_insertion;
							if (position_insertion_it->_prev >= 0) {
								_best_mobile_insertion_prev = &(solution.mobile(position_insertion_it->_prev)._mobile);
							} else {
								_best_mobile_insertion_prev = nullptr;
							}
							improved = true;
						}
						++position_insertion_it;
					}
				}
				++interceptor_insertion_id;
			}
			++position_extraction_it;
		}
		++interceptor_extraction_id;
	}
	
	return improved;
}

template<typename Policy>
void MoveMove2Routes<Policy>::commit(Solution & solution)
{
	int extraction_next_index = solution.mobile(_best_mobile_candidate->id())._next;
	solution.remove(*_best_mobile_candidate);
	if (_best_mobile_insertion_prev != nullptr) {
		solution.insertAfter(*_best_mobile_insertion_prev, *_best_interceptor_insertion, *_best_mobile_candidate, _best_interception_date);
	} else {
		solution.prepend(*_best_interceptor_insertion, *_best_mobile_candidate, _best_interception_date);
	}
	if (extraction_next_index != -1) {
		solution.recomputeFrom((unsigned) extraction_next_index);
	}
	solution.recomputeFrom(*_best_mobile_candidate);
}


// ---------------------------------------------------------------------------------------------------------------------
// SWAP1ROUTE MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveSwap1Route<Policy>::MoveSwap1Route() :	_best_mobile_swap1(nullptr),
											_best_mobile_swap2(nullptr),
											_best_interceptor(nullptr),
											_best_interception_date(-1)
{}

template <typename Policy>
MoveSwap1Route<Policy>::~MoveSwap1Route()
{}

template <typename Policy>
bool MoveSwap1Route<Policy>::scan(const Solution & solution)
{
	const Problem & problem = solution.problem();
	Policy::reset();
	bool improved = false;
	unsigned interceptor_id = 0;
	Solution::const_iterator mobile_it_first = NULL;
	Solution::const_iterator mobile_it_second = NULL;
	Time interception_date = 0;
	Location interceptor_position;
	const Interceptor * interceptor;

	//_best_interception_date = solution.worstInterceptionTime();
	_best_interception_date = Policy::maxAcceptableTime();

	// for each interceptor if the policy says keep on
	while (interceptor_id < problem.nbInterceptors() && Policy::keepOn())
	{
		std::cout << "interceptor: " << interceptor_id << std::endl;
		interceptor = &(problem.interceptors()[interceptor_id]);
		_best_interception_date = solution.lastInterceptionTime(*interceptor);
		mobile_it_first = solution.begin(*interceptor);
		// for each mobile in the route
		while(mobile_it_first != solution.end(*interceptor) && Policy::keepOn())
		{
			mobile_it_second = mobile_it_first;
			++mobile_it_second;
			std::cout << "mobile 1: " << mobile_it_first->_mobile.id() << std::endl;


			// for each mobile in the route (after mobile_it_first)
			while(mobile_it_second != solution.end(*interceptor) && Policy::keepOn())
			{
				std::cout << "mobile 2: " << mobile_it_second->_mobile.id() << std::endl;
				std::cout << "date begin: " << interception_date << std::endl;

				interception_date = 0;
				interceptor_position = interceptor->position();
				// if there is mobiles between the begining and mobile_it_first
				if(mobile_it_first->_prev != -1)
				{
					std::cout << "before mob1" << std::endl;
					interception_date += solution.evaluate(interceptor->position(),interception_date,*interceptor,solution.begin(*interceptor)->_mobile,problem.mobiles()[mobile_it_first->_prev]);
					interceptor_position = problem.mobiles()[mobile_it_first->_prev].position(interception_date);

					std::cout << "date prev mob1: " << interception_date << std::endl;
				}
				// first swap: interception of mobile_it_second
				interception_date += interceptor->computeInterception(interceptor_position, mobile_it_second->_mobile, interception_date);
				interceptor_position = mobile_it_second->_mobile.position(interception_date);

				std::cout << "mob2 catched: " << interception_date << std::endl;

				// if there is mobiles between mobile_it_first and mobile_it_second
				if(mobile_it_first->_next != (int)mobile_it_second->_mobile.id())
				{
					std::cout << "between mob1 & 2" << std::endl;

					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[mobile_it_first->_next], problem.mobiles()[mobile_it_second->_prev]);
					interceptor_position = problem.mobiles()[mobile_it_second->_prev].position(interception_date);

					std::cout << "between mob1 & 2: "<< interception_date << std::endl;
				}

				// second swap: interception of mobile_it_first
				interception_date += interceptor->computeInterception(interceptor_position, mobile_it_first->_mobile, interception_date);
				interceptor_position = mobile_it_first->_mobile.position(interception_date);

				std::cout << "mob1 catched: " << interception_date << std::endl;

				// if there is mobiles after mobile_it_second
				if(mobile_it_second->_next != -1)
				{
					std::cout << "after mob2" << std::endl;
					std::cout << mobile_it_second->_next << " " << solution.lastOfRoute(*interceptor)._mobile.id() << std::endl;
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[mobile_it_second->_next], solution.lastOfRoute(*interceptor)._mobile);

					std::cout << "after mob2: " << interception_date << std::endl;
				}

				// compare
				if (std::isfinite(interception_date)
						&& interception_date < solution.lastInterceptionTime(*interceptor)
						&& Policy::update(interception_date,_best_interception_date)) {
					// Update results
					_best_mobile_swap1 = &(mobile_it_first->_mobile);
					_best_mobile_swap2 = &(mobile_it_second->_mobile);
					_best_interceptor = interceptor;
					improved = true;
				}
				++mobile_it_second;
			}
			++mobile_it_first;
		}
		++interceptor_id;
	}
	return improved;
}

template <typename Policy>
void MoveSwap1Route<Policy>::commit(Solution & solution)
{
	// move of the first mobile to swap
	int mobile_prev1 = solution.mobile(_best_mobile_swap1->id())._prev;
	solution.remove(*_best_mobile_swap1);
	solution.insertAfter(*_best_mobile_swap2,*_best_interceptor,*_best_mobile_swap1,_best_interception_date);

	// move of the second mobile to swap
	solution.remove(*_best_mobile_swap2);
	if(mobile_prev1 == -1)
	{
		solution.prepend(*_best_interceptor,*_best_mobile_swap2,_best_interception_date);
	}
	else
	{
		solution.insertAfter(mobile_prev1, _best_interceptor->id(), _best_mobile_swap2->id(), _best_interception_date);
	}
	solution.recomputeFrom(solution[_best_interceptor->id()]._first);
}


// ---------------------------------------------------------------------------------------------------------------------
// SWAP2ROUTES MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveSwap2Routes<Policy>::MoveSwap2Routes() :	_best_mobile_swap1(nullptr),
												_best_mobile_swap2(nullptr),
												_best_interceptor1(nullptr),
												_best_interceptor2(nullptr),
												_best_interception_date(-1)
{}

template <typename Policy>
MoveSwap2Routes<Policy>::~MoveSwap2Routes()
{}

template <typename Policy>
bool MoveSwap2Routes<Policy>::scan(const Solution & solution)
{
	//TODO: comment it!
	const Problem & problem = solution.problem();
	Policy::reset();
	bool improved = false;
	unsigned interceptor_id_first = 0;
	unsigned interceptor_id_second = 1;
	Solution::const_iterator mobile_it_first = NULL;
	Solution::const_iterator mobile_it_second = NULL;
	Time interception_date_route1 = 0;
	Time interception_date_route2 = 0;
	Location interceptor_position_first;
	Location interceptor_position_second;
	const Interceptor * interceptor_first;
	const Interceptor * interceptor_second;

	//_best_interception_date = solution.worstInterceptionTime();
	_best_interception_date = Policy::maxAcceptableTime();

	// for each interceptor
	while(interceptor_id_first < problem.nbInterceptors()-1 && Policy::keepOn())
	{
		std::cout << "interceptor 1: " << interceptor_id_first << std::endl;
		interceptor_first = &(problem.interceptors()[interceptor_id_first]);

		// for each interceptor after interceptor_id_first
		while(interceptor_id_second < problem.nbInterceptors()-1 && Policy::keepOn())
		{
			std::cout << "interceptor 2: " << interceptor_id_second << std::endl;
			interceptor_second = &(problem.interceptors()[interceptor_id_second]);

			mobile_it_first = solution.begin(*interceptor_first);
			// for each mobile in the first route
			while(mobile_it_first != solution.end(*interceptor_first) && Policy::keepOn())
			{
				mobile_it_second = solution.begin(*interceptor_second);

				std::cout << "mob1: " << mobile_it_first->_mobile.id() << std::endl;

				// for each mobile in the second route
				while(mobile_it_second != solution.end(*interceptor_second) && Policy::keepOn())
				{
					std::cout << "mob2: " << mobile_it_second->_mobile.id() << std::endl;

					//-------------------------------------------------
					// computes interception date for the first route
					interception_date_route1 = 0;
					interceptor_position_first = interceptor_first->position();

					// if there is mobiles before mobile_it_first
					if(mobile_it_first->_prev != -1)
					{
						std::cout << "route1: before mob1" << std::endl;
						interception_date_route1 += solution.evaluate(interceptor_first->position(),interception_date_route1,*interceptor_first,solution.begin(*interceptor_first)->_mobile,problem.mobiles()[mobile_it_first->_prev]);
						interceptor_position_first = problem.mobiles()[mobile_it_first->_prev].position(interception_date_route1);

						std::cout << "route1: date prev mob1: " << interception_date_route1 << std::endl;
					}
					// first swap: interception of mobile_it_second
					interception_date_route1 += interceptor_first->computeInterception(interceptor_position_first, mobile_it_second->_mobile, interception_date_route1);
					interceptor_position_first = mobile_it_second->_mobile.position(interception_date_route1);

					std::cout << "route1: mob2 catched: " << interception_date_route1 << std::endl;

					// if there is mobiles after mobile_it_first
					if(mobile_it_first->_next != -1)
					{
						std::cout << "route1: after mob1" << std::endl;
						std::cout << mobile_it_first->_next << " " << solution.lastOfRoute(*interceptor_first)._mobile.id() << std::endl;
						interception_date_route1 += solution.evaluate(interceptor_position_first, interception_date_route1, *interceptor_first, problem.mobiles()[mobile_it_first->_next], solution.lastOfRoute(*interceptor_first)._mobile);

						std::cout << "route1: after mob2: " << interception_date_route1 << std::endl;
					}

					//------------------------------------------------
					// computes interception date for the second route
					interception_date_route2 = 0;
					interceptor_position_second = interceptor_second->position();

					// if there is mobiles before mobile_it_second
					if(mobile_it_second->_prev != -1)
					{
						std::cout << "route2: before mob2" << std::endl;
						interception_date_route2 += solution.evaluate(interceptor_second->position(),interception_date_route2,*interceptor_second,solution.begin(*interceptor_second)->_mobile,problem.mobiles()[mobile_it_second->_prev]);
						interceptor_position_second = problem.mobiles()[mobile_it_second->_prev].position(interception_date_route2);

						std::cout << "route2: date prev mob2: " << interception_date_route2 << std::endl;
					}
					// second swap: interception of mobile_it_first
					interception_date_route2 += interceptor_second->computeInterception(interceptor_position_second, mobile_it_first->_mobile, interception_date_route2);
					interceptor_position_second = mobile_it_first->_mobile.position(interception_date_route2);

					std::cout << "route2: mob1 catched: " << interception_date_route2 << std::endl;

					// if there is mobiles after mobile_it_second
					if(mobile_it_second->_next != -1)
					{
						std::cout << "route2: after mob2" << std::endl;
						std::cout << mobile_it_second->_next << " " << solution.lastOfRoute(*interceptor_second)._mobile.id() << std::endl;
						interception_date_route2 += solution.evaluate(interceptor_position_second, interception_date_route2, *interceptor_second, problem.mobiles()[mobile_it_second->_next], solution.lastOfRoute(*interceptor_second)._mobile);

						std::cout << "route2: after mob2: " << interception_date_route2 << std::endl;
					}

					// compare
					if (std::isfinite(interception_date_route1)
							&& std::isfinite(interception_date_route2)
							&& (interception_date_route1 + interception_date_route2) < (solution.lastInterceptionTime(*interceptor_first) + solution.lastInterceptionTime(*interceptor_second))
							&& Policy::update(std::max(interception_date_route1, interception_date_route2), _best_interception_date)) {
						// Update results
						_best_mobile_swap1 = &(mobile_it_first->_mobile);
						_best_mobile_swap2 = &(mobile_it_second->_mobile);
						_best_interceptor1 = interceptor_first;
						_best_interceptor2 = interceptor_second;
						improved = true;
					}

					++mobile_it_second;
				}
				++mobile_it_first;
			}
			++interceptor_id_second;
		}
		++interceptor_id_first;
	}

	return improved;
}

template <typename Policy>
void MoveSwap2Routes<Policy>::commit(Solution & solution)
{
	// move of the first mobile to swap
	int mobile_prev1 = solution.mobile(_best_mobile_swap1->id())._prev;
	solution.remove(*_best_mobile_swap1);
	solution.insertAfter(*_best_mobile_swap2,*_best_interceptor2,*_best_mobile_swap1,_best_interception_date);

	// move of the second mobile to swap
	solution.remove(*_best_mobile_swap2);
	if(mobile_prev1 == -1)
	{
		solution.prepend(*_best_interceptor1,*_best_mobile_swap2,_best_interception_date);
	}
	else
	{
		solution.insertAfter(mobile_prev1, _best_interceptor1->id(), _best_mobile_swap2->id(), _best_interception_date);
	}
	solution.recomputeFrom(*_best_mobile_swap1);
	solution.recomputeFrom(*_best_mobile_swap2);
}
