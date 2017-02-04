#include <cmath>
#include <limits>
#include <algorithm>

template <class CachePolicy>
Heuristic_fastest<CachePolicy>::Heuristic_fastest(const Problem & p) : Heuristic(p)
{
	_name = "Fastest";
}

template <class CachePolicy>
Heuristic_fastest<CachePolicy>::~Heuristic_fastest() {}

template <class CachePolicy>
void Heuristic_fastest<CachePolicy>::run() {
	this->run<CachePolicy::isCached>();
}

template <class CachePolicy>
template <bool isCached>
void Heuristic_fastest<CachePolicy>::run()
{
	if (isCached)
	{
		// Mobiles
		std::vector<unsigned> uncaught_mobiles(_problem.nbMobiles());	// Uncaught mobiles counter
		unsigned j;							// Current interceptor index
		const Mobile * best_mobile;			// Best mobile (the one that costs the shortest time to be caught)
		std::vector<unsigned>::iterator best_mobile_itr;
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

		// Initialization
		best_interceptor = nullptr;					// Best interceptor is currently unknown
		best_mobile = nullptr;						// Best mobile is currently unknown
		std::iota(uncaught_mobiles.begin(),uncaught_mobiles.end(),0);
		// Compute the last known position and time for each interceptor and fill the cache
		CachePolicy::init(_problem.nbInterceptors(),_problem.nbMobiles());
		for (VInterceptors::const_iterator i = _problem.interceptors().begin(); i != _problem.interceptors().end(); ++i)
		{
			interceptor_location.push_back(i->position());
			interceptor_time.emplace_back(0.);
			for (VMobiles::const_iterator j = _problem.mobiles().begin(); j != _problem.mobiles().end(); ++j) {
				CachePolicy::set(i->id(),j->id(),i->computeInterception(interceptor_location.back(),*j,0.)); // Compute interception time for current mobile by current interceptor
			}
		}

		while (!uncaught_mobiles.empty() > 0 && std::isfinite(best_time))
		{	// There are still some uncaught mobiles
			//i = 0;
			best_time = std::numeric_limits<Time>::infinity();		// Guess there is no solution
			for (std::vector<unsigned>::iterator i = uncaught_mobiles.begin(); i != uncaught_mobiles.end(); ++i)
			{	// Walk through the uncaught mobiles
				current_mobile = &(_problem.mobiles()[*i]);		// Define the mobile candidate
				j = 0;
				while (j < _problem.nbInterceptors())
				{	// Walk through the interceptors
					current_interceptor = &(_problem.interceptors()[j]);	// Define the interceptor candidate
					needed_time = CachePolicy::get(current_interceptor->id(),current_mobile->id());
					if (needed_time >= 0 && needed_time < best_time)
					{  // Current mobile is catchable before best mobile
						best_time = needed_time;				// Backup best time
						best_mobile = current_mobile;			// Backup best mobile
						best_mobile_itr = i;					// Backup mobile position iterator
						best_interceptor = current_interceptor; // Backup best interceptor
					}
					++j; // Jump to next interceptor
				}
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

				// Update the cache
				uncaught_mobiles.erase(best_mobile_itr);
				// For all the other uncaught mobiles, recompute the catch distance for the best interceptor
				for (std::vector<unsigned>::iterator mob = uncaught_mobiles.begin(); mob != uncaught_mobiles.end(); ++mob)
				{
					// Compute interception time for current mobile by current interceptor
					CachePolicy::set(j,*mob,best_interceptor->computeInterception(interceptor_location[j],_problem.mobiles()[*mob],interceptor_time[j]));
				}
			}
		}
	} else {
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
						needed_time = current_interceptor->computeInterception(interceptor_location[j],*current_mobile,interceptor_time[j]); // Compute interception time for current mobile by current interceptor
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
}
