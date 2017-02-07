#include <cmath>

// ---------------------------------------------------------------------------------------------------------------------
// INSERT MOVE
// ---------------------------------------------------------------------------------------------------------------------
template <typename Policy>
MoveInsert<Policy>::MoveInsert() : _best_interception_date(-1) {}

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
MoveExtract<Policy>::MoveExtract() {}

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
				interception_date += solution.evaluate(interceptor_position, interception_date, interceptor, _p.mobiles()[position_it->_next], solution.lastOfRoute(interceptor)._mobile);
			}
			if (std::isfinite(interception_date) && Policy::update(interception_date,_best_interception_date)) {
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
Move2Opt<Policy>::Move2Opt(Problem & p, int m0, int m1)
{
	_mobile[0] = m0;
	_mobile[1] = m1;
}

template <typename Policy>
Move2Opt<Policy>::~Move2Opt() {}

template <typename Policy>
bool Move2Opt<Policy>::scan(const Solution & solution)
{
	const Problem & _p = solution.problem();
	bool improved = true;
	Time interception_time[2] = {0,0};
	const Interceptor * interceptor[2];
	Location interceptor_position;
	int j = 0;
	
	interceptor[0] = solution.mobile(_mobile[0])._interceptor;
	interceptor[1] = solution.mobile(_mobile[1])._interceptor;
	
	for(int i = 0; i < 2; ++i)
	{
		j = (i+1)%2;
		
		interception_time[i] = solution.mobile(_mobile[i])._date;
		interceptor_position = _p.mobiles()[_mobile[i]].position(interception_time[i]);
		
		Solution::const_iterator mobile_it = Solution::const_iterator(&solution.mobile(_mobile[j]));
		++mobile_it;

		while(mobile_it != solution.end(*interceptor[j]) && std::isfinite(interception_time[i]))
		{

			interception_time[i] += interceptor[i]->computeInterception(interceptor_position,
																		mobile_it->_mobile,
																		interception_time[i]);


			interceptor_position = _p.mobiles()[mobile_it->_mobile.id()].position(interception_time[i]);
			++mobile_it;
		}
	}

	// test max
	
	std::cout << "Result Cross0: " << interception_time[0] << std::endl;
	std::cout << "Result Cross1: " << interception_time[1] << std::endl;
	
	std::cout << "Route0: " << solution.lastInterceptionTime(interceptor[0]->id()) << std::endl;
	std::cout << "Route1: " << solution.lastInterceptionTime(interceptor[1]->id()) << std::endl;
	
	improved = std::max(interception_time[0], interception_time[1]) < std::max(solution.lastInterceptionTime(interceptor[0]->id()), solution.lastInterceptionTime(interceptor[1]->id()));
	
	return improved;
}

template <typename Policy>
void Move2Opt<Policy>::commit(Solution & solution)
{
	
}

