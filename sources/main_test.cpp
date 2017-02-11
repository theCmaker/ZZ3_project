#include <iostream>
#include <fstream>
#include <chrono>
#include "heuristic_fastest.hpp"
#include "heuristic_sequence.hpp"
#include "tikzifyer.hpp"
#include "tikzgrapher.hpp"
#include "move.hpp"
#include "vnd.hpp"

int main(int argc, const char *argv[]){
	if (argc > 1) {
		Tikzifyer::addStyle("thick,color=OrangeRed,dotted,line width=1mm");
		Tikzifyer::addStyle("thick,color=Green,dashed");
		TikzGrapher::addStyle("thick,color=Green");
		TikzGrapher::addStyle("thick,color=OrangeRed");
		TikzGrapher::addStyle("thick,color=Cyan");
		TikzGrapher::addStyle("thick,color=Plum");
		Tikzifyer tikzify;
		TikzGrapher tikzgraph;
		std::ofstream tikz_output_test_file("../tests/tikz-content.tex");
		std::ofstream tikz_output_graph_file("../tests/tikz-graph.tex");

		Problem p(argv[1]);
//		Problem p(40,1000,7);
		std::cout << p << std::endl;

		tikzgraph(p);
		tikzify(p);
//		Heuristic_fastest<> h0a(p);
		Heuristic_fastest<SimpleCachePolicy> h0b(p);

//		auto start = std::chrono::steady_clock::now();
//		h0a.run();
//		auto end = std::chrono::steady_clock::now();
//		std::chrono::duration<double,std::micro> diff = end-start;
//		std::cout << "H0 >>> Computing time (without cache): " << diff.count() << "µs" << std::endl;

//		start = std::chrono::steady_clock::now();
		h0b.run();
//		end = std::chrono::steady_clock::now();
//		diff = end-start;
//		std::cout << "H0 >>> Computing time (with cache): " << diff.count() << "µs" << std::endl;


		Heuristic_sequence h1a(p);
		h1a.run();

		Heuristic_sequence h1b(p);
		std::vector<unsigned> expected_sequence({3,0,1});
		h1b.run();
		//std::cout << h1b.solution() << std::endl;

		Solution s = h0b.solution();
		/*MoveInsert<> movement;
		if (movement.scan(s)) {
			movement.commit(s);
		} else {
			std::cerr << "Impossible to find an amelioration" << std::endl;
		}
		std::cout << s << std::endl;
		MoveInsert<> movement2;
		if (movement2.scan(s)) {
			movement2.commit(s);
		} else {
			std::cerr << "Impossible to find an amelioration" << std::endl;
		}

		std::cout << s << std::endl;
		MoveExtract<> movement3;
		if (movement3.scan(s)) {
			movement3.commit(s);
		}
		*/

		std::cout << s << std::endl;
		VND vnd;
		vnd.run(s);
		std::cout << s << std::endl;

//		tikzify(h0b.solution());
//		tikzify(h1a.solution());
		tikzify(s);
		//tikzify(h1b.solution());



//		tikzgraph(h0b.solution());
		tikzgraph(s);
		//tikzgraph(h1b.solution());
//		std::cout << h0a << std::endl;
//		std::cout << h0b << std::endl;

		tikz_output_test_file << tikzify;

		tikz_output_graph_file << tikzgraph;
	}
	return 0;
}
