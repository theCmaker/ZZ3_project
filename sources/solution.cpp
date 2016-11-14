#include "solution.hpp"

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
	interceptor_t * inter_sequence = &(_interceptors[interceptor_index]);
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

// void Solution::swap(int mobile1, int mobile2)
// {

// }

Time Solution::last_interception_time(int interceptor_index) const
{
	return _sequence[(unsigned) _interceptors[(unsigned) interceptor_index]._last]._date;
}

Time Solution::last_interception_time(const Interceptor & i) const
{
	return _sequence[(unsigned) _interceptors[i.id()]._last]._date;
}

Solution::interceptor_t Solution::operator[] (unsigned i)
{
	return _interceptors[i];
}

const Solution::interceptor_t Solution::operator[] (unsigned i) const
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

Solution::solution_t Solution::mobile(int i)
{
	return _sequence[i];
}

const Solution::solution_t Solution::mobile(int i) const
{
	return _sequence[i];
}

//******************************************************
// ITERATORS
//******************************************************

Solution::iterator::iterator(const Solution & s, const Interceptor & i) :
	_solution(s),
	_position(s._interceptors[i.id()]._first)
{}

Solution::iterator::iterator(const Solution & s, const Interceptor &, bool) :
	_solution(s),
	_position(-1)
{}

Solution::iterator::~iterator() {}

Solution::solution_t Solution::iterator::operator* ()
{
	return _solution._sequence[(unsigned) _position];
}

const Solution::solution_t * Solution::iterator::operator-> ()
{
	return &(_solution._sequence[(unsigned) _position]);
}

Solution::iterator & Solution::iterator::operator++ ()
{
	_position = _solution._sequence[(unsigned) _position]._next;
	return *this;
}

bool Solution::iterator::operator!= (Solution::iterator itr)
{
	return _position != itr._position;
}

Solution::iterator Solution::begin(const Interceptor & i) const
{
	return Solution::iterator(*this, i);
}

Solution::iterator Solution::end(const Interceptor & i) const
{
	return Solution::iterator(*this, i, false);
}

bool Solution::isEmpty(const Interceptor & i) const
{
	return (_interceptors[i.id()]._first == -1);
}

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
