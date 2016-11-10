#include "heuristic_sequence.hpp"
#include <cmath>
#include <limits>

Heuristic_sequence::Heuristic_sequence(const Problem & p) : Heuristic(p)
{
	_name = "Sequence";
}

Heuristic_sequence::~Heuristic_sequence() {}

void Heuristic_sequence::run()
{
	// Mobiles
	unsigned j;		// Current interceptor index

	// Interceptors
	VInterceptors::const_iterator best_interceptor;	// Best interceptor (the quickest one to catch the "best mobile")
	std::vector<Location> interceptor_location; // Last position for each interceptor
	std::vector<Time> interceptor_time;			// Last interception time for each interceptor

	// Time
	Time total_time;		// Local variable to store the total_time for current interceptor
	Time best_time = 0.;	// Time needed by best interceptor to catch best mobile
	Time needed_time;		// Time needed by current interceptor to catch current mobile

	// Angles
	double alpha;			// Direction to follow for the interceptor


	// Compute the last known position and time for each interceptor
	for (VInterceptors::const_iterator i = _problem.interceptors().begin(); i != _problem.interceptors().end(); ++i)
	{
		interceptor_location.push_back(i->position());
		interceptor_time.emplace_back(0.);
	}

	for (VMobiles::const_iterator current_mobile = _problem.mobiles().begin(); current_mobile != _problem.mobiles().end(); ++ current_mobile)
	{
		best_time = std::numeric_limits<Time>::infinity();		// Guess there is no solution
		for (VInterceptors::const_iterator current_interceptor = _problem.interceptors().begin(); current_interceptor != _problem.interceptors().end(); ++current_interceptor)
		{	// Walk through the interceptors
			needed_time = current_interceptor->computeInterception(interceptor_location[j],*current_mobile,interceptor_time[j],alpha); // Compute interception time for current mobile by current interceptor
			if (needed_time >= 0 && needed_time < best_time)
			{  // Current mobile is catchable before best mobile
				best_time = needed_time;				// Backup best time
				//best_alpha = alpha;					// Backup best angle
				best_interceptor = current_interceptor; // Backup best interceptor
			}
		}
		if (std::isfinite(best_time))
		{	// The mobile can be caughtt
			j = best_interceptor->id();
			total_time = interceptor_time[j] + best_time;

			// Update local data on interceptors
			interceptor_location[j] = current_mobile->position(total_time);
			interceptor_time[j] = total_time;

			// Update the solution
			_solution.append(*best_interceptor,*current_mobile,total_time);
		}
	}
}
