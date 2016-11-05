#include "heuristic.hpp"

Heuristic::Heuristic(const Problem & p) : _problem(p), _solution(p), _name("Abstract") {}

Heuristic::~Heuristic() {}

const Solution & Heuristic::solution() const
{
	return _solution;
}

const Problem & Heuristic::problem() const
{
	return _problem;
}

const std::string Heuristic::name() const
{
	return _name;
}

std::ostream & operator<< (std::ostream & o, const Heuristic & h)
{
	o << "Heuristic " << h.name() << std::endl;
	o << h.solution() << std::endl;
	return o;
}