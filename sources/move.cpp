#include <cmath>
#include "move.hpp"

// implementation of Move

Move::Move(Problem & p) : _p(p) {}

Move::~Move() {}

// implementation of InsertMove

InsertMove::InsertMove(Problem & p, const Mobile & m_in, int m_prev, const Interceptor & i) : Move(p), _mobile_in(m_in), _mobile_prev(m_prev), _interceptor(i), _interception_date(-1) {}
InsertMove::~InsertMove() {}
		
bool InsertMove::scan(const Solution & solution)
{
	bool improved = true;
	Time interception_time = 0;
	// iterator of the first MobileNode of the route (case: insertion ahead)
	Solution::const_iterator mobile_it = solution.begin(_interceptor);
	Location interceptor_position = _interceptor.position();
	
	std::cout << "scan init" << std::endl;
	
	if(_mobile_prev >= 0)
	{
		// computes time of interception of mobile_prev (mobile before the insertion)
		interception_time = solution.mobile(_mobile_prev)._date;
		
		// increments the iterator to the mobile after _mobile_prev
		const Solution::MobileNode & mobile_tmp = solution.mobile(_mobile_prev);
		mobile_it = Solution::const_iterator(&mobile_tmp);
		++mobile_it;
		
		// new position of the interceptor
		interceptor_position = _p.mobiles()[_mobile_prev].position(interception_time);
	}
	
	// sums with the interception time of the mobile inserted 
	interception_time += _interceptor.computeInterception(interceptor_position,
										 				  _mobile_in,
														  interception_time);
														  
														  
	interceptor_position = _p.mobiles()[_mobile_in.id()].position(interception_time);
	// saves the interception time for the commit
	_interception_date = interception_time;
	
	
						  
	// checks if there is no problem in the route after the insertion
	// and computes the interception time for each following mobile
	while(mobile_it != solution.end(_interceptor) && std::isfinite(interception_time))
	{
		interception_time += _interceptor.computeInterception(interceptor_position,
															  mobile_it->_mobile,
															  interception_time);
															  
		
		interceptor_position = _p.mobiles()[mobile_it->_mobile.id()].position(interception_time);
		++mobile_it;
	}
	
	std::cout << "Worst interception time: " << solution.worstInterceptionTime() << std::endl;
	std::cout << "New interception time: " << interception_time << std::endl;
									 
	// compares time of interception between the update route and the worse route (in time)
	if (!std::isfinite(interception_time) || solution.worstInterceptionTime() < interception_time)
	{
		improved = false;
	}
	
	std::cout << "scan finished" << std::endl;
	
	return improved;
}

void InsertMove::commit(Solution & solution)
{
	// insertion ahead or creation of a route
	if(_mobile_prev < 0)
	{
		solution.prepend(_interceptor, _mobile_in, _interception_date);
	}
	// if insertion at end
	else if(solution.mobile(_mobile_prev)._next < 0)
	{
		solution.append(_interceptor, _mobile_in, _interception_date);
	}
	else
	{
		solution.insertAfter(_mobile_prev, _interceptor, _mobile_in, _interception_date);
	}
	solution.recomputeFrom(_mobile_in);
}

// implementation of DeleteMove
DeleteMove::DeleteMove(Problem & p, const Mobile & m_out, int m_prev, const Interceptor & i) : Move(p), _mobile_out(m_out), _mobile_prev(m_prev), _interceptor(i) {}
DeleteMove::~DeleteMove() {}

bool DeleteMove::scan(const Solution & solution)
{
	bool improved = true;
	Time interception_time = 0;
	// iterator of the first MobileNode of the route (case: insertion ahead)
	Solution::const_iterator mobile_it = solution.begin(_interceptor);
	Location interceptor_position = _interceptor.position();
	
	std::cout << "scan init" << std::endl;
	
	if(_mobile_prev >= 0)
	{
		// computes time of interception of mobile_prev (mobile before the suppression)
		interception_time = solution.mobile(_mobile_prev)._date;
		
		// new position of the interceptor
		interceptor_position = _p.mobiles()[_mobile_prev].position(interception_time);
	}
														  
	interceptor_position = _p.mobiles()[_mobile_prev].position(interception_time);
	
	// the iterator points to the mobile after _mobile_out (the mobile to delete of the route)
	const Solution::MobileNode & mobile_tmp = solution.mobile(_mobile_out.id());
	mobile_it = Solution::const_iterator(&mobile_tmp);
	++mobile_it;
						  
	// checks if there is no problem in the route after the insertion
	// and computes the interception time for each following mobile
	while(mobile_it != solution.end(_interceptor) && std::isfinite(interception_time))
	{
		interception_time += _interceptor.computeInterception(interceptor_position,
															  mobile_it->_mobile,
															  interception_time);
															  
		
		interceptor_position = _p.mobiles()[mobile_it->_mobile.id()].position(interception_time);
		++mobile_it;
	}
	
	std::cout << "Worst interception time: " << solution.worstInterceptionTime() << std::endl;
	std::cout << "New interception time: " << interception_time << std::endl;
									 
	// compares time of interception between the update route and the worse route (in time)
	if (!std::isfinite(interception_time) || solution.worstInterceptionTime() < interception_time)
	{
		improved = false;
	}
	
	std::cout << "scan finished" << std::endl;
	
	return improved;
}

void DeleteMove::commit(Solution & solution)
{
	solution.remove(_mobile_out);
	solution.recomputeFrom(_mobile_prev);
}

// implementation of CrossMove
CrossMove::CrossMove(Problem & p, int m0, int m1) : Move(p) 
{
	_mobile[0] = m0;
	_mobile[1] = m1;
}

CrossMove::~CrossMove() {}

bool CrossMove::scan(const Solution & solution)
{
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

void CrossMove::commit(Solution & solution)
{
	
}

