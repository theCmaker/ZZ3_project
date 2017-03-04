#include "decoder.h"
#include <algorithm>
#include <heuristic_sequence_multisol.hpp>

// sort the elements according to their keys
std::vector<size_t> makeSequence (std::vector<double> const & w)
{
	// create the Id vector
	std::vector<size_t> indices(w.size());
	std::iota(begin(indices), end(indices), static_cast<size_t>(0));

	// sort it according to the weights, uses a C++11 lambda function for the comparison
	std::sort(begin(indices), end(indices), [&](size_t i, size_t j) { return (w[i] < w[j]); });
	return indices;
}


double Decoder::decode (std::vector<double> & chromosome)
{
	if (chromosome.size() > 0) {
		std::vector<unsigned> sequence(chromosome.size());
		std::iota(begin(sequence), end(sequence), 0u);
		std::sort(begin(sequence), end(sequence), [&](size_t i, size_t j) { return (chromosome[i] < chromosome[j]); });

		solver.clear();

		Heuristic_Sequence_MultiSol heuristic(data);

		heuristic.run(sequence);

		unsigned n = data.nbMobiles();
		for (auto sol : heuristic.solutions()) {
			solver.add((double)(n - sol.totalInterceptionCount()), sol.worstInterceptionTime(),sol);
		}

		solver.computeHypervolume();
	} else {
		std::cerr << "Whattafuck" << std::endl;
	}
	return solver.hypervolume;
}
