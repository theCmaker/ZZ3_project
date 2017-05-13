#include <cmath>
#include <assert.h>

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

	// Find a route for extraction
	while (interceptor_extraction_id < problem.nbInterceptors() && Policy::keepOn()) {
		interceptor_extraction = &(problem.interceptors()[interceptor_extraction_id]);
		Solution::const_iterator position_extraction_it = solution.begin(*interceptor_extraction);
		// Find a mobile for extraction
		while (position_extraction_it != solution.end(*interceptor_extraction) && Policy::keepOn()) {
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
			// Find an insertion route
			interceptor_insertion_id = 0;
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

							insertion_interception_date = previousNode->_date;
							insertion_interceptor_position = previousNode->_mobile.position(insertion_interception_date);
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
#ifndef NDEBUG
	const Interceptor * best_interceptor_extraction =  solution.mobile(_best_mobile_candidate->id())._interceptor;
#endif
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

	assert(round(_best_interception_date*1.0e+6) == round(std::max(solution.lastInterceptionTime(*_best_interceptor_insertion),solution.lastInterceptionTime(*best_interceptor_extraction))*1.0e+6));
	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
