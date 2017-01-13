bool InsertMove::scan(const Solution & solution)
{
	Time interception_time;
	double alpha;
	Time best_overhead_time = std::numeric_limits<Time>::infinity();	// cost of time in the sequence after insertion of mobile_in
	//Time current_overhead_time; // current cost of time
	
	// for each interceptors
	for(auto interceptor : _p.interceptors())
	{
		// calcul de la date d'interception en premiere position de la sequence
		interception_time = interceptor.computeInterception(interceptor.position(), _mobile_in, 0., alpha);
		// iterateur sur la sequence
		Solution::iterator it_mobile_after = solution.begin(interceptor);
		// tant qu'on peut insérer mobile_in et qu'il reste qu'on n'a pas parcouru tous les mobiles
		while(std::isfinite(interception_time) && it_mobile_after != solution.end(interceptor))
		{
			// tant qu'on ne rejette aucun mobile (== tps total fini) et qu'on n'a pas parcouru tous les mobiles
			// iterateur sur les mobiles
			Solution::iterator it_mobile = it_mobile_after;
			
			while(std::isfinite(interception_time) && it_mobile != solution.end(interceptor))
			{
				// calcul de la nouvelle date d'interception avec interception du mobile courant
				interception_time += interceptor.computeInterception(mobile.position(), *it_mobile, interception_time, alpha);
				++it_mobile;
			} 
			
			if(std::isfinite(interception_time) && interception_time < best_overhead_time)
			{
				_mobile_next = *it_mobile_after;
				_best_interceptor = interceptor;
				best_overhead_time = interception_time;
			}
			
			interception_time = interceptor.computeInterception(solution->catch_position(it_mobile_after->_mobile), _mobile_in, it_mobile_after->_date, alpha);
			++it_mobile_after;
			// reste un truc à update
			
		}
		// si on doit calculer pour tous les points d'insertion, il faut encore calculer pour le dernier point (cas où it_mobile_after = -1)
		
	}
	
}
