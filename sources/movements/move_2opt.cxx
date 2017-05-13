#include <cmath>
#include <assert.h>

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
		//std::cout << "int_first: " << interceptor_id_first << std::endl;
		interceptor_id_second = interceptor_id_first+1;
		// chooses an interceptor after interceptor_id_first
		while(interceptor_id_second < _p.nbInterceptors() && Policy::keepOn())
		{
			//std::cout << "int_second: " << interceptor_id_second << std::endl;

			// for each mobile+1 in the route of interceptor_id_first
			mobile_it_first = solution.begin(_p.interceptors()[interceptor_id_first]);
			while(((i == -1 && mobile_it_first != solution.end(_p.interceptors()[interceptor_id_first])) || i != -1) && Policy::keepOn())
			//while(mobile_it_first != solution.end(_p.interceptors()[interceptor_id_first]) && Policy::keepOn())
			{
				//std::cout << "mob_first: " << i << std::endl;

				// for each mobile+1 in the route of interceptor_id_second
				mobile_it_second = solution.begin(_p.interceptors()[interceptor_id_second]);
				while(((j == -1 && mobile_it_second != solution.end(_p.interceptors()[interceptor_id_second])) || j != -1) && Policy::keepOn())
				//while(mobile_it_second != solution.end(_p.interceptors()[interceptor_id_second]) && Policy::keepOn())
				{
					//std::cout << "mob_second: " << j << std::endl;

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

					//std::cout << "res: " << std::max(interception_time_first, interception_time_second) << std::endl;

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

	//std::cout << "scan finished" << std::endl;

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

	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
