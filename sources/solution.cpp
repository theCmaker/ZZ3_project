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
	_next(-1)
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
	if (_first == -1) //Liste intercepteurs vide
	{
		_first = interceptor_index;
		_last = interceptor_index;
	} else if (inter_sequence->_first == -1) {
		_interceptors[(unsigned) _last]._next = interceptor_index;
		_last = interceptor_index;
	}
	if (inter_sequence->_first == -1) //Cas liste mobiles vide
	{
		inter_sequence->_first = mobile_index;
	} else { //Cas classique
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
	return _sequence[(unsigned) _interceptors[i.id()]._last]._date;
}

unsigned Solution::bestInterceptionCount() const
{
	unsigned best = 0u;
	unsigned count;
	for (VInterceptors::const_iterator interceptor = _problem.interceptors().cbegin(); interceptor != _problem.interceptors().cend(); ++interceptor) {
		count = 0u;
		for (Solution::iterator mobile = begin(*interceptor);
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
		for (Solution::iterator mobile = begin(*interceptor);
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

bool Solution::isEmpty(const Interceptor & i) const
{
	return (_interceptors[i.id()]._first == -1);
}

Solution::iterator Solution::begin(const Interceptor & i) const
{
	return Solution::iterator(&(_sequence[0]) + _interceptors[i.id()]._first);
}

Solution::iterator Solution::end(const Interceptor & i) const
{
	return Solution::iterator(&(_sequence[0]) - 1);
}

//******************************************************
// ITERATORS
//******************************************************

Solution::iterator::iterator(const Solution::MobileNode * s) :
	_solution(s)
{}

Solution::iterator::~iterator() {}

Solution::MobileNode Solution::iterator::operator* ()
{
	return *_solution;
}

const Solution::MobileNode * Solution::iterator::operator-> ()
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
