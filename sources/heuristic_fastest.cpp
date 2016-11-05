#include "heuristic_fastest.hpp"
#include <cmath>
#include <limits>

Heuristic_fastest::Heuristic_fastest(const Problem & p) : Heuristic(p)
{
	_name = "Fastest";
}

Heuristic_fastest::~Heuristic_fastest() {}

void Heuristic_fastest::run()
{
	// Mobiles
	int uncaught_mobiles;				// Uncaught mobiles counter
	unsigned i;							// Current mobile index
	unsigned j;							// Current interceptor index
	const Mobile * best_mobile;			// Best mobile (the one that costs the shortest time to be caught)
	const Mobile * current_mobile;		// Mobile that is being inspected

	// Interceptors
	const Interceptor * best_interceptor;		// Best interceptor (the quickest one to catch the "best mobile")
	const Interceptor * current_interceptor;	// Interceptor that is being inspected
	std::vector<Location> interceptor_location; // Last position for each interceptor
	std::vector<Time> interceptor_time;			// Last interception time for each interceptor

	// Time
	Time total_time;		// Local variable to store the total_time for current interceptor
	Time best_time = 0.;	// Time needed by best interceptor to catch best mobile
	Time needed_time;		// Time needed by current interceptor to catch current mobile

	// Angles
	double alpha;			// Direction to follow for the interceptor
	//double best_alpha;		// Direction followed by the best interceptor to catch the best mobile


	// Initialization
	best_interceptor = nullptr;					// Best interceptor is currently unknown
	best_mobile = nullptr;						// Best mobile is currently unknown
	uncaught_mobiles = _problem.nbMobiles();	// Maximum number of uncaught mobiles
	// Compute the last known position and time for each interceptor
	for (VInterceptors::const_iterator i = _problem.interceptors().begin(); i != _problem.interceptors().end(); ++i)
	{
		interceptor_location.push_back(i->position());
		interceptor_time.emplace_back(0.);
	}

	while (uncaught_mobiles > 0 && std::isfinite(best_time))
	{	// There are still some uncaught mobiles
		i = 0;
		best_time = std::numeric_limits<Time>::infinity();		// Guess there is no solution
		//TODO: Use iterators instead of indexes i/j ?
		while (i < _problem.nbMobiles())
		{	// Walk through the mobiles
			current_mobile = &(_problem.mobiles()[i]); //&(_solution.mobile(i)._mobile);	// Define the mobile candidate
			if (! _solution.is_caught(*current_mobile))
			{	// The mobile has not been caught yet
				j = 0;
				while (j < _problem.nbInterceptors())
				{	// Walk through the interceptors
					current_interceptor = &(_problem.interceptors()[j]);	// Define the interceptor candidate
					needed_time = current_interceptor->compute_interception(interceptor_location[j],*current_mobile,interceptor_time[j],alpha); // Compute interception time for current mobile by current interceptor
					if (needed_time >= 0 && needed_time < best_time)
					{  // Current mobile is catchable before best mobile
						best_time = needed_time;				// Backup best time
						best_mobile = current_mobile;			// Backup best mobile
						//best_alpha = alpha;					// Backup best angle
						best_interceptor = current_interceptor; // Backup best interceptor
					}
					++j; // Jump to next interceptor
				}
			}
			++i; // Jump to next mobile
		}
		if (std::isfinite(best_time))
		{	// A mobile is caught
			// Some readable variables
			j = best_interceptor->id();
			total_time = interceptor_time[j] + best_time;

			// Update local data on interceptors
			interceptor_location[j] = best_mobile->position(total_time);
			interceptor_time[j] = total_time;

			// Update the solution
			_solution.append(*best_interceptor,*best_mobile,total_time);
			uncaught_mobiles--; // One more mobile has been caught
		}
	}
}
