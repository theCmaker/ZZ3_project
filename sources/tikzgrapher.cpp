#include "tikzgrapher.hpp"
#include <cmath>

std::vector<std::string> TikzGrapher::_styles({"interceptor"});

TikzGrapher::TikzGrapher()
{
}

void TikzGrapher::operator()(const Solution & s)
{
	_solutions.push_back(&s);
	addToBounds(Location(s.worstInterceptionTime(),s.bestInterceptionCount()));
}

void TikzGrapher::operator()(const Problem & p)
{
	_problem = &p;
	addToBounds(Location());
}

const std::string & TikzGrapher::style(unsigned index)
{
	return TikzGrapher::_styles[index % TikzGrapher::_styles.size()];
}

void TikzGrapher::addStyle(const std::string & style)
{
	TikzGrapher::_styles.push_back(style);
}

std::ostream &operator<<(std::ostream & o, const TikzGrapher & t)
{
	unsigned count; // Number of mobiles in the current route
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

	// Curves
	const Solution * solution;
	Location position;
	unsigned route_index = 0u;
	for (VPSolutions::const_iterator solutionptr = t.solutions().begin();
		 solutionptr != t.solutions().end();
		 ++solutionptr)
	{
		solution = *solutionptr;
		for (VInterceptors::const_iterator interceptor = t.interceptors().begin(); interceptor != t.interceptors().end(); ++interceptor)
		{
			position._x = 0.;
			position._y = 0.;
			count = 0u;

			if (! solution->isEmpty(*interceptor))
			{
				o << R"(\draw[)" << TikzGrapher::style(route_index) << "]" << position;

				for (Solution::iterator step = solution->begin(*interceptor);
					 step != solution->end(*interceptor);
					 ++step)
				{
					position._x = step->_date;
					position._y = ++count;
					o << " -| " << position;

				}
				o << ";" << std::endl;
				position._y -= 1.;

				o << R"(\draw[dashed,)" << TikzGrapher::style(route_index) << "]" << position;
				position._y = 0.;

				o << " -- " << position << " node[anchor=" << ((position._y - 1 < ymin)?"south":"north") << " "
						<< ((position._x - 3 < xmin)?"west":"east") <<"] {$t_{" << interceptor->id() << "}="
						<< solution->last_interception_time(*interceptor) << "$};" << std::endl;
			}
			++route_index;
		}
	}
	return o;
}
