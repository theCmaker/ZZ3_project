#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "solution.hpp"

class Move
{
	protected:
		Problem & _p;
		
	public:
		Move(Problem &);
		virtual ~Move();
		
		virtual bool scan(const Solution &) = 0;
		virtual void commit(Solution &) = 0;
};

class InsertMove : public Move
{
	private:
		const Mobile & 			_mobile_in;		// mobile to insert
		int						_mobile_prev;	// insertion of the mobile will be after this one
		const Interceptor & 	_interceptor;	// interceptor for the wanted route 
		
	public:
		InsertMove(Problem &, const Mobile &, int, const Interceptor &);
		virtual ~InsertMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

class DeleteMove : public Move
{
	private:
		
	public:
		DeleteMove(Problem &);
		virtual ~DeleteMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

class CrossMove : public Move
{
	private:
		
	public:
		CrossMove(Problem &);
		virtual ~CrossMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

class ReplaceMove : public Move
{
	private:
		
	public:
		ReplaceMove(Problem &);
		virtual ~ReplaceMove();
		
		virtual bool scan(const Solution &);
		virtual void commit(Solution &);
};

#endif
