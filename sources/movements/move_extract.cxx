#include <cmath>
#include <assert.h>

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
		const Solution::InterceptionNode * previousNode;
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
#ifndef NDEBUG
	const Interceptor * best_interceptor = solution.mobile(_best_mobile_candidate->id())._interceptor;
#endif
	solution.remove(*_best_mobile_candidate);
	if (_best_recompute_from != nullptr) {
		solution.recomputeFrom(*_best_recompute_from);
		assert(round(1.0e+6*_best_interception_date) == round(1.0e+6*solution.lastInterceptionTime(*best_interceptor)));
	}
	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
