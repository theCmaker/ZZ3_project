#include "solution.hpp"
#include "heuristic_sequence.hpp"
#include <random>
#include <algorithm>

//******************************************************
// INTERNAL STRUCTS
//******************************************************

//Mobile
Solution::MobileNode::MobileNode(const Mobile &m, const Time d, const Interceptor *i, int prev, int next) :
	_date(d),
	_mobile(m),
	_interceptor(i),
	_next(next),
	_prev(prev)
{}

Solution::MobileNode::MobileNode(const Mobile &m) :
	_date(-1.),
	_mobile(m),
	_interceptor(nullptr),
	_next(-1),
	_prev(-1)
{}

//Interceptor
Solution::InterceptorNode::InterceptorNode(const Interceptor &i, int first, int last, int prev, int next) :
	_interceptor(i),
	_first(first),
	_last(last),
	_next(next),
	_prev(prev)
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

Solution::Solution(const Solution & s) : _problem(s._problem), _sequence(s._sequence), _interceptors(s._interceptors), _first(s._first), _last(s._last)
{}

Solution::~Solution() {}

Solution &Solution::operator=(const Solution & other) {
	if ((this != &other) && (&_problem) == (&(other._problem))) {
		_sequence.clear();
		for (std::vector<MobileNode>::const_iterator itr = other._sequence.begin(); itr != other._sequence.end(); ++itr) {
			_sequence.emplace_back(itr->_mobile, itr->_date, itr->_interceptor, itr->_prev, itr->_next);
		}
		_interceptors.clear();
		for (std::vector<InterceptorNode>::const_iterator itr = other._interceptors.begin(); itr != other._interceptors.end(); ++itr) {
			_interceptors.emplace_back(itr->_interceptor, itr->_first, itr->_last, itr->_prev, itr->_next);
		}
		_first = other._first;
		_last = other._last;
	}
	return *this;
}

const Problem & Solution::problem() const
{
	return _problem;
}

void Solution::shake()
{
	// Check that solution is not empty, otherwise shake is useless.
	if (_first != -1) {
		static std::mt19937 rand;

		// Get the sequence
		std::vector<unsigned> sequence;
		for (VInterceptors::const_iterator interceptor = _problem.interceptors().begin(); interceptor != _problem.interceptors().end(); ++interceptor) {
			for (iterator interception = begin(*interceptor); interception != end(*interceptor); ++interception) {
				sequence.push_back(interception->_mobile.id());
			}
		}

		// Shake the sequence
		// Realise (min 2, max n/6) swap operations between mobiles in the sequence
		for (unsigned i = 0; i < std::max(2u, (unsigned) sequence.size()/6u); ++i) {
			std::swap(sequence[sequence.size() * (rand() / rand.max())],
					  sequence[sequence.size() * (rand() / rand.max())]);
		}



		// Rebuild the solution
		Heuristic_sequence h(_problem);
		h.run(sequence);
		*this = h.solution();
	}
}

Solution Solution::random(const Problem &p)
{
	// Initialize the sequence and shake it
	std::vector<unsigned> mobile_seq(p.nbMobiles());
	std::iota(mobile_seq.begin(),mobile_seq.end(),0);
	std::random_shuffle(mobile_seq.begin(),mobile_seq.end());

	// Run the heuristic sequence
	Heuristic_sequence h(p);
	h.run(mobile_seq);

	// Return the solution
	return h.solution();
}

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

const Solution::MobileNode &Solution::firstOfRoute(const Interceptor & i) const
{
	return _sequence[_interceptors[i.id()]._first];
}

const Solution::MobileNode &Solution::lastOfRoute(const Interceptor & i) const
{
	return _sequence[_interceptors[i.id()]._last];
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
	int next_mobile_index = _sequence[prev_mobile_index]._next;
	InterceptorNode * inter_sequence = &(_interceptors[interceptor_index]);

	_sequence[mobile_index]._prev = (int) prev_mobile_index;
	_sequence[mobile_index]._next = next_mobile_index;
	_sequence[prev_mobile_index]._next = (int) mobile_index;
	if (next_mobile_index == -1) {
		// Current insertion is done at the end of the route
		inter_sequence->_last = mobile_index;
	} else {
		_sequence[next_mobile_index]._prev = mobile_index;
	}
	//Interception info
	_sequence[mobile_index]._date = d;
	_sequence[mobile_index]._interceptor = &(inter_sequence->_interceptor);
}

void Solution::insertAfter(const Mobile & m_prev, const Interceptor & i, const Mobile & m, const Time & d)
{
	insertAfter(m_prev.id(), i.id(), m.id(), d);
}

void Solution::appendSeq(const Interceptor & i, const Mobile & m)
{
	int interceptor_index = i.id();
	InterceptorNode * inter_sequence = &(_interceptors[interceptor_index]);
	int mobile_index = m.id();
	if (inter_sequence->_first == -1) {
		// New route needs to be created
		if (_first == -1) {
			// Empty list of routes
			_first = interceptor_index;
		} else {
			// Append it to the list of routes
			_interceptors[(unsigned) _last]._next = interceptor_index;
		}
		// Chain the new element
		inter_sequence->_prev = (int) _last;
		inter_sequence->_next = -1;
		// Update the end of the list of routes
		_last = interceptor_index;
		inter_sequence->_first = mobile_index;
	} else {
		// Already existing route
		// Simple case
		_sequence[(unsigned) inter_sequence->_last]._next = mobile_index;
	}
	_sequence[mobile_index]._prev = inter_sequence->_last;

	MobileNode * mobile_it = &(_sequence[mobile_index]);
	while(mobile_it->_next != -1)
	{
		mobile_it->_interceptor = &i;
		mobile_it = &(_sequence[mobile_it->_next]);
	}
	mobile_it->_interceptor = &i;
	inter_sequence->_last = mobile_it->_mobile.id();
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
			// Next route exists, re-chain previous route
			_interceptors[next_route]._prev = prev_route;
		} else {
			// No route after, update _last
			_last = prev_route;
		}
		if (prev_route != -1) {
			// Previous route exists, re-chain next route
			_interceptors[prev_route]._next = next_route;
		} else {
			// No route before, update _first
			_first = next_route;
		}
		// Reset the route chainings
		_interceptors[interceptor->id()]._next = -1;
		_interceptors[interceptor->id()]._prev = -1;
		_interceptors[interceptor->id()]._first = -1;
		_interceptors[interceptor->id()]._last = -1;
	}

	// Reset the interception info
	m_node._next = -1;
	m_node._prev = -1;
	m_node._date = -1.;
	m_node._interceptor = nullptr;
}

void Solution::removeSeqFrom(const Mobile & m)
{
	MobileNode & m_node = _sequence[m.id()];
	const Interceptor * interceptor = m_node._interceptor;

	if (m_node._prev == -1) {
		// Node is the first of the route, all the nodes will be removed
		_interceptors[interceptor->id()]._first = -1;
		//_interceptors[interceptor->id()]._last = -1;
	}

	_interceptors[interceptor->id()]._last = m_node._prev;

	if (_interceptors[interceptor->id()]._first == -1 && _interceptors[interceptor->id()]._last == -1) {
		// Route is now empty, we need to remove if from the list of the routes.
		int next_route = _interceptors[interceptor->id()]._next;
		int prev_route = _interceptors[interceptor->id()]._prev;
		if (next_route != -1) {
			_interceptors[next_route]._prev = prev_route;
		} else {
			_last = prev_route;
		}
		if (prev_route != -1) {
			_interceptors[prev_route]._next = next_route;
		} else {
			_first = next_route;
		}
		_interceptors[interceptor->id()]._next = -1;
		_interceptors[interceptor->id()]._prev = -1;
	}
}

Time Solution::recomputeFrom(unsigned mobile_index)
{
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
		interception_date += interceptor.computeInterception(interceptor_position, itr->_mobile, interception_date);
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

Time Solution::evaluate(const Location & start_pos, const Time start_date, const Interceptor & interceptor, const Mobile & first_mobile, const Mobile & last_mobile) const
{
	Time interception_date = start_date;

	Location interceptor_position = start_pos;

	Solution::const_iterator itr(&(_sequence[first_mobile.id()]));
	Solution::const_iterator itr_end(&(_sequence[last_mobile.id()]));
	++itr_end; // Put the iterator just after the last mobile so it can be caught.

	while (itr != itr_end) {
		interception_date += interceptor.computeInterception(interceptor_position, itr->_mobile, interception_date);
		interceptor_position = itr->_mobile.position(interception_date);
		++itr;
	}

	return interception_date - start_date;
}

Time Solution::worstInterceptionTime() const
{
	Time worst_duration = 0.;
	Time duration;
	for (VInterceptors::const_iterator interceptor = _problem.interceptors().cbegin(); interceptor != _problem.interceptors().cend(); ++interceptor) {
		duration = lastInterceptionTime(*interceptor);
		if (worst_duration < duration) {
			worst_duration = duration;
		}
	}
	return worst_duration;
}

const Interceptor & Solution::worstRoute() const
{
	Time duration, worst_duration = 0.;
	const Interceptor * i = nullptr;
	for (VInterceptors::const_iterator interceptor = _problem.interceptors().cbegin(); interceptor != _problem.interceptors().cend(); ++interceptor) {
		duration = lastInterceptionTime(*interceptor);
		if (worst_duration < duration) {
			worst_duration = duration;
			i = &(*interceptor);
		}
	}
	return *i;
}

Time Solution::lastInterceptionTime(int interceptor_index) const
{
	return _sequence[(unsigned) _interceptors[(unsigned) interceptor_index]._last]._date;
}

Time Solution::lastInterceptionTime(const Interceptor & i) const
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

bool Solution::isCaught(const Mobile & m) const
{
	return (_sequence[m.id()]._date >= 0.);
}

Location Solution::catchPosition (const Mobile & m) const
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

std::vector<const Mobile *> Solution::uncaughtMobiles() const
{
	std::vector<const Mobile *> res;
	for (VMobiles::const_iterator i = _problem.mobiles().begin(); i != _problem.mobiles().end(); ++i) {
		if (!isCaught(*i)) {
			res.push_back(&(*i));
		}
	}
	return res;
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

bool Solution::check() const
{
	//TODO: check dates
	bool ok = true;
	std::vector<unsigned> mobiles(_problem.nbMobiles());
	std::vector<unsigned> interceptors(_problem.nbInterceptors());

	for (auto & i : mobiles) {
		i = 0;
	}
	for (auto & i : interceptors) {
		i = 0;
	}

	int route = _first;
	while (route != -1 && interceptors[route] == 0) {
		interceptors[route]++;
		const_iterator itr = begin(_problem.interceptors()[route]);
		while (itr != end(_problem.interceptors()[route]) && mobiles[itr->_mobile.id()] == 0) {
			mobiles[itr->_mobile.id()]++;
			++itr;
		}
		if (itr != end(_problem.interceptors()[route])) {
			ok = false;
			std::cerr << "Mobile " << itr->_mobile << " found twice." << std::endl;
		}

		//Check dates
		if (lastInterceptionTime(route) != evaluate(_problem.interceptors()[route].position(),0.,_problem.interceptors()[route],firstOfRoute(_problem.interceptors()[route])._mobile,lastOfRoute(_problem.interceptors()[route])._mobile)) {
			std::cerr << "Dates for route " << route << " are wrong." << std::endl;
			ok = false;
		}
		route = _interceptors[route]._next;
	}
	if (route != -1) {
		ok = false;
		std::cerr << "Route for interceptor " << route << " found twice." << std::endl;
	}

	//TODO: check all the uncaught mobiles and unused routes. They must be empty and clean.
	for (auto & route : _interceptors) {
		if (interceptors[route._interceptor.id()] == 0) {
			if (route._next != -1) {
				ok = false;
				std::cerr << "Route " << route._interceptor.id() << ": Next must be -1." << std::endl;
			}
			if (route._prev != -1) {
				ok = false;
				std::cerr << "Route " << route._interceptor.id() << ": Prev must be -1." << std::endl;
			}
			if (route._first != -1) {
				ok = false;
				std::cerr << "Route " << route._interceptor.id() << ": First must be -1." << std::endl;
			}
			if (route._last != -1) {
				ok = false;
				std::cerr << "Route " << route._interceptor.id() << ": Last must be -1." << std::endl;
			}
		}
	}

	for (auto & interception : _sequence) {
		if (mobiles[interception._mobile.id()] == 0) {
			if (interception._next != -1) {
				ok = false;
				std::cerr << "Interception " << interception._mobile.id() << ": Next must be -1." << std::endl;
			}
			if (interception._prev != -1) {
				ok = false;
				std::cerr << "Interception " << interception._mobile.id() << ": Prev must be -1." << std::endl;
			}
			if (interception._date != -1) {
				ok = false;
				std::cerr << "Interception " << interception._mobile.id() << ": Date must be -1." << std::endl;
			}
			if (interception._interceptor != nullptr) {
				ok = false;
				std::cerr << "Interception " << interception._mobile.id() << ": Interceptor must be NULL." << std::endl;
			}
		}
	}
	return ok;
}

bool Solution::operator<(const Solution &other) const {
	return worstInterceptionTime() < other.worstInterceptionTime();
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
		o << "\t[D" << s[interceptor_id]._interceptor.depot()->id() << ";pos" << s[interceptor_id]._interceptor.position()
		  << ";t(0)]" << std::endl;
		while (mobile_id != -1)
		{
			o << "\t[" << s.mobile(mobile_id)._mobile.id() << ";pos" << s.mobile(mobile_id)._mobile.position(s.mobile(mobile_id)._date) << ";t(" << s.mobile(mobile_id)._date << ")]" << std::endl;
			mobile_id = s.mobile(mobile_id)._next;
		}
		Solution::MobileNode last = s.lastOfRoute(s.problem().interceptors()[interceptor_id]);
		o << "\t[D" << s[interceptor_id]._interceptor.depot()->id() << ";pos" << s[interceptor_id]._interceptor.position()
		  << ";t(" << last._date + s[interceptor_id]._interceptor.timeFromTo(last._mobile.position(last._date), s[interceptor_id]._interceptor.position())
		  << ")]" << std::endl;
		o << "]" << std::endl;
		interceptor_id = s[interceptor_id]._next;
	}
	return o;
}
