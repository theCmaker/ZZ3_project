#include "problem.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <random>


Problem::Problem() {}

Problem::Problem(unsigned nb_interceptors, unsigned nb_mobiles, unsigned nb_depots, bool)
{
	static std::mt19937_64 gen(std::random_device{}());
	unsigned depot_index;
	_interceptors.reserve(nb_interceptors);
	_mobiles.reserve(nb_mobiles);
	_depots.reserve(nb_depots);
	std::uniform_int_distribution<unsigned> depot(0u, nb_depots-1);
	std::uniform_real_distribution<Distance> position(-10., 10.);
	std::uniform_real_distribution<Distance> mob_speed(-2., 2.);
	std::uniform_real_distribution<Distance> inter_speed(2., 5.);
	for (unsigned i = 0; i < nb_mobiles; ++i)
	{
		_mobiles.emplace_back(position(gen),position(gen),mob_speed(gen),mob_speed(gen),i);
	}
	for (unsigned i = 0; i < nb_depots; ++i)
	{
		_depots.emplace_back(position(gen),position(gen),i);
	}
	for (unsigned i = 0; i < nb_interceptors; ++i)
	{
		depot_index = depot(gen);
		_interceptors.emplace_back(_depots[depot_index],inter_speed(gen),i);
		_depots[depot_index].addInterceptor(_interceptors.back());
	}
}

Problem::Problem(const char * filename)
{
	unsigned	nb_repos = 0,
				nb_mobiles = 0,
				nb_interceptors = 0,
				d = 0;
	Distance	x = 0,
				y = 0;
	Speed		dx = 0,
				dy = 0.,
				s = 0.;
	Time		r = 0.;
	std::string buf;
	std::ifstream ifs (filename,std::ifstream::in);
	if (!ifs.is_open()) {
		std::cout << "Could not read file." << std::endl;
	} else {
		if (ifs.eof()) {
			std::cout << "File is empty!" << std::endl;
		} else {
			while (std::getline(ifs,buf)) {
				if (buf.compare(0,8,"NB REPOS") == 0) {
					ifs >> nb_repos;
					_depots.reserve(nb_repos);
				} else if (buf.compare(0,10,"NB MOBILES") == 0) {
					ifs >> nb_mobiles;
					_mobiles.reserve(nb_mobiles);
				} else if (buf.compare(0,15,"NB INTERCEPTORS") == 0) {
					ifs >> nb_interceptors;
					_interceptors.reserve(nb_interceptors);
				} else if (buf.compare(0,7,"MOBILES") == 0) {
					unsigned i = 0;
					while (i < nb_mobiles && ifs >> x >> y >> dx >> dy) {
						_mobiles.emplace_back(x,y,dx,dy,i);
						++i;
					}
				} else if (buf.compare(0,12,"INTERCEPTORS") == 0) {
					unsigned i = 0;
					while (i < nb_interceptors && ifs >> d >> s >> r) {
						_interceptors.emplace_back(_depots[d],s,i,r);
						_depots[d].addInterceptor(_interceptors.back());
						++i;
					}
				} else if (buf.compare(0,5,"REPOS") == 0) {
					unsigned i = 0;
					while (i < nb_repos && ifs >> x >> y ) {
						_depots.emplace_back(x,y,i);
						++i;
					}
				}
			}
		}
		ifs.close();
	}
}

Problem::Problem(Problem &) {}

Problem::~Problem() {}


unsigned Problem::nbInterceptors() const
{
	return _interceptors.size();
}
unsigned Problem::nbMobiles() const
{
	return _mobiles.size();
}

unsigned Problem::nbDepots() const
{
	return _depots.size();
}


const VMobiles & Problem::mobiles() const
{
	return _mobiles;
}

const VInterceptors & Problem::interceptors() const
{
	return _interceptors;
}

const VDepots & Problem::depots() const
{
	return _depots;
}

void Problem::write(const char * filename) const
{
	std::ofstream output(filename);
	output << "NB REPOS" << std::endl;
	output << this->nbDepots() << std::endl;
	output << "REPOS" << std::endl;
	for (VDepots::const_iterator i = _depots.begin(); i != _depots.end(); ++i)
	{
		output << i->position()._x << ' ' << i->position()._y << std::endl;
	}
	output << "NB MOBILES" << std::endl;
	output << this->nbMobiles() << std::endl;
	output << "MOBILES" << std::endl;
	for (VMobiles::const_iterator i = _mobiles.begin(); i != _mobiles.end(); ++i)
	{
		output << i->position()._x << ' ' << i->position()._y << ' ' <<  i->direction()._sx << ' ' << i->direction()._sy << std::endl;
	}
	output << "NB INTERCEPTORS" << std::endl;
	output << this->nbInterceptors() << std::endl;
	output << "INTERCEPTORS" << std::endl;
	for (VInterceptors::const_iterator i = _interceptors.begin(); i != _interceptors.end(); ++i)
	{
		output << i->depot()->id() << ' ' << i->speed() << ' ';
		if (std::isfinite(i->range())) {
			output << i->range();
		} else {
			output << "1e+308";
		}
		output << std::endl;
	}
}

std::ostream & operator<< (std::ostream & o, const Problem & p) {
	o << "INTERCEPTORS(" << p.nbInterceptors() << "):" << std::endl;
	for (VInterceptors::const_iterator i = p.interceptors().begin(); i != p.interceptors().end(); i++)
	{
	  o << *i << std::endl;
	}
	o << "DEPOTS(" << p.nbDepots() << "):" << std::endl;
	for (VDepots::const_iterator i = p.depots().begin(); i != p.depots().end(); i++)
	{
	  o << *i << std::endl;
	}
	o << "MOBILES(" << p.nbMobiles() << "):" << std::endl;
	for (VMobiles::const_iterator i = p.mobiles().begin(); i != p.mobiles().end(); i++)
	{
	  o << *i << std::endl;
	}
	return o;
}
