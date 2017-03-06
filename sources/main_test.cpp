#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include "heuristic_fastest.hpp"
#include "heuristic_sequence.hpp"
#include "tikzifyer.hpp"
#include "tikzgrapher.hpp"
#include "move.hpp"
#include "vnd.hpp"
#include "ms_els.hpp"

class MY_VND : public VND<30> {
	static const std::vector<Move *> movements;
public:
	MY_VND() : VND<30>(movements) {}
	~MY_VND() {
		for (auto movement : movements) {
			delete movement;
		}
	}
};
const std::vector<Move *> MY_VND::movements = {
		new MoveInsert<BestAvailablePolicy>,
		new MoveSwap1Route<FirstAvailablePolicy>,
		new MoveExtract<BestAvailablePolicy>,
		new MoveMove2Routes<BestAvailablePolicy>,
		new Move2Opt<FirstAvailablePolicy>,
		new MoveReplace<BestAvailablePolicy>,
		new MoveMove1Route<FirstAvailablePolicy>,
		new MoveSwap2Routes<FirstAvailablePolicy>};

int main(int argc, const char *argv[]){
	if (argc == 2) {

		Problem p(argv[1]);
		struct cmp {
			bool operator() (const Solution & s1, const Solution & s2) {
				Time record = s2.worstInterceptionTime();
				unsigned count_record = s2.bestInterceptionCount();

				return BestAvailablePolicy::update(s1.worstInterceptionTime(), record, s1.bestInterceptionCount(), count_record);
			}
		};

		MS_ELS<5,5,10, MY_VND, Problem, Solution, cmp> multi_start;
		auto start = std::chrono::steady_clock::now();
		Solution s = multi_start(p);
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double,std::micro> diff = end-start;

		std::cout << argv[1] << ": ";
		std::cout << s.worstInterceptionTime() << ' ' << p.nbMobiles() - s.totalInterceptionCount() << std::endl;
		std::cout << "Computing time: " << diff.count() << "µs" << std::endl;

	} else if (argc > 3) {
		Problem p(argv[1]);
		Heuristic_fastest<SimpleCachePolicy> fastest(p);
		Heuristic_sequence sequence(p);
		if (strcmp(argv[2],"SPATIAL") == 0) {
			Tikzifyer::addStyle("thick,color=OrangeRed,dotted,line width=1mm");
			Tikzifyer::addStyle("thick,color=Green,dashed");
			Tikzifyer tikzify;
			std::ofstream tikz_output_test_file("../tests/tikz-content.tex");
			tikzify(p);
			if (strcmp(argv[3],"FASTEST") == 0) {
				fastest.run();
				tikzify(fastest.solution());
			} else if (strcmp(argv[3],"SEQUENCE") == 0) {
				sequence.run();
				tikzify(sequence.solution());
			}
			tikz_output_test_file << tikzify;
		} else if (strcmp(argv[2],"TEMPORAL") == 0) {
			std::ofstream tikz_output_graph_file("../tests/tikz-graph.tex");
			TikzGrapher::addStyle("thick,color=Green");
			TikzGrapher::addStyle("thick,color=OrangeRed");
			TikzGrapher::addStyle("thick,color=Cyan");
			TikzGrapher::addStyle("thick,color=Plum");
			TikzGrapher tikzgraph;
			tikzgraph(p);
			if (strcmp(argv[3],"FASTEST") == 0) {
				fastest.run();
				tikzgraph(fastest.solution());
			} else if (strcmp(argv[3],"SEQUENCE") == 0) {
				sequence.run();
				tikzgraph(sequence.solution());
			}
			tikz_output_graph_file << tikzgraph;
		} else if (strcmp(argv[2],"CHRONO") == 0) {

			std::cout << argv[1] << std::endl;

			auto start = std::chrono::steady_clock::now();
			for (unsigned i = 0; i < 100; ++i) {
				Heuristic_fastest<SimpleCachePolicy> fastest_cache(p);
				fastest_cache.run();
			}
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double,std::micro> diff = end-start;
			std::cout << "Computing time (with cache): " << diff.count()/100 << "µs" << std::endl;

			start = std::chrono::steady_clock::now();
			for (unsigned i = 0; i < 100; ++i) {
				Heuristic_fastest<NoCachePolicy> fastest_nocache(p);
				fastest_nocache.run();
			}
			end = std::chrono::steady_clock::now();
			diff = end-start;
			std::cout << "Computing time (without cache): " << diff.count()/100 << "µs" << std::endl;
		} else if (strcmp(argv[2],"VND") == 0) {
			std::cout << argv[1] << std::endl;
			sequence.run();
			Solution s = sequence.solution();
			VND<> vnd;
			VND<30>::before(s);
			auto start = std::chrono::steady_clock::now();
			vnd(s);
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double,std::micro> diff = end-start;
			std::cout << s.worstInterceptionTime() << ' ' << p.nbMobiles() - s.totalInterceptionCount() << std::endl;
			std::cout << "Computing time: " << diff.count() << "µs" << std::endl;
		}
	}
	return 0;
}
