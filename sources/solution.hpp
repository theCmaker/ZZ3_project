#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__

#include <list>
#include "problem.hpp"

class Solution
{
public:
	struct MobileNode
	{
		//Metadata
		Time				_date;
		const Mobile	  & _mobile;
		const Interceptor * _interceptor;

		//Mobiles chained list
		int _next;
		int _prev;

		//Constructors
		MobileNode(const Mobile &, const Time, const Interceptor *);
		MobileNode(const Mobile &);
	};

	struct InterceptorNode
	{
		//Metadata
		const Interceptor & _interceptor;

		//Mobiles
		int _first;
		int _last;

		//Interceptors
		int _next;

		//Constructors
		InterceptorNode(const Interceptor &);
	};

	class iterator
	{
	private:
		const MobileNode * _solution;
	public:
		iterator(const MobileNode *);
		~iterator();
		MobileNode operator* ();
		const MobileNode * operator-> ();
		iterator & operator++ ();
		bool operator!= (iterator);
	};
private:
	const Problem				  & _problem;
	std::vector<MobileNode>			_sequence;
	std::vector<InterceptorNode>	_interceptors;

	//Interceptors
	int _first;
	int _last;

public:
	Solution(const Problem &);
	~Solution();

	void append(unsigned, unsigned, const Time &);
	void append(const Interceptor &, const Mobile &, const Time &);
	Time last_interception_time() const;
	Time last_interception_time(int) const;
	Time last_interception_time(const Interceptor &) const;

	int first() const;
	int last() const;

	InterceptorNode operator[] (unsigned);
	const InterceptorNode operator[] (unsigned) const;

	bool is_caught (const Mobile &) const;
	Location catch_position (const Mobile &) const;

	MobileNode mobile(unsigned i);
	const MobileNode mobile(unsigned i) const;

	Solution::iterator begin(const Interceptor &) const;
	Solution::iterator end(const Interceptor &) const;
	bool isEmpty(const Interceptor &) const;
};

std::ostream & operator<< (std::ostream &, const Solution &);

#endif
