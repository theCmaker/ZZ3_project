#include "move_replace.hpp"
#include <cmath>
#include <assert.h>

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
	const Solution::InterceptionNode * previousNode;
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

	assert(round(1.0e+6*_best_interception_date) == round(1.0e+6*solution.lastInterceptionTime(*_best_interceptor)));
	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
