#include "ms_els.hpp"
#include <algorithm>
#include "heuristic_sequence.hpp"

MS_ELS::MS_ELS() {}

MS_ELS::~MS_ELS() {}

Solution MS_ELS::run(Problem & pb, int max_it_ms, int max_it_els, int max_cp, VND vnd)
{
	std::vector<unsigned> mobile_seq;

	for(unsigned i = 0; i < pb.nbMobiles(); ++i)
	{
		mobile_seq.push_back(i);
	}

	Solution record(pb);
	// Multi-Start
	for(int nb_it_ms = 0; nb_it_ms < max_it_ms; ++nb_it_ms)
	{
		// Shuffle the sequence of mobiles
		std::random_shuffle(mobile_seq.begin(),mobile_seq.end());
		// Run the heuristic sequence
		Heuristic_sequence h(pb);
		h.run(mobile_seq);
		// Create the solution

		Solution sol = h.solution();
		record = sol;
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
				if(best_local_sol.worstInterceptionTime() < record.worstInterceptionTime())
				{
					record = best_local_sol;
				}
				sol = best_local_sol;
			}
		}
	}
	return record;
}
