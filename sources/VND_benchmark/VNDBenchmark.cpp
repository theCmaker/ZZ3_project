#include <iostream>
#include <vnd.hpp>
#include <heuristic_fastest.hpp>
#include <random>
#include <algorithm>
#include <functional>
#include <map>

struct BenchmarkResult {
	Time _time;
	unsigned _mobiles;
	std::vector<unsigned> _sequence;

	BenchmarkResult(Time time, unsigned mobiles, std::vector<unsigned> sequence) :
		_time(time),
		_mobiles(mobiles),
		_sequence(sequence)
	{}
	~BenchmarkResult() {}

	bool operator< (const BenchmarkResult & other) const {
		return _time < other._time;
	}

	bool operator> (const BenchmarkResult & other) const {
		return _mobiles > other._mobiles;
	}
};

std::ostream & operator << (std::ostream & o, const BenchmarkResult & br) {
	o << "Time: " << br._time << ";\tMobiles: " << br._mobiles << ";\tSequence:";
	for (auto i : br._sequence) {
		o << ' ' << i ;
	}
	return o;
}

int main(int argc, const char * argv[]) {

	unsigned test_id = 0, number_of_tests = 100, file_id;

	std::vector<const char*> files = {"../examples/test_100m_8i_5d"};
	std::vector<Problem *> problems;
	std::vector<Solution *> solutions;

	std::multimap<const char*,BenchmarkResult> results;

	for (std::vector<const char*>::iterator file = files.begin(); file != files.end(); ++file) {
		problems.emplace_back(new Problem(*file));
		Heuristic_fastest<SimpleCachePolicy> fastest(*problems.back());
		fastest.run();
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
		std::random_shuffle(movements.begin(),movements.end());
		// Shake the policies
		std::random_shuffle(policies.begin(),policies.end());

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
			VND vnd(sequence);
			vnd.run(solution);

			//Store the result
			if (solution.bestInterceptionCount() > 0) {
				results.insert(std::pair<const char*,BenchmarkResult>(files[file_id],BenchmarkResult(solution.worstInterceptionTime(),solution.totalInterceptionCount(),sequence_ids)));
			}

			++file_id;
		}

		for (std::vector<Move *>::iterator itr = sequence.begin(); itr != sequence.end(); ++itr) {
			delete *itr;
		}

		++test_id;
		std::cerr << test_id << std::endl;
	}

	for (std::multimap<const char*, BenchmarkResult>::iterator itr = results.begin(); itr != results.end(); ++itr) {
		std::cout << itr->first << ":\t" << itr->second << std::endl;
	}

	for (unsigned i = 0; i < problems.size(); ++i) {
		delete problems[i];
		delete solutions[i];
	}

	return 0;
}
