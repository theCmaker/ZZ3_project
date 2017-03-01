#ifndef __MS_ELS_HPP__
#define __MS_ELS_HPP__

#include <vector>
#include "vnd.hpp"

template <unsigned max_it_ms, unsigned max_it_els, unsigned max_cp, typename ELS>
class MS_ELS
{
private:

public:
	MS_ELS();
	~MS_ELS();

	Solution run(Problem & pb);
};

#include "ms_els.cxx"
#endif
