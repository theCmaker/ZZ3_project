#ifndef __MS_ELS_HPP__
#define __MS_ELS_HPP__

#include <vector>

template <unsigned max_it_ms,
		  unsigned max_it_els,
		  unsigned max_cp,
		  class ELS,
		  class PROBLEM,
		  class SOLUTION,
		  class CMP = std::less<SOLUTION> >
class MS_ELS
{
public:
	MS_ELS();
	~MS_ELS();

	SOLUTION operator() (const PROBLEM & pb);
};

#include "ms_els.cxx"
#endif
