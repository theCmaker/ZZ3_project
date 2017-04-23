#ifndef __BENCHMARKRESULT_HPP__
#define __BENCHMARKRESULT_HPP__

#include <iostream>
#include <units.hpp>

struct BenchmarkResult {
	Time _time;
	unsigned _mobiles;
	double _computation_time;

	BenchmarkResult(Time time, unsigned mobiles, double computation);
	~BenchmarkResult();

	bool operator< (const BenchmarkResult & other) const;
};

std::ostream & operator << (std::ostream & o, const BenchmarkResult & br);

#endif // __BENCHMARKRESULT_HPP__
