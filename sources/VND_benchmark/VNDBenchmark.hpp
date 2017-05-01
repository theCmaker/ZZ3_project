#ifndef __VNDBENCHMARK_HPP__
#define __VNDBENCHMARK_HPP__

#include <vnd.hpp>
#include <functional>
#include <map>
#include "benchmarkresult.hpp"

typedef std::vector< unsigned > vnd_variant_t;
struct VariantWithScore {
	VariantWithScore(vnd_variant_t, unsigned);
	~VariantWithScore();

	vnd_variant_t	variant;
	unsigned		score;
};

std::ostream & operator<< (std::ostream &, const VariantWithScore &);

template<unsigned INSTANCES = 10, unsigned VND_VARIANTS = 1000>
class VNDBenchmark
{
protected:
	std::vector< Problem * > instances;
	std::vector< Solution > basic_solutions;
	std::vector< std::multimap< BenchmarkResult,unsigned > > results;
	std::vector< vnd_variant_t > variants_ids;
	std::vector< std::vector< Move * > > variants;
	std::vector< VND<30> * > vnds;

private:
	// Movements ids
	static vnd_variant_t movements;
	// Policies (2, 1 for each movement)
	static std::vector<unsigned> policies;
	// List of available movements
	static std::vector<std::function<Move*(void)>> movementsBuilders;

public:
	// Constructor
	VNDBenchmark();
	// Destructor
	~VNDBenchmark();
	// Random
	std::vector<VariantWithScore> operator ()();
	// Specific variant, specific basic solution (and Problem!)
	//std::vector< vnd_variant_t > operator ()(const Solution & basic_solution, const vnd_variant_t & vnd_variant);
};

#include "VNDBenchmark.cxx"

#endif // __VNDBENCHMARK_HPP__
