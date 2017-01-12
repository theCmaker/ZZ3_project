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
	double alpha;
	// iterator of the first MobileNode of the route (case: insertion ahead)
	Solution::iterator mobile_it = solution.begin(_interceptor);
	Location interceptor_position = _interceptor.position();
	
	if(_mobile_prev >= 0)
	{
		// computes time of interception of mobile_prev (mobile before the insertion)
		interception_time = solution.mobile(_mobile_prev)._date;
		
		// iterator of the mobile next mobile_prev
		// TODO: UGLY -> prettify this immediatly !!!
		Solution::MobileNode mobile_tmp = solution.mobile(solution.mobile(_mobile_prev)._next);
		mobile_it = Solution::iterator(&mobile_tmp);
		
		// new position of the interceptor
		interceptor_position = _p.mobiles()[_mobile_prev].position(interception_time);
	}
	
	// sums with the interception time of the mobile inserted 
	interception_time += _interceptor.computeInterception(interceptor_position,
										 				  _mobile_in,
														  interception_time,
														  alpha);
	interceptor_position = _p.mobiles()[_mobile_in.id()].position(interception_time);
	_interception_date = interception_time;
														  
	// checks if there is no problem in the route after the insertion
	// and computes the interception time for each following mobile
	while(mobile_it != solution.end(_interceptor) && std::isfinite(interception_time))
	{
		interception_time += _interceptor.computeInterception(interceptor_position,
															  mobile_it->_mobile,
															  interception_time,
															  alpha);
		interceptor_position = _p.mobiles()[mobile_it->_mobile.id()].position(interception_time);
		++mobile_it;
	}
									 
	// compares time of interception between the update route and the worse route (in time)
	if (!std::isfinite(interception_time) || solution.worst_interception_time() < interception_time)
	{
		improved = false;
	}
	
	return improved;
}

void InsertMove::commit(Solution & solution)
{
	// insertion ahead or creation of a route
	if(_mobile_prev < 0)
	{
		// solution.prepend(...) TODO
	}
	// if insertion at end
	else if(solution.mobile(_mobile_prev)._next < 0)
	{
		solution.append(_interceptor, _mobile_in, _interception_date);
	}
	else
	{
		// solution.insertAfter(...) TODO
	}
}
