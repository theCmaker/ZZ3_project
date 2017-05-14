/**
 * @file benchmarkresult.hpp
 * @brief Benchmark Result container
 */

#ifndef __BENCHMARKRESULT_HPP__
#define __BENCHMARKRESULT_HPP__

#include <iostream>
#include <core/units.hpp>

/**
 * @brief Benchmark Result container. Keep time, uncaught mobiles and computation time.
 */
struct BenchmarkResult {
	Time _time;					///< Time for the longest route.
	unsigned _mobiles;			///< Uncaught mobiles
	double _computation_time;	///< Time needed during processing

	/**
	 * @brief Constructor
	 * @param time Time for the longest route
	 * @param mobiles Uncaught mobiles
	 * @param computation Time needed during processing
	 */
	BenchmarkResult(Time time, unsigned mobiles, double computation);

	/**
	 * @brief Destructor
	 */
	~BenchmarkResult();

	/**
	 * @brief Comparator
	 * @param other the other result
	 * @return true if this BenchmarkResult is better than the other one
	 */
	bool operator< (const BenchmarkResult & other) const;
};

/**
 * @brief Append benchmark result representation to output stream
 * @param o output stream
 * @param br benchmark result
 * @return output stream
 */
std::ostream & operator << (std::ostream & o, const BenchmarkResult & br);

#endif // __BENCHMARKRESULT_HPP__
