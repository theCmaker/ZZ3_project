#include "tikzifyer.hpp"
#include <limits>
#include <cmath>

std::vector<std::string> Tikzifyer::_styles;
Tikzifyer::Initializer Tikzifyer::__init;

Tikzifyer::Tikzifyer() :
	_top_left(std::numeric_limits<Distance>::infinity(), -std::numeric_limits<Distance>::infinity()),
	_bottom_right(-std::numeric_limits<Distance>::infinity(), std::numeric_limits<Distance>::infinity()),
	_problem(NULL)
{}

Tikzifyer::~Tikzifyer() {}

const Problem & Tikzifyer::problem() const
{
	return *_problem;
}

const VPSolutions & Tikzifyer::solutions() const
{
	return _solutions;
}

const VMobiles & Tikzifyer::mobiles() const
{
	return _problem->mobiles();
}

const VInterceptors & Tikzifyer::interceptors() const
{
	return _problem->interceptors();
}

Distance Tikzifyer::xmin() const
{
	return _top_left._x;
}

Distance Tikzifyer::xmax() const
{
	return _bottom_right._x;
}

Distance Tikzifyer::ymin() const
{
	return _bottom_right._y;
}

Distance Tikzifyer::ymax() const
{
	return _top_left._y;
}

const std::string & Tikzifyer::style(unsigned index)
{
	return Tikzifyer::_styles[index % Tikzifyer::_styles.size()];
}

void Tikzifyer::addStyle(const std::string & style)
{
	Tikzifyer::_styles.push_back(style);
}

void Tikzifyer::clear()
{
	_top_left = Location(std::numeric_limits<Distance>::infinity(), -std::numeric_limits<Distance>::infinity());
	_bottom_right = Location(-std::numeric_limits<Distance>::infinity(), std::numeric_limits<Distance>::infinity());
	_problem = NULL;
	_solutions.clear();
}

void Tikzifyer::addToBounds(const Location & l)
{
	if (l._x < _top_left._x)
	{
		_top_left._x = l._x;
	}
	else if (l._x > _bottom_right._x)
	{
		_bottom_right._x = l._x;
	}
	if (l._y < _bottom_right._y)
	{
		_bottom_right._y = l._y;
	}
	else if (l._y > _top_left._y)
	{
		_top_left._y = l._y;
	}
}

void Tikzifyer::operator() (const Problem & p)
{
	_problem = &p;
	for (VMobiles::const_iterator mobile = p.mobiles().begin(); mobile != p.mobiles().end(); mobile++)
	{
		addToBounds(mobile->position());
	}
	for (VInterceptors::const_iterator interceptor = p.interceptors().begin(); interceptor != p.interceptors().end(); ++interceptor)
	{
		addToBounds(interceptor->position());
	}
	for (VDepots::const_iterator depot = p.depots().begin(); depot != p.depots().end(); ++depot)
	{
		addToBounds(depot->position());
	}
}

void Tikzifyer::operator() (const Solution & s)
{
	_solutions.push_back(&s);
	for (VMobiles::const_iterator mobile = _problem->mobiles().begin(); mobile != _problem->mobiles().end(); mobile++)
	{
		if (s.is_caught(*mobile))
		{
			addToBounds(s.catch_position(*mobile));
		}
	}
}

std::ostream & operator<< (std::ostream & o, const Tikzifyer & t) {
	int xmin = int(floor(t.xmin())),
		xmax = int(ceil(t.xmax())),
		ymin = int(floor(t.ymin())),
		ymax = int(ceil(t.ymax()));
	// Grid
	o << R"(\draw[grided,step=1.0,thin] ()" << xmin << "," << ymin << ") grid (" << xmax << ',' << ymax << ");" << std::endl;

	// Axes
	o << R"(\draw[color=Gray] ()" << xmin << ",0) -- coordinate (x axis mid) (" << xmax << ",0);" << std::endl;	// Abscissa
	o << R"(\draw[color=Gray] (0,)" << ymin << ") -- coordinate (x axis mid) (0," << ymax << ");" << std::endl;	// Ordinates
	o << R"(\foreach \x in {)" << xmin << ",...," << xmax << "}" << std::endl << R"(\draw[color=Gray] (\x,1pt) -- (\x,-3pt) node[anchor=north] {\x};)" << std::endl;	//Abscissa labels
	o << R"(\foreach \y in {)" << ymin << ",...," << ymax << "}" << std::endl << R"(\draw[color=Gray] (1pt,\y) -- (-3pt,\y) node[anchor=east] {\y};)" << std::endl;	// Ordinates labels

	// Interceptors
	for (VInterceptors::const_iterator interceptor = t.interceptors().begin(); interceptor != t.interceptors().end(); ++interceptor)
	{
		o << R"(\node[interceptor] (I)" << interceptor->id() << ") at " << interceptor->position() << R"( {\interceptor};)" << std::endl; 	// Symbol
		o << R"(\node[interceptor] at (I)" << interceptor->id() << ".south east) {$I_" << interceptor->id() << "$};" << std::endl; 		// Labels
		//TODO: Define label position from the number of interceptors on the repo
	}

	// Mobiles
	for (VMobiles::const_iterator mobile = t.mobiles().begin(); mobile != t.mobiles().end(); ++mobile)
	{
		//TODO: if the mobile is never caught, change the color
		o << R"(\node[mobile,anchor=center] (M)" << mobile->id() << ") at " << mobile->position() << R"( {\mobile};)" << std::endl; // Symbol
		o << R"(\node[mobile] at (M)" << mobile->id() << ".south east) {$M_" << mobile->id() << "$};" << std::endl; // Label
		if (mobile->speed() > 0.)
		{
		  o << R"(\draw[speed] (M)" << mobile->id() << ".center) -- ($ (M" << mobile->id() << ".center) + " << mobile->direction() << " $);" << std::endl; // Arrow
		}
	}

	// Interception paths
	Location interception_coords, interceptor_coords;
	const Solution * solution;
	unsigned solution_index = 0;
	for (VPSolutions::const_iterator solutionptr = t.solutions().begin(); solutionptr != t.solutions().end(); ++solutionptr)
	{
		solution = *solutionptr;
		for (VInterceptors::const_iterator interceptor = t.interceptors().begin(); interceptor != t.interceptors().end(); ++interceptor)
		{
			interceptor_coords = interceptor->position();
			for (Solution::iterator step = solution->begin(*interceptor); step != solution->end(*interceptor); ++step)
			{
				interception_coords = step->_mobile.position(step->_date);
				if (step->_mobile.speed() > 0.) // Draw the path taken by the mobile
				{
					o << R"(\draw[camino] (M)" << step->_mobile.id() << ".center) -- " << interception_coords << ";" << std::endl;
				}
				o << R"(\draw[)" << Tikzifyer::style(solution_index) << "] " << interceptor_coords << " -- " << interception_coords << ';' << std::endl;	// Draw the interceptor path
				o << R"(\node[interceptor] at )" << interception_coords << R"( {\mobile};)" << std::endl;	// Mark the interception place
				//o << R"(\node[caught] at (M)" << step->_mobile.id() << R"() {\mobile};)" << std::endl; 		// Update mobile state
				interceptor_coords = interception_coords; // Update interceptor's position
			}
			// End date
			if (! solution->isEmpty(*interceptor))
			{
				o << R"(\draw[interceptor] )" << interceptor_coords << " node[anchor=" << ((interceptor_coords._y - 1 < ymin)?"south":"north") << " " 
					<< ((interceptor_coords._x - 3 < xmin)?"west":"east") <<"] {$t_{" << interceptor->id() << "}=" 
					<< solution->last_interception_time(*interceptor) << "$};" << std::endl;
			}
		}
		++solution_index;
	}
	return o;
}
