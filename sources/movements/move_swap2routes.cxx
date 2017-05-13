#include <cmath>
#include <assert.h>

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
		//std::cout << "interceptor 1: " << interceptor_id_first << std::endl;
		interceptor_first = &(problem.interceptors()[interceptor_id_first]);

		// for each interceptor after interceptor_id_first
		while(interceptor_id_second < problem.nbInterceptors()-1 && Policy::keepOn())
		{
			//std::cout << "interceptor 2: " << interceptor_id_second << std::endl;
			interceptor_second = &(problem.interceptors()[interceptor_id_second]);

			mobile_it_first = solution.begin(*interceptor_first);
			// for each mobile in the first route
			while(mobile_it_first != solution.end(*interceptor_first) && Policy::keepOn())
			{
				mobile_it_second = solution.begin(*interceptor_second);

				//std::cout << "mob1: " << mobile_it_first->_mobile.id() << std::endl;

				// for each mobile in the second route
				while(mobile_it_second != solution.end(*interceptor_second) && Policy::keepOn())
				{
					//std::cout << "mob2: " << mobile_it_second->_mobile.id() << std::endl;

					//-------------------------------------------------
					// computes interception date for the first route
					interception_date_route1 = 0;
					interceptor_position_first = interceptor_first->position();

					// if there is mobiles before mobile_it_first
					if(mobile_it_first->_prev != -1)
					{
						//std::cout << "route1: before mob1" << std::endl;
						interception_date_route1 += solution.evaluate(interceptor_first->position(),interception_date_route1,*interceptor_first,solution.begin(*interceptor_first)->_mobile,problem.mobiles()[mobile_it_first->_prev]);
						interceptor_position_first = problem.mobiles()[mobile_it_first->_prev].position(interception_date_route1);

						//std::cout << "route1: date prev mob1: " << interception_date_route1 << std::endl;
					}
					// first swap: interception of mobile_it_second
					interception_date_route1 += interceptor_first->computeInterception(interceptor_position_first, mobile_it_second->_mobile, interception_date_route1);
					interceptor_position_first = mobile_it_second->_mobile.position(interception_date_route1);

					//std::cout << "route1: mob2 catched: " << interception_date_route1 << std::endl;

					// if there is mobiles after mobile_it_first
					if(mobile_it_first->_next != -1)
					{
						//std::cout << "route1: after mob1" << std::endl;
						//std::cout << mobile_it_first->_next << " " << solution.lastOfRoute(*interceptor_first)._mobile.id() << std::endl;
						interception_date_route1 += solution.evaluate(interceptor_position_first, interception_date_route1, *interceptor_first, problem.mobiles()[mobile_it_first->_next], solution.lastOfRoute(*interceptor_first)._mobile);

						//std::cout << "route1: after mob2: " << interception_date_route1 << std::endl;
					}

					//------------------------------------------------
					// computes interception date for the second route
					interception_date_route2 = 0;
					interceptor_position_second = interceptor_second->position();

					// if there is mobiles before mobile_it_second
					if(mobile_it_second->_prev != -1)
					{
						//std::cout << "route2: before mob2" << std::endl;
						interception_date_route2 += solution.evaluate(interceptor_second->position(),interception_date_route2,*interceptor_second,solution.begin(*interceptor_second)->_mobile,problem.mobiles()[mobile_it_second->_prev]);
						interceptor_position_second = problem.mobiles()[mobile_it_second->_prev].position(interception_date_route2);

						//std::cout << "route2: date prev mob2: " << interception_date_route2 << std::endl;
					}
					// second swap: interception of mobile_it_first
					interception_date_route2 += interceptor_second->computeInterception(interceptor_position_second, mobile_it_first->_mobile, interception_date_route2);
					interceptor_position_second = mobile_it_first->_mobile.position(interception_date_route2);

					//std::cout << "route2: mob1 catched: " << interception_date_route2 << std::endl;

					// if there is mobiles after mobile_it_second
					if(mobile_it_second->_next != -1)
					{
						//std::cout << "route2: after mob2" << std::endl;
						//std::cout << mobile_it_second->_next << " " << solution.lastOfRoute(*interceptor_second)._mobile.id() << std::endl;
						interception_date_route2 += solution.evaluate(interceptor_position_second, interception_date_route2, *interceptor_second, problem.mobiles()[mobile_it_second->_next], solution.lastOfRoute(*interceptor_second)._mobile);

						//std::cout << "route2: after mob2: " << interception_date_route2 << std::endl;
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

	assert(round(1.0e+6*_best_interception_date) == round(1.0e+6*std::max(solution.lastInterceptionTime(*_best_interceptor1),solution.lastInterceptionTime(*_best_interceptor2))));
	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
