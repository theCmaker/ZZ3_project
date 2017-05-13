#include <cmath>
#include <assert.h>

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
		//std::cout << "interceptor: " << interceptor_id << std::endl;
		interceptor = &(problem.interceptors()[interceptor_id]);
		//_best_interception_date = solution.lastInterceptionTime(*interceptor);
		mobile_it_first = solution.begin(*interceptor);
		// for each mobile in the route
		while(mobile_it_first != solution.end(*interceptor) && Policy::keepOn())
		{
			mobile_it_second = mobile_it_first;
			++mobile_it_second;
			//std::cout << "mobile 1: " << mobile_it_first->_mobile.id() << std::endl;


			// for each mobile in the route (after mobile_it_first)
			while(mobile_it_second != solution.end(*interceptor) && Policy::keepOn())
			{
				//std::cout << "mobile 2: " << mobile_it_second->_mobile.id() << std::endl;
				//std::cout << "date begin: " << interception_date << std::endl;

				interception_date = 0;
				interceptor_position = interceptor->position();
				// if there is mobiles between the begining and mobile_it_first
				if(mobile_it_first->_prev != -1)
				{
					//std::cout << "before mob1" << std::endl;
					interception_date = solution.mobile(mobile_it_first->_prev)._date;
					interceptor_position = problem.mobiles()[mobile_it_first->_prev].position(interception_date);

					//std::cout << "date prev mob1: " << interception_date << std::endl;
				}
				// first swap: interception of mobile_it_second
				interception_date += interceptor->computeInterception(interceptor_position, mobile_it_second->_mobile, interception_date);
				interceptor_position = mobile_it_second->_mobile.position(interception_date);

				//std::cout << "mob2 catched: " << interception_date << std::endl;

				// if there is mobiles between mobile_it_first and mobile_it_second
				if(mobile_it_first->_next != (int)mobile_it_second->_mobile.id())
				{
					//std::cout << "between mob1 & 2" << std::endl;

					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[mobile_it_first->_next], problem.mobiles()[mobile_it_second->_prev]);
					interceptor_position = problem.mobiles()[mobile_it_second->_prev].position(interception_date);

					//std::cout << "between mob1 & 2: "<< interception_date << std::endl;
				}

				// second swap: interception of mobile_it_first
				interception_date += interceptor->computeInterception(interceptor_position, mobile_it_first->_mobile, interception_date);
				interceptor_position = mobile_it_first->_mobile.position(interception_date);

				//std::cout << "mob1 catched: " << interception_date << std::endl;

				// if there is mobiles after mobile_it_second
				if(mobile_it_second->_next != -1)
				{
					//std::cout << "after mob2" << std::endl;
					//std::cout << mobile_it_second->_next << " " << solution.lastOfRoute(*interceptor)._mobile.id() << std::endl;
					interception_date += solution.evaluate(interceptor_position, interception_date, *interceptor, problem.mobiles()[mobile_it_second->_next], solution.lastOfRoute(*interceptor)._mobile);

					//std::cout << "after mob2: " << interception_date << std::endl;
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
	//TODO: recompute from swap2 may be more efficient
	solution.recomputeFrom(solution[_best_interceptor->id()]._first);

	assert(round(1.0e+6*_best_interception_date) == round(1.0e+6*solution.lastInterceptionTime(*_best_interceptor)));
	assert((!std::isnan(solution.worstInterceptionTime())));
	assert((std::isfinite(solution.worstInterceptionTime())));
}
