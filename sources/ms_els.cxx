#include <algorithm>

template <unsigned max_it_ms,
		  unsigned max_it_els,
		  unsigned max_cp,
		  class LS,
		  class PROBLEM,
		  class SOLUTION,
		  class CMP>
MS_ELS<max_it_ms, max_it_els, max_cp, LS, PROBLEM, SOLUTION, CMP>::MS_ELS() {}

template <unsigned max_it_ms,
		  unsigned max_it_els,
		  unsigned max_cp,
		  class LS,
		  class PROBLEM,
		  class SOLUTION,
		  class CMP>
MS_ELS<max_it_ms, max_it_els, max_cp, LS, PROBLEM, SOLUTION, CMP>::~MS_ELS() {}

template <unsigned max_it_ms,
		  unsigned max_it_els,
		  unsigned max_cp,
		  class LS,
		  class PROBLEM,
		  class SOLUTION,
		  class CMP>
SOLUTION MS_ELS<max_it_ms, max_it_els, max_cp, LS, PROBLEM, SOLUTION, CMP>::operator() (const PROBLEM & pb)
{
	LS local_search;
	CMP is_better;

	SOLUTION sol = SOLUTION::random(pb);
	SOLUTION record = sol;

	// Multi-Start
	for(unsigned nb_it_ms = 0; nb_it_ms < max_it_ms; ++nb_it_ms)
	{
		sol = SOLUTION::random(pb);

		LS::before(sol);
		// ELS runs
		for(unsigned nb_it_els = 0; nb_it_els < max_it_els; ++nb_it_els)
		{
			SOLUTION best_local_sol = sol;

			// Copies
			for(unsigned nb_cp = 0; nb_cp < max_cp; ++nb_cp)
			{
				SOLUTION copy(sol);
				copy.shake();
				local_search(copy);
				if (is_better(copy,best_local_sol)) {
					best_local_sol = copy;
				}
			}

			if (is_better(best_local_sol,record)) {
				record = best_local_sol;
			}
			sol = best_local_sol;
		}
	}
	return record;
}
