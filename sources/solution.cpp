#include "solution.hpp"

//******************************************************
// INTERNAL STRUCTS
//******************************************************

//Mobile
Solution::MobileNode::MobileNode(const Mobile &m, const Time d, const Interceptor *i) :
	_date(d),
	_mobile(m),
	_interceptor(i),
	_next(-1),
	_prev(-1)
{}

Solution::MobileNode::MobileNode(const Mobile &m) :
	_date(-1.),
	_mobile(m),
	_interceptor(nullptr),
	_next(-1),
	_prev(-1)
{}

//Interceptor
Solution::InterceptorNode::InterceptorNode(const Interceptor &i) :
	_interceptor(i),
	_first(-1),
	_last(-1),
	_next(-1),
	_prev(-1)
{}

//******************************************************
// Constructor & Destructor
//******************************************************

Solution::Solution(const Problem & p) : _problem(p), _first(-1), _last(-1)
{
	for (VMobiles::const_iterator i = p.mobiles().begin(); i != p.mobiles().end(); ++i)
	{
		_sequence.emplace_back(*i);
	}
	for (VInterceptors::const_iterator i = p.interceptors().begin(); i != p.interceptors().end(); ++i)
	{
		_interceptors.emplace_back(*i);
	}
}

Solution::~Solution() {}

//******************************************************
// Methods
//******************************************************

int Solution::first() const
{
	return _first;
}

int Solution::last() const
{
	return _last;
}

void Solution::append(unsigned interceptor_index, unsigned mobile_index, const Time & d)
{
	InterceptorNode * inter_sequence = &(_interceptors[interceptor_index]);
	if (_first == -1)
	{
		// No route
		_first = interceptor_index;
		_last = interceptor_index;
	} else if (inter_sequence->_first == -1) {
		// Empty route, add it to the list
		_interceptors[(unsigned) _last]._next = interceptor_index;
		inter_sequence->_prev = (int) _last;
		_last = interceptor_index;
	}
	if (inter_sequence->_first == -1)
	{
		// Empty route
		inter_sequence->_first = mobile_index;
	} else {
		// Simple case
		_sequence[(unsigned) inter_sequence->_last]._next = mobile_index;
	}
	_sequence[mobile_index]._prev = inter_sequence->_last;
	_sequence[mobile_index]._next = -1;
	inter_sequence->_last = mobile_index;
	//Interception info
	_sequence[mobile_index]._date = d;
	_sequence[mobile_index]._interceptor = &(inter_sequence->_interceptor);
}

void Solution::append(const Interceptor & i, const Mobile & m, const Time & d)
{
	append(i.id(), m.id(), d);
}

void Solution::prepend(unsigned interceptor_index, unsigned mobile_index, const Time &d)
{
	InterceptorNode * inter_sequence = &(_interceptors[interceptor_index]);
	if (_first == -1)
	{
		// Empty list of routes
		_first = interceptor_index;
		_last = interceptor_index;
	} else if (inter_sequence->_first == -1) {
		// Empty route, add it to the list of non-empty routes.
		_interceptors[(unsigned) _last]._next = interceptor_index;
		inter_sequence->_prev = (int) _last;
		_last = interceptor_index;
	}
	if (inter_sequence->_first == -1)
	{
		// Empty route
		inter_sequence->_last = mobile_index;
	} else {
		// Classical case
		_sequence[(unsigned) inter_sequence->_first]._prev = mobile_index;
	}
	_sequence[mobile_index]._prev = -1;
	_sequence[mobile_index]._next = inter_sequence->_first;
	inter_sequence->_first = mobile_index;
	//Interception info
	_sequence[mobile_index]._date = d;
	_sequence[mobile_index]._interceptor = &(inter_sequence->_interceptor);
}

void Solution::prepend(const Interceptor & i, const Mobile & m, const Time & d)
{
	prepend(i.id(), m.id(), d);
}

void Solution::insertAfter(unsigned prev_mobile_index, unsigned interceptor_index, unsigned mobile_index, const Time & d)
{
	InterceptorNode * inter_sequence = &(_interceptors[interceptor_index]);

	_sequence[mobile_index]._prev = (int) prev_mobile_index;
	_sequence[mobile_index]._next = _sequence[prev_mobile_index]._next;
	_sequence[prev_mobile_index]._next = (int) mobile_index;
	if (_sequence[mobile_index]._next == -1) {
		// Current insertion is done at the end of the route
		inter_sequence->_last = mobile_index;
	}
	//Interception info
	_sequence[mobile_index]._date = d;
	_sequence[mobile_index]._interceptor = &(inter_sequence->_interceptor);
}

void Solution::insertAfter(const Mobile & m_prev, const Interceptor & i, const Mobile & m, const Time & d)
{
	insertAfter(m_prev.id(), i.id(), m.id(), d);
}

void Solution::remove(const Mobile &m)
{
	remove(m.id());
}

void Solution::remove(unsigned mobile_index)
{
	MobileNode & m_node = _sequence[mobile_index];
	const Interceptor * interceptor = m_node._interceptor;

	if (m_node._prev == -1) {
		// Node is the first of the route
		_interceptors[interceptor->id()]._first = m_node._next;
	} else {
		_sequence[m_node._prev]._next = m_node._next;
	}

	if (m_node._next == -1) {
		// Node is the last of the route
		_interceptors[interceptor->id()]._last = m_node._prev;
	} else {
		_sequence[m_node._next]._prev = m_node._prev;
	}

	if (m_node._next == -1 && m_node._prev == -1) {
		// Route is now empty, we need to remove if from the list of the routes.
		int next_route = _interceptors[interceptor->id()]._next;
		int prev_route = _interceptors[interceptor->id()]._prev;
		if (next_route != -1) {
			_interceptors[next_route]._prev = prev_route;
		} else {
			_last = next_route;
		}
		if (prev_route != -1) {
			_interceptors[prev_route]._next = next_route;
		} else {
			_first = next_route;
		}
	}

	m_node._next = -1;
	m_node._prev = -1;
	m_node._date = -1.;
	m_node._interceptor = nullptr;
}

Time Solution::recomputeFrom(unsigned mobile_index)
{
	double alpha;
	const Interceptor & interceptor = *(_sequence[mobile_index]._interceptor);
	Solution::iterator itr(&(_sequence[mobile_index]));

	// Assume the mobile is the first of the route. Get the interceptor departure data.
	Time interception_date = 0.;
	Location interceptor_position = interceptor.position();

	if (itr->_prev != -1) {
		// The mobile is not the first of the route. Get the previous mobile interception data.
		interception_date = _sequence[itr->_prev]._date;
		interceptor_position = _sequence[itr->_prev]._mobile.position(interception_date);
	}

	// Recompute the end of the route.
	while (itr != end(interceptor)) {
		interception_date += interceptor.computeInterception(interceptor_position, itr->_mobile, interception_date,alpha);
		itr->_date = interception_date;
		interceptor_position = itr->_mobile.position(itr->_date);
		++itr;
	}

	return interception_date;
}

Time Solution::recomputeFrom(const Mobile & m)
{
	return recomputeFrom(m.id());
}

Time Solution::worstInterceptionTime() const
{
	Time worst_duration = 0.;
	Time duration;
	for (VInterceptors::const_iterator interceptor = _problem.interceptors().cbegin(); interceptor != _problem.interceptors().cend(); ++interceptor) {
		duration = last_interception_time(*interceptor);
		if (worst_duration < duration) {
			worst_duration = duration;
		}
	}
	return worst_duration;
}

Time Solution::last_interception_time(int interceptor_index) const
{
	return _sequence[(unsigned) _interceptors[(unsigned) interceptor_index]._last]._date;
}

Time Solution::last_interception_time(const Interceptor & i) const
{
	Time duration = -1.;
	if (_interceptors[i.id()]._last != -1) {
		duration = _sequence[(unsigned) _interceptors[i.id()]._last]._date;
	}
	return duration;
}

unsigned Solution::bestInterceptionCount() const
{
	unsigned best = 0u;
	unsigned count;
	for (VInterceptors::const_iterator interceptor = _problem.interceptors().cbegin(); interceptor != _problem.interceptors().cend(); ++interceptor) {
		count = 0u;
		for (Solution::const_iterator mobile = begin(*interceptor);
			 mobile != end(*interceptor);
			 ++mobile)
		{
			++count;
		}
		if (count > best) {
			best = count;
		}
	}
	return best;
}

unsigned Solution::totalInterceptionCount() const
{
	unsigned count = 0u;
	for (VInterceptors::const_iterator interceptor = _problem.interceptors().cbegin();
		 interceptor != _problem.interceptors().cend();
		 ++interceptor)
	{
		for (Solution::const_iterator mobile = begin(*interceptor);
			 mobile != end(*interceptor);
			 ++mobile)
		{
			++count;
		}
	}
	return count;
}

Solution::InterceptorNode Solution::operator[] (unsigned i)
{
	return _interceptors[i];
}

const Solution::InterceptorNode Solution::operator[] (unsigned i) const
{
	return _interceptors[i];
}

bool Solution::is_caught(const Mobile & m) const
{
	return (_sequence[m.id()]._date >= 0.);
}

Location Solution::catch_position (const Mobile & m) const
{
	return m.position(_sequence[m.id()]._date);
}

Solution::MobileNode & Solution::mobile(unsigned i)
{
	return _sequence[i];
}

const Solution::MobileNode & Solution::mobile(unsigned i) const
{
	return _sequence[i];
}

Solution::iterator Solution::begin(const Interceptor & i)
{
	return Solution::iterator(&(_sequence[0]) + _interceptors[i.id()]._first);
}

Solution::iterator Solution::end(const Interceptor &)
{
	return Solution::iterator(&(_sequence[0]) - 1);
}

bool Solution::isEmpty(const Interceptor & i) const
{
	return (_interceptors[i.id()]._first == -1);
}

Solution::const_iterator Solution::begin(const Interceptor & i) const
{
	return Solution::const_iterator(&(_sequence[0]) + _interceptors[i.id()]._first);
}

Solution::const_iterator Solution::end(const Interceptor & i) const
{
	return Solution::const_iterator(&(_sequence[0]) - 1);
}

//******************************************************
// ITERATORS
//******************************************************

Solution::iterator::iterator(MobileNode *s) :
	_solution(s)
{}

Solution::iterator::~iterator() {}

Solution::MobileNode & Solution::iterator::operator* ()
{
	return *_solution;
}

Solution::MobileNode * Solution::iterator::operator-> ()
{
	return _solution;
}

Solution::iterator & Solution::iterator::operator++ ()
{
	_solution = _solution - int(_solution->_mobile.id()) + _solution->_next;
	return *this;
}

bool Solution::iterator::operator!= (Solution::iterator itr)
{
	return _solution != itr._solution;
}

//******************************************************
// CONST ITERATORS
//******************************************************

Solution::const_iterator::const_iterator(const MobileNode *s) :
	_solution(s)
{}

Solution::const_iterator::~const_iterator() {}

const Solution::MobileNode & Solution::const_iterator::operator* () const
{
	return *_solution;
}

const Solution::MobileNode * Solution::const_iterator::operator-> () const
{
	return _solution;
}

Solution::const_iterator & Solution::const_iterator::operator++ ()
{
	_solution = _solution - int(_solution->_mobile.id()) + _solution->_next;
	return *this;
}

bool Solution::const_iterator::operator!= (Solution::const_iterator itr)
{
	return _solution != itr._solution;
}

//******************************************************
// STANDARD OPERATORS
//******************************************************

std::ostream & operator<< (std::ostream & o, const Solution & s)
{
	int interceptor_id = s.first();
	int mobile_id = -1;
	while (interceptor_id != -1)
	{
		o << "[" << s[interceptor_id]._interceptor.id() << ":" << std::endl;
		mobile_id = s[interceptor_id]._first;
		while (mobile_id != -1)
		{
			o << "\t[" << s.mobile(mobile_id)._mobile.id() << ";pos" << s.mobile(mobile_id)._mobile.position(s.mobile(mobile_id)._date) << ";t(" << s.mobile(mobile_id)._date << ")]" << std::endl;
			mobile_id = s.mobile(mobile_id)._next;
		}
		o << "]" << std::endl;
		interceptor_id = s[interceptor_id]._next;
	}
	return o;
}
