#include "tikzifyer.hpp"
#include <limits>
#include <cmath>

std::vector<std::string> Tikzifyer::_styles({"interceptor"});

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
	std::string	color_options;
	bool		caught_flag;
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
	for (auto depot = t.problem().depots().begin(); depot != t.problem().depots().end(); ++depot)
	{
		int interlabel_angle = 360 / depot->interceptors().size();	// Angle between two consecutive labels
		double label_distance = std::max(0.4,std::min(1.,depot->interceptors().size()*0.05));	// Label distance to the symbol, in [0.4,1]
		int label_angle = 315;	// -45 degrees, default angle
		for (auto interceptor : depot->interceptors())
		{
			o << R"(\node[interceptor] (I)" << interceptor->id() << ") at " << interceptor->position() << R"( {\interceptor};)" << std::endl;	// Symbol
			o << R"(\node[interceptor] at ($ (I)" << interceptor->id() << ") + (" << label_angle << ":" << label_distance << ") $) {$I_" << interceptor->id() << "$};" << std::endl;		// Labels
			label_angle += interlabel_angle;
		}
	}

	// Mobiles
	for (VMobiles::const_iterator mobile = t.mobiles().begin(); mobile != t.mobiles().end(); ++mobile)
	{
		// Compute the pen color for drawing the mobile
		caught_flag = false;
		for (auto s : t.solutions())
		{
			caught_flag = caught_flag || s->is_caught(*mobile);
		}
		color_options = (caught_flag ? "" : ",color=red");

		o << R"(\node[mobile,anchor=center)" << color_options << "] (M" << mobile->id() << ") at " << mobile->position() << R"( {\mobile};)" << std::endl; // Symbol
		o << R"(\node[mobile)" << color_options <<"] at (M" << mobile->id() << ".south east) {$M_{" << mobile->id() << "}$};" << std::endl; // Label
		if (mobile->speed() > 0.)
		{
		  o << R"(\draw[speed)" << color_options << "] (M" << mobile->id() << ".center) -- ($ (M" << mobile->id() << ".center) + " << mobile->direction() << " $);" << std::endl; // Arrow
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
				interceptor_coords = interception_coords; // Update interceptor's position
			}
			// End date
			if (! solution->isEmpty(*interceptor))
			{
				o << R"(\draw[)" << Tikzifyer::style(solution_index) << "]" << interceptor_coords << " node[anchor=" << ((interceptor_coords._y - 1 < ymin)?"south":"north") << " "
					<< ((interceptor_coords._x - 3 < xmin)?"west":"east") <<"] {$t_{" << interceptor->id() << "}="
					<< solution->last_interception_time(*interceptor) << "$};" << std::endl;
			}
		}
		++solution_index;
	}
	return o;
}
