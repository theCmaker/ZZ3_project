#include "problem.hpp"
#include <iostream>
#include <fstream>
#include <string>


Problem::Problem() {}

Problem::Problem(const char * filename)
{
	unsigned	nb_repos = 0,
				nb_mobiles = 0,
				nb_interceptors = 0;
	Distance	x = 0,
				y = 0;
	Speed		dx = 0,
				dy = 0.,
				s = 0.;
	std::string buf;
	std::ifstream ifs (filename,std::ifstream::in);
	if (!ifs.is_open()) {
		std::cout << "Could not read file." << std::endl;
	} else {
		if (ifs.eof()) {
			std::cout << "File is empty!" << std::endl;
		} else {
			while (std::getline(ifs,buf)) {
				if (buf == "NB REPOS") {
					ifs >> nb_repos;
				} else if (buf == "NB MOBILES") {
					ifs >> nb_mobiles;
				} else if (buf == "NB INTERCEPTORS") {
					ifs >> nb_interceptors;
				} else if (buf == "MOBILES") {
					unsigned i = 0;
					while (i < nb_mobiles && ifs >> x >> y >> dx >> dy && std::getline(ifs,buf)) {
						++i;
						_mobiles.push_back(Mobile(x,y,dx,dy));
					}
				} else if (buf == "INTERCEPTORS") {
					unsigned i = 0;
					while (i < nb_interceptors && ifs >> x >> y >> s && std::getline(ifs,buf)) {
						++i;
						_interceptors.push_back(Interceptor(x,y,s));
					}
				} else if (buf == "REPOS") {
					unsigned i = 0;
					while (i < nb_repos && ifs >> x >> y && std::getline(ifs,buf)) {
						++i;
						_depots.push_back(Depot(x,y));
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
