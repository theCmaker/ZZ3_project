#include "benchmarkresult.hpp"

BenchmarkResult::BenchmarkResult(Time time, unsigned mobiles, double computation) :
	_time(time),
	_mobiles(mobiles),
	_computation_time(computation)
{}

BenchmarkResult::~BenchmarkResult() {}

bool BenchmarkResult::operator<(const BenchmarkResult &other) const {
	return (_time < other._time) || (_time == other._time && _mobiles > other._mobiles) || (_time == other._time && _mobiles == other._mobiles && _computation_time < other._computation_time);
}

std::ostream & operator << (std::ostream & o, const BenchmarkResult & br) {
	o << "Time: " << br._time
	  << ";\tMobiles: " << br._mobiles
	  << ";\t Computation: " << br._computation_time << " ms";
	return o;
}
