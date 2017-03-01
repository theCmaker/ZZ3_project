#include "ms_els.hpp"

MS_ELS::MS_ELS() {}

MS_ELS::~MS_ELS() {}

Solution * MS_ELS::run(Solution & sol, int max_it_ms, int max_it_els, int max_cp, VND vnd)
{
	Solution * record = nullptr;
	for(int nb_it_ms = 0; nb_it_ms < max_it_ms; ++nb_it_ms)
	{
		// sol = sol.random(pb);
		record = &sol;
		// ELS
		for(int nb_it_els = 0; nb_it_els < max_it_els; ++nb_it_els)
		{
			Solution best_local_sol = sol;

			//copies
			for(int nb_cp = 0; nb_cp < max_cp; ++nb_cp)
			{
				Solution copy(sol);
				copy.shake();
				vnd.run(copy);
				if(copy.worstInterceptionTime() < best_local_sol.worstInterceptionTime())
				{
					best_local_sol = copy;
				}
				if(best_local_sol.worstInterceptionTime() < record->worstInterceptionTime())
				{
					record = &best_local_sol;
				}
				sol = best_local_sol;
			}
		}
	}
	return record;
}
