#include <cmath>
#include <assert.h>

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

		// Find a position for extraction
		while (position_it != solution.end(*interceptor) && Policy::keepOn())
		{
			// Try to insert ahead of the route (if not first)
			if (position_it->_prev != -1) {
				// Departure from the depot
				interception_date = 0;
				interceptor_position = interceptor->position();

				// Catch the mobile candidate
				interception_date += interceptor->computeInterception(interceptor_position, position_it->_mobile, interception_date);
				interceptor_position = position_it->_mobile.position(interception_date);

				// Catch all to the candidate predecessor
				interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.firstOfRoute(*interceptor)._mobile, solution.mobile(position_it->_prev)._mobile);
				interceptor_position = solution.mobile(position_it->_prev)._mobile.position(interception_date);

				// Catch all after the candidate
				if (position_it->_next != -1) {
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.mobile(position_it->_next)._mobile, solution.lastOfRoute(*interceptor)._mobile);
					interceptor_position = solution.lastOfRoute(*interceptor)._mobile.position(interception_date);
				}

				// Test
				if (std::isfinite(interception_date)
						&& interception_date < solution.lastInterceptionTime(*interceptor)
						&& Policy::update(interception_date,_best_interception_date)) {
					// Update results
					_best_mobile_move = &(position_it->_mobile);
					_best_mobile_position = -1;
					_best_interceptor = interceptor;
					improved = true;
				}
			}
			// For each insertion position before candidate
			Solution::const_iterator mobile_it = solution.begin(*interceptor);
			if (mobile_it->_next == position_it->_next) {
				++mobile_it;
			}
			while (mobile_it != solution.end(*interceptor) && mobile_it->_next != (int) position_it->_mobile.id() && Policy::keepOn())
			{
				// Catch all the mobiles before insertion position
				interception_date = mobile_it->_date;
				interceptor_position = mobile_it->_mobile.position(interception_date);

				// Catch the mobile candidate
				interception_date += interceptor->computeInterception(interceptor_position, position_it->_mobile, interception_date);
				interceptor_position = position_it->_mobile.position(interception_date);

				// Catch the other mobiles after insertion position to the candidate predecessor
				if (position_it->_prev != -1) {
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.mobile(mobile_it->_next)._mobile, solution.mobile(position_it->_prev)._mobile);
					interceptor_position = solution.mobile(position_it->_prev)._mobile.position(interception_date);
				}

				// Catch all the mobiles after candidate to the end, if any
				if (position_it->_next != -1) {
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.mobile(position_it->_next)._mobile, solution.lastOfRoute(*interceptor)._mobile);
				}

				// Test
				if (std::isfinite(interception_date)
						&& interception_date < solution.lastInterceptionTime(*interceptor)
						&& Policy::update(interception_date,_best_interception_date)) {
					// Update results
					_best_mobile_move = &(position_it->_mobile);
					_best_mobile_position = (int) mobile_it->_mobile.id();
					_best_interceptor = interceptor;
					improved = true;
				}
				++mobile_it;
			}

			// For each insertion position after candidate
			mobile_it = position_it;
			++mobile_it;
			while (mobile_it != solution.end(*interceptor) && Policy::keepOn()) {
				// Catch all the mobiles before candidate, if any
				if (position_it->_prev != -1) {
					interception_date = solution.mobile(position_it->_prev)._date;
					interceptor_position = solution.mobile(position_it->_prev)._mobile.position(interception_date);
				} else {
					interception_date = 0.;
					interceptor_position = interceptor->position();
				}

				// Catch the mobiles between extraction and insertion positions
				interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.mobile(position_it->_next)._mobile, mobile_it->_mobile);
				interceptor_position = mobile_it->_mobile.position(interception_date);

				// Catch the mobile candidate
				interception_date += interceptor->computeInterception(interceptor_position, position_it->_mobile, interception_date);
				interceptor_position = position_it->_mobile.position(interception_date);

				// Catch all the mobiles after insertion position to the end, if any
				if (mobile_it->_next != -1) {
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, solution.mobile(mobile_it->_next)._mobile, solution.lastOfRoute(*interceptor)._mobile);
				}

				// Test
				if (std::isfinite(interception_date)
						&& interception_date < solution.lastInterceptionTime(*interceptor)
						&& Policy::update(interception_date,_best_interception_date)) {
					// Update results
					_best_mobile_move = &(position_it->_mobile);
					_best_mobile_position = (int) mobile_it->_mobile.id();
					_best_interceptor = interceptor;
					improved = true;
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
	solution.recomputeFrom(solution.firstOfRoute(*_best_interceptor)._mobile);

	assert(round(1.0e+6*_best_interception_date) == round(1.0e+6*solution.lastInterceptionTime(*_best_interceptor)));
	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
