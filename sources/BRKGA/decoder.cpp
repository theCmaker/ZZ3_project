#include "decoder.h"
#include <algorithm>
#include <heuristic_sequence_multisol.hpp>
#include <vnd.hpp>

unsigned Decoder::VNDcounter = 0;

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

		auto solutions = heuristic.solutions();

		decltype(solutions) enhancements;

		std::vector< DataPoint<double,double,Solution> > dataPts;
		for (auto sol : solutions) {
			dataPts.emplace_back((double)(n - sol.totalInterceptionCount()), sol.worstInterceptionTime(),sol);
		}

		ParetoFrontSolver<DataPoint<double,double,Solution> > s;
		s.setPts(dataPts);
		s.compute_frontiers();

		for (auto point : s.getPFrontiers()[0]) {
			solver.add((double)(n - point.getInfo().totalInterceptionCount()), point.getInfo().worstInterceptionTime(),point.getInfo());
			enhancements.emplace_back(point.getInfo());
			VND<20>::before(enhancements.back());
			VND<20> vnd;
			vnd(enhancements.back());
			++VNDcounter;
		}

		for (auto sol : enhancements) {
			solver.add((double)(n - sol.totalInterceptionCount()), sol.worstInterceptionTime(),sol);
		}

		for (auto sol : solutions) {
			solver.add((double)(n - sol.totalInterceptionCount()), sol.worstInterceptionTime(),sol);
		}

		solver.computeHypervolume();
	} else {
		std::cerr << "What dafuck" << std::endl;
	}
	return solver.hypervolume;
}
