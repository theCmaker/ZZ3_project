#include <cmath>
#include <assert.h>

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
	_best_interception_date = solution.lastInterceptionTime();
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
				if (std::isfinite(interception_date) && Policy::update(interception_date, _best_interception_date)) {
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


	assert(round(1.0e+6*_best_interception_date) == round(1.0e+6*solution.lastInterceptionTime(*_best_interceptor)));

	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
