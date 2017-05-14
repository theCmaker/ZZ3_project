/**
 * @file VNDBenchmark.hpp
 * @brief Benchmark of the VND. Try to find the best combinations of movements.
 */

#ifndef __VNDBENCHMARK_HPP__
#define __VNDBENCHMARK_HPP__

#include <metaheuristics/vnd.hpp>
#include <functional>
#include <map>
#include "benchmarkresult.hpp"

/**
 * @brief VND variant representation - with identifiers.
 */
typedef std::vector< unsigned > vnd_variant_t;

/**
 * @brief Structure to associate a variant to a score (occurrences in best front)
 */
struct VariantWithScore {
	/**
	 * @brief Constructor
	 * @param variant VND variant
	 * @param score occurences in best front
	 */
	VariantWithScore(vnd_variant_t variant, unsigned score);

	/**
	 * @brief Destructor
	 */
	~VariantWithScore();

	vnd_variant_t	variant;	///< VND variant
	unsigned		score;		///< Occurences in best front
};

/**
 * @brief Append the string representation of a VariantWithScore to an output stream
 * @param o output stream
 * @param v the variant/score couple
 * @return the output stream
 */
std::ostream & operator<< (std::ostream & o, const VariantWithScore & v);

/**
 * @brief Perform a benchmark on the VND
 * @tparam INSTANCES number of generated instances
 * @tparam VND_VARIANTS number of generated VND variants
 */
template<unsigned INSTANCES = 10, unsigned VND_VARIANTS = 1000>
class VNDBenchmark
{
protected:
	std::vector< Problem * > instances;									///< Generated instances
	std::vector< Solution > basic_solutions;							///< Simple solutions (fastest heuristic)
	std::vector< std::multimap< BenchmarkResult,unsigned > > results;	///< VND results by instance by variant
	std::vector< vnd_variant_t > variants_ids;							///< VND variants with ids
	std::vector< std::vector< Move * > > variants;						///< VND variants (real movements)
	std::vector< VND<30> * > vnds;										///< VND objects

private:
	static vnd_variant_t movements;										///< Movement ids
	static std::vector<unsigned> policies;								///< Policies ids
	static std::vector<std::function<Move*(void)>> movementsBuilders;	///< Available movement constructors

public:
	/**
	 * @brief Constructor
	 */
	VNDBenchmark();

	/**
	 * @brief Destructor
	 */
	~VNDBenchmark();

	/**
	 * @brief Run the benchmark
	 * @return the selection of best variants (~ 5%) with their scores
	 */
	std::vector<VariantWithScore> operator ()();
};

#include "VNDBenchmark.cxx"

#endif // __VNDBENCHMARK_HPP__
