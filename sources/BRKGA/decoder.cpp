#include "decoder.h"
#include <algorithm>
#include <heuristic_sequence_multisol.hpp>

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
		std::cerr << "What dafuck" << std::endl;
	}
	return solver.hypervolume;
}
