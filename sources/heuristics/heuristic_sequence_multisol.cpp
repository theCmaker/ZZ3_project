#include "heuristic_sequence_multisol.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

Heuristic_Sequence_MultiSol::Heuristic_Sequence_MultiSol(const Problem & p) : Heuristic(p)
{
	_name = "Sequence MultiSolutions";
}

Heuristic_Sequence_MultiSol::~Heuristic_Sequence_MultiSol() {}

void Heuristic_Sequence_MultiSol::run()
{
	std::vector<unsigned> vec(_problem.nbMobiles());
	std::iota(vec.begin(),vec.end(),0u);
	run(vec);
}

void Heuristic_Sequence_MultiSol::run(const std::vector<unsigned> & sequence)
{
	// Mobiles
	const Mobile * current_mobile;
	unsigned j;		// Current interceptor index

	// Interceptors
	VInterceptors::const_iterator best_interceptor;	// Best interceptor (the quickest one to catch the "best mobile")
	std::vector<Location> interceptor_location; // Last position for each interceptor
	std::vector<Time> interceptor_time;			// Last interception time for each interceptor

	// Time
	Time total_time;		// Local variable to store the total_time for current interceptor
	Time best_time = 0.;	// Time needed by best interceptor to catch best mobile
	Time needed_time;		// Time needed by current interceptor to catch current mobile

	// Compute the last known position and time for each interceptor
	for (VInterceptors::const_iterator i = _problem.interceptors().begin(); i != _problem.interceptors().end(); ++i)
	{
		interceptor_location.push_back(i->position());
		interceptor_time.emplace_back(0.);
	}

	for (auto mob_index : sequence)
	{
		current_mobile = &(_problem.mobiles()[mob_index]);
		best_time = std::numeric_limits<Time>::infinity();		// Guess there is no solution
		for (VInterceptors::const_iterator current_interceptor = _problem.interceptors().begin(); current_interceptor != _problem.interceptors().end(); ++current_interceptor)
		{	// Walk through the interceptors
			j = current_interceptor->id();
			needed_time = current_interceptor->computeInterception(interceptor_location[j],*current_mobile,interceptor_time[j]); // Compute interception time for current mobile by current interceptor
			if (needed_time >= 0 && needed_time < best_time)
			{  // Current interceptor can catch current mobile earlier
				best_time = needed_time;				// Backup best time
				best_interceptor = current_interceptor; // Backup best interceptor
			}
		}
		if (std::isfinite(best_time))
		{	// The mobile can be caught
			j = best_interceptor->id();
			total_time = interceptor_time[j] + best_time;

			// Update local data on interceptors
			interceptor_location[j] = current_mobile->position(total_time);
			interceptor_time[j] = total_time;

			// Update the solution
			_solution.append(*best_interceptor,*current_mobile,total_time);

			_solutions.push_back(_solution);
		}
	}
}

std::vector<Solution> &Heuristic_Sequence_MultiSol::solutions()
{
	return _solutions;
}

const std::vector<Solution> &Heuristic_Sequence_MultiSol::solutions() const
{
	return _solutions;
}
