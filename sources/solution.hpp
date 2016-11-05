#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__

#include <list>
#include "problem.hpp"

class Solution 
{
public:
	typedef struct _solution
	{
		Time			_date;
		const Mobile  & _mobile;
		const Interceptor * _interceptor;

		//Mobiles
		int _next;
		int _prev;

		_solution(const Mobile & m, const Time d, const Interceptor * i) : _date(d), _mobile(m), _interceptor(i), _next(-1), _prev(-1)  {};
		_solution(const Mobile & m) : _date(-1.), _mobile(m), _interceptor(nullptr), _next(-1), _prev(-1)  {};

	} solution_t;

	typedef struct _interceptorseq {
		const Interceptor & _interceptor;
		
		//Mobiles
		int _first;
		int _last;

		//Interceptors
		int _next;

		_interceptorseq(const Interceptor & i) : _interceptor(i), _first(-1), _last(-1), _next(-1) {};
	} interceptor_t;
private:
	const Problem & _problem;
	std::vector<solution_t> 	_sequence;
	std::vector<interceptor_t>  _interceptors;

	//Interceptors
	int _first;
	int _last;

public:
	Solution(const Problem &);
	~Solution();

	void append(unsigned, unsigned, const Time &);
	void append(const Interceptor &, const Mobile &, const Time &);
	Time last_interception_time(int) const ;

	int first() const;
	int last() const;

	interceptor_t operator[] (unsigned);
	const interceptor_t operator[] (unsigned) const;

	bool is_caught (const Mobile &) const;
	
	solution_t mobile(int i);
	const solution_t mobile(int i) const;
};

std::ostream & operator<< (std::ostream &, const Solution &);

#endif
