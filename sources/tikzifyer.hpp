#ifndef __TIKZIFYER_HPP__
#define __TIKZIFYER_HPP__
#include "solution.hpp"

typedef std::vector<const Solution *> VPSolutions;

class Tikzifyer
{
public:
	Tikzifyer();
	~Tikzifyer();

	void operator() (const Solution &);
	void operator() (const Problem &);

	const Problem & problem() const;
	const VPSolutions & solutions() const;

	const VMobiles & mobiles() const;
	const VInterceptors & interceptors() const;

	Distance xmin() const;
	Distance xmax() const;
	Distance ymin() const;
	Distance ymax() const;

	static const std::string & style(unsigned);
	static void addStyle(const std::string &);

	void clear();

private:
	// Bounds
	Location _top_left;
	Location _bottom_right;
	void addToBounds(const Location &);

	// Data
	const Problem * _problem;
	VPSolutions _solutions;

	// Line styles
	static std::vector<std::string> _styles;
};

std::ostream & operator<< (std::ostream &, const Tikzifyer &);

#endif
