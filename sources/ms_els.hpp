#ifndef __MS_ELS_HPP__
#define __MS_ELS_HPP__

#include <vector>
#include "vnd.hpp"

class MS_ELS
{
private:

public:
	MS_ELS();
	~MS_ELS();

	Solution run(Problem & pb, int max_it_ms, int max_it_els, int max_cp, VND vnd);
};


#endif
