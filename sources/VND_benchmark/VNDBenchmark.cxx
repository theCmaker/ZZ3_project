#include <iostream>
#include <heuristic_fastest.hpp>
#include <random>
#include <algorithm>
#include <chrono>
#include <array>
#include <Pareto/paretofrontsolver.h>

std::mt19937 g(/*time(0)*/0);

int randomAccess (int i) { return g()%i; }

template< typename tPair >
struct second_t {
	typename tPair::second_type operator()( const tPair& p ) const { return     p.second; }
};

template< typename tMap >
second_t< typename tMap::value_type > second( const tMap& m ) { return second_t<     typename tMap::value_type >(); }

// Static members
template<unsigned INSTANCES, unsigned VND_VARIANTS>
std::vector<unsigned> VNDBenchmark<INSTANCES, VND_VARIANTS>::movements = {0, 1, 2, 3, 4, 5, 6, 7};

template<unsigned INSTANCES, unsigned VND_VARIANTS>
std::vector<unsigned> VNDBenchmark<INSTANCES, VND_VARIANTS>::policies = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

template<unsigned INSTANCES, unsigned VND_VARIANTS>
std::vector< std::function<Move*(void)> > VNDBenchmark<INSTANCES, VND_VARIANTS>::movementsBuilders = {
	// BestAvailable Policy
	[]() -> Move * { return new MoveInsert<BestAvailablePolicy>; },
	[]() -> Move * { return new MoveExtract<BestAvailablePolicy>; },
	[]() -> Move * { return new MoveMove1Route<BestAvailablePolicy>; },
	[]() -> Move * { return new MoveMove2Routes<BestAvailablePolicy>; },
	[]() -> Move * { return new MoveSwap1Route<BestAvailablePolicy>; },
	[]() -> Move * { return new MoveSwap2Routes<BestAvailablePolicy>; },
	[]() -> Move * { return new MoveReplace<BestAvailablePolicy>; },
	[]() -> Move * { return new Move2Opt<BestAvailablePolicy>; },
	// FirstAvailablePolicy
	[]() -> Move * { return new MoveInsert<FirstAvailablePolicy>; },
	[]() -> Move * { return new MoveExtract<FirstAvailablePolicy>; },
	[]() -> Move * { return new MoveMove1Route<FirstAvailablePolicy>; },
	[]() -> Move * { return new MoveMove2Routes<FirstAvailablePolicy>; },
	[]() -> Move * { return new MoveSwap1Route<FirstAvailablePolicy>; },
	[]() -> Move * { return new MoveSwap2Routes<FirstAvailablePolicy>; },
	[]() -> Move * { return new MoveReplace<FirstAvailablePolicy>; },
	[]() -> Move * { return new Move2Opt<FirstAvailablePolicy>; }
};


// Template definitions
template<unsigned INSTANCES, unsigned VND_VARIANTS>
VNDBenchmark<INSTANCES, VND_VARIANTS>::VNDBenchmark() :
	results(INSTANCES)
{
	std::mt19937 rand(0);
	instances.reserve(INSTANCES);
	basic_solutions.reserve(INSTANCES);
	variants_ids.reserve(VND_VARIANTS);
	variants.reserve(VND_VARIANTS);
	vnds.reserve(VND_VARIANTS);

	// Random number generators for instances
	std::uniform_real_distribution<double> amount_interceptors(1,8);
	std::uniform_real_distribution<double> amount_mobiles(1,50);
	std::uniform_real_distribution<double> amount_repos(1,5);

	for (unsigned i = 0; i < INSTANCES; ++i) {
		instances.push_back(new Problem(unsigned(amount_interceptors(rand)), unsigned(amount_mobiles(rand)), unsigned(amount_repos(rand))));
		Heuristic_fastest<SimpleCachePolicy> heuristic(*instances.back());
		heuristic.run();
		basic_solutions.push_back(heuristic.solution());
	}
	std::vector<unsigned> policies_ids = policies;
	for (unsigned i = 0; i < VND_VARIANTS; ++i) {
		variants.emplace_back();
		variants_ids.push_back(movements);
		// Shake the movements
		std::random_shuffle(variants_ids.back().begin(),variants_ids.back().end(), randomAccess);
		// Shake the policies
		std::random_shuffle(policies_ids.begin(),policies_ids.end(), randomAccess);

		// Build the variant
		for (auto & itr : variants_ids.back()) {
			// Add the movement
			variants.back().push_back(movementsBuilders[itr + movements.size() * policies_ids[itr]]());
			// Adjust the policies offset
			itr += movements.size() * policies_ids[itr];
		}

		// Prepare the VND
		vnds.push_back(new VND<30>(variants[i]));
	}
}

template<unsigned INSTANCES, unsigned VND_VARIANTS>
VNDBenchmark<INSTANCES, VND_VARIANTS>::~VNDBenchmark()
{
	for (unsigned i = 0; i < VND_VARIANTS; ++i) {
		for (std::vector<Move *>::iterator itr = variants[i].begin(); itr != variants[i].end(); ++itr) {
			delete *itr;
		}
		delete vnds[i];
	}

	for (unsigned i = 0; i < INSTANCES; ++i) {
		delete instances[i];
	}
}

template<unsigned INSTANCES, unsigned VND_VARIANTS>
std::vector< typename VNDBenchmark<INSTANCES, VND_VARIANTS>::vnd_variant_t > VNDBenchmark<INSTANCES, VND_VARIANTS>::operator ()() {

	typedef DataPoint<double, double, unsigned> DataPoint;
	typedef std::multimap< BenchmarkResult,unsigned > ResultSet;

	//For each instance
	for (unsigned instance_id = 0; instance_id < INSTANCES; ++instance_id) {

		// Initialise the policies
		AvailablePolicy::maxAcceptableTime() = 1.1 * basic_solutions[instance_id].worstInterceptionTime();
		AvailablePolicy::minAcceptableCount() =
				std::max(0u, unsigned(instances[instance_id]->nbMobiles() - 1.1 * (instances[instance_id]->nbMobiles()
								  - basic_solutions[instance_id].totalInterceptionCount())));
		//For each variant
		for (unsigned variant_id = 0; variant_id < VND_VARIANTS; ++variant_id) {
			//Run the VND
			Solution & solution = basic_solutions[instance_id];

			// Start the chrono...
			auto start = std::chrono::steady_clock::now();
			vnds[variant_id]->run(solution);
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double,std::milli> diff = end-start;

			// Store the result
			results[instance_id].insert(std::pair<BenchmarkResult,unsigned> (
											BenchmarkResult(std::round(solution.worstInterceptionTime()*1.0e+6)/1.0e+6,
															solution.problem().nbMobiles() - solution.totalInterceptionCount(),
															diff.count()),
											variant_id));
		}
	}

	std::vector<ParetoFront<DataPoint> > frontsVector;
	std::vector<DataPoint> data;
	// For each map of instance results
	for (std::vector<ResultSet>::iterator result = results.begin(); result != results.end(); ++result) {
		ParetoFrontSolver<DataPoint> solver;

		// 1- Gather the data
		data.clear();
		auto itr = result->begin();
		while (itr != result->end()) {
			data.push_back(DataPoint(double(itr->first._mobiles),
									 itr->first._time,
									 itr->second));
			++itr;
		}
		// 2- Solve
		solver.setPts(data);
		solver.compute_inclusive_frontiers();
		frontsVector.push_back(solver.getPFrontiers()[0]);

	}

	std::vector<VNDBenchmark::vnd_variant_t> best_variants;
	std::array<unsigned, VND_VARIANTS> scores;
	scores.fill(0u);

	for (std::vector<ParetoFront<DataPoint> >::iterator front = frontsVector.begin(); front != frontsVector.end(); ++front) {
		for (auto sol = front->begin(); sol != front->end(); ++sol) {
			++scores[sol->getInfo()];
			best_variants.push_back(variants_ids[sol->getInfo()]);
		}
	}

	for (auto val : scores) {
		std::cerr << val << " ";
	}
	std::cerr << std::endl;

	std::cout << "End" << std::endl;
	//TODO: compute the stats

	return best_variants;
}

/*void run_random_benchmark(unsigned instance_count = 10, unsigned vnd_variants_count = 1000) {

	unsigned test_id = 0, number_of_tests = 1000, file_id;

	std::vector<const char*> files = {"../examples/test_40m_5i_4d",
									 "../examples/test_60m_8i_4d",
									 "../examples/test_100m_8i_5d"};
	std::vector<Problem *> problems;
	std::vector<Solution *> solutions;

	std::multimap<const char*,BenchmarkResult> results;

	for (std::vector<const char*>::iterator file = files.begin(); file != files.end(); ++file) {
		problems.emplace_back(new Problem(*file));
		Heuristic_fastest<SimpleCachePolicy> fastest(*problems.back());
		fastest.run();
		std::cout << *file << ":\tTime: " << fastest.solution().worstInterceptionTime() << ";\tMobiles: " << fastest.solution().totalInterceptionCount() << std::endl;
		solutions.emplace_back(new Solution(fastest.solution()));
	}



	// VND sequence to be built
	std::vector<Move *> sequence;
	std::vector<unsigned> sequence_ids;


	// Number of tests
	while (test_id != number_of_tests) {
		// Shake the movements
		std::random_shuffle(movements.begin(),movements.end(), randomAccess);
		// Shake the policies
		std::random_shuffle(policies.begin(),policies.end(), randomAccess);


		sequence.clear();
		sequence_ids.clear();
		// Build the sequence
		for (std::vector<unsigned>::iterator itr = movements.begin(); itr != movements.end(); ++itr) {
			sequence.push_back(movementsBuilders[*itr + 8 * policies[*itr]]());
			sequence_ids.push_back(*itr + 8 * policies[*itr]);
		}

		//For each solution (i.e. each problem)
		file_id = 0;
		for (std::vector<Solution *>::iterator sol = solutions.begin(); sol != solutions.end(); ++sol) {
			// Get a copy of the solution
			Solution solution = **sol;

			AvailablePolicy::maxAcceptableTime() = 1.1 * solution.worstInterceptionTime();
			AvailablePolicy::minAcceptableCount() = solution.problem().nbMobiles() - 1.1 * (solution.problem().nbMobiles() - solution.totalInterceptionCount());

			//Run the VND
			VND<> vnd(sequence);
			auto start = std::chrono::steady_clock::now();
			vnd.run(solution);
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double,std::milli> diff = end-start;
			//Store the result
			if (solution.bestInterceptionCount() > 0) {
				results.insert(std::pair<const char*,BenchmarkResult>(files[file_id],BenchmarkResult(solution.worstInterceptionTime(),solution.totalInterceptionCount(),sequence_ids, diff.count())));
			}

			++file_id;
		}
		for (std::vector<Move *>::iterator itr = sequence.begin(); itr != sequence.end(); ++itr) {
			delete *itr;
		}

		++test_id;
	}

	std::vector<std::pair<const char*,BenchmarkResult> > fileResults;
	for (std::vector<const char *>::iterator file = files.begin(); file != files.end(); ++file) {
		fileResults.clear();
		auto itr = results.find(*file);
		auto count = results.count(*file);
		std::copy_n(itr,count,std::back_inserter(fileResults));
		std::sort(fileResults.begin(),fileResults.end(),[](std::pair<const char*,BenchmarkResult> a, std::pair<const char*,BenchmarkResult> b) -> bool { return a.second < b.second; });
		std::cout << itr->first << std::endl;
		unsigned i = 0;
		for (std::vector<std::pair<const char*,BenchmarkResult> >::iterator itr = fileResults.begin(); itr != fileResults.end(), i < 100; ++itr, ++i) {
			std::cout << itr->second << std::endl;
		}
		std::cout << std::endl;
	}

	for (unsigned i = 0; i < problems.size(); ++i) {
		delete problems[i];
		delete solutions[i];
	}
}*/
