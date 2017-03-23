#include <iostream>
#include <vnd.hpp>
#include <heuristic_fastest.hpp>
#include <random>
#include <algorithm>
#include <functional>
#include <map>
#include <chrono>

std::mt19937 g(time(0));

int randomAccess (int i) { return g()%i; }

template< typename tPair >
struct second_t {
	typename tPair::second_type operator()( const tPair& p ) const { return     p.second; }
};

template< typename tMap >
second_t< typename tMap::value_type > second( const tMap& m ) { return second_t<     typename tMap::value_type >(); }

struct BenchmarkResult;
std::ostream & operator << (std::ostream & o, const BenchmarkResult & br);

struct BenchmarkResult {
	Time _time;
	unsigned _mobiles;
	std::vector<unsigned> _sequence;
	double _computation_time;

	BenchmarkResult(Time time, unsigned mobiles, std::vector<unsigned> sequence, double computation) :
		_time(time),
		_mobiles(mobiles),
		_sequence(sequence),
		_computation_time(computation)
	{}
	~BenchmarkResult() {}

	bool operator< (const BenchmarkResult & other) const {
		return (_time < other._time) || (_time == other._time && _mobiles > other._mobiles) || (_time == other._time && _mobiles == other._mobiles && _computation_time < other._computation_time);
	}
};

std::ostream & operator << (std::ostream & o, const BenchmarkResult & br) {
	o << "Time: " << br._time << ";\tMobiles: " << br._mobiles << ";\tSequence:";
	for (auto i : br._sequence) {
		o << ' ' << i ;
	}
	o << ";\t Computation: " << br._computation_time << " ms";
	return o;
}

void run_defined_benchmark();
void run_random_benchmark();

int main(int argc, const char * argv[]) {

	if (argc == 1) {
		run_random_benchmark();
	} else {
		run_defined_benchmark();
	}
	return 0;
}

void run_defined_benchmark() {
	unsigned test_id = 0, file_id;

	std::vector<const char*> files = {"../examples/test_5m_2i","../examples/test_5m_1i","../examples/test_6m_2i","../examples/test_6m_1i"/*"../examples/test_100m_8i_5d",
									 "../examples/test_60m_8i_4d",
									 "../examples/test_40m_5i_4d"*/};
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

	std::vector<std::vector< unsigned > > sequences_ids;

	sequences_ids.push_back(std::vector<unsigned>({ 1, 3, 6, 13, 4, 15, 0, 2 }));
	sequences_ids.push_back(std::vector<unsigned>({ 13, 1, 14, 3, 12, 15, 2, 8 }));
	sequences_ids.push_back(std::vector<unsigned>({ 13, 3, 12, 1, 8, 15, 6, 2 }));
	sequences_ids.push_back(std::vector<unsigned>({ 13, 3, 14, 0, 1, 4, 15, 2 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 0, 1, 12, 14, 13, 15, 2 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 1, 0, 12, 7, 14, 2, 13 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 1, 12, 15, 5, 2, 0, 6 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 1, 13, 12, 14, 15, 2, 0 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 1, 5, 4, 15, 6, 2, 8 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 1, 6, 12, 5, 7, 2, 8 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 12, 13, 15, 2, 6, 0, 9 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 12, 13, 7, 14, 2, 9, 0 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 12, 15, 5, 1, 0, 2, 14 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 12, 7, 1, 13, 2, 14, 0 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 12, 7, 1, 2, 0, 5, 14 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 13, 12, 15, 1, 2, 14, 0 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 13, 14, 4, 7, 1, 2, 8 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 4, 1, 13, 8, 6, 15, 2 }));
	sequences_ids.push_back(std::vector<unsigned>({ 3, 4, 6, 15, 13, 0, 1, 2 }));
//	sequences_ids.push_back(std::vector<unsigned>({ 12, 1, 13, 3, 14, 15, 10, 8 }));

//	sequences_ids.push_back(std::vector<unsigned>({ 1, 0, 15, 3, 4, 2, 14, 5 }));
//	sequences_ids.push_back(std::vector<unsigned>({ 7, 13, 1, 4, 11, 10, 8, 14 }));
//	sequences_ids.push_back(std::vector<unsigned>({ 4, 15, 6, 11, 1, 13, 2, 0 }));
//	sequences_ids.push_back(std::vector<unsigned>({ 4, 5, 14, 3, 15, 1, 10, 8 }));
//	sequences_ids.push_back(std::vector<unsigned>({ 3, 1, 14, 4, 5, 10, 0, 15 }));
//	sequences_ids.push_back(std::vector<unsigned>({ 6, 3, 0, 12, 13, 15, 9, 10 }));

	std::vector<std::vector<Move *> > sequences(sequences_ids.size());

	// List of available movements
	std::vector<std::function<Move*(void)>> movementsBuilders {
		// BestAvailable Policy
		[]() -> Move * { return new MoveInsert<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveExtract<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove1Route<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove2Routes<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap1Route<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap2Routes<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveReplace<BestAvailablePolicy>(); },
		[]() -> Move * { return new Move2Opt<BestAvailablePolicy>(); },
		// FirstAvailablePolicy
		[]() -> Move * { return new MoveInsert<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveExtract<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove1Route<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove2Routes<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap1Route<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap2Routes<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveReplace<FirstAvailablePolicy>(); },
		[]() -> Move * { return new Move2Opt<FirstAvailablePolicy>(); }
	};

	// Build the sequence
	for (unsigned i = 0; i < sequences_ids.size(); ++i) {
		for (std::vector<unsigned>::iterator itr = sequences_ids[i].begin(); itr != sequences_ids[i].end(); ++itr) {
			sequences[i].emplace_back(movementsBuilders[*itr]());
		}
	}
	// VND sequence to be built
	std::vector<Move *> sequence;
	std::vector<unsigned> sequence_ids;

	// Number of tests
	while (test_id != sequences.size()) {

		sequence = sequences[test_id];
		sequence_ids = sequences_ids[test_id];

		//For each solution (i.e. each problem)
		file_id = 0;
		for (std::vector<Solution *>::iterator sol = solutions.begin(); sol != solutions.end(); ++sol) {
			// Get a copy of the solution
			Solution solution = **sol;

			AvailablePolicy::maxAcceptableTime() = 1.1 * solution.worstInterceptionTime();
			AvailablePolicy::minAcceptableCount() = solution.problem().nbMobiles() - 1.1 * (solution.problem().nbMobiles() - solution.totalInterceptionCount());

			//Run the VND
			VND vnd(sequence);
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


		++test_id;
		std::cerr << test_id << std::endl;
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
		for (std::vector<std::pair<const char*,BenchmarkResult> >::iterator itr = fileResults.begin(); itr != fileResults.end(), i < 5; ++itr, ++i) {
			std::cout << itr->second << std::endl;
		}
		std::cout << std::endl;
	}
	/*for (std::multimap<const char*, BenchmarkResult>::iterator itr = results.begin(); itr != results.end(); ++itr) {
		std::cout << itr->first << ":\t" << itr->second << std::endl;
	}*/

	for (unsigned i = 0; i < problems.size(); ++i) {
		delete problems[i];
		delete solutions[i];
	}
}

void run_random_benchmark() {
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

	// Movements ids
	std::vector<unsigned> movements = {0, 1, 2, 3, 4, 5, 6, 7};
	// Policies (2, 1 for each movement)
	std::vector<unsigned> policies = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

	// VND sequence to be built
	std::vector<Move *> sequence;
	std::vector<unsigned> sequence_ids;

	// List of available movements
	std::vector<std::function<Move*(void)>> movementsBuilders {
		// BestAvailable Policy
		[]() -> Move * { return new MoveInsert<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveExtract<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove1Route<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove2Routes<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap1Route<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap2Routes<BestAvailablePolicy>(); },
		[]() -> Move * { return new MoveReplace<BestAvailablePolicy>(); },
		[]() -> Move * { return new Move2Opt<BestAvailablePolicy>(); },
		// FirstAvailablePolicy
		[]() -> Move * { return new MoveInsert<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveExtract<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove1Route<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveMove2Routes<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap1Route<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveSwap2Routes<FirstAvailablePolicy>(); },
		[]() -> Move * { return new MoveReplace<FirstAvailablePolicy>(); },
		[]() -> Move * { return new Move2Opt<FirstAvailablePolicy>(); }
	};

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
		if ((test_id-1) % 100 == 0)
			std::cerr << test_id << std::endl;
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
	/*
	for (std::multimap<const char*, BenchmarkResult>::iterator itr = results.begin(); itr != results.end(); ++itr) {
		std::cout << itr->first << ":\t" << itr->second << std::endl;
	}*/

	for (unsigned i = 0; i < problems.size(); ++i) {
		delete problems[i];
		delete solutions[i];
	}
}
