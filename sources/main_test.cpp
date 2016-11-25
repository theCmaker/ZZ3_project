#include <iostream>
#include <fstream>
#include <chrono>
#include "heuristic_fastest.hpp"
#include "heuristic_sequence.hpp"
#include "tikzifyer.hpp"

int main(int argc, const char *argv[]){
	if (argc > 1) {
		Tikzifyer::addStyle("thick,color=Green");
		Tikzifyer::addStyle("thick,color=OrangeRed");
		Tikzifyer tikzify;
		std::ofstream tikz_output_test_file("../tests/tikz-content.tex");

		Problem p(argv[1]);
		std::cout << p << std::endl;

		tikzify(p);
		Heuristic_fastest h0(p);
		auto start = std::chrono::steady_clock::now();
		h0.run();
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double,std::micro> diff = end-start;
		std::cout << "Computing time: " << diff.count() << "µs" << std::endl;

		Heuristic_sequence h1a(p);
		h1a.run();

		Heuristic_sequence h1b(p);
		std::vector<unsigned> expected_sequence({2,0,1});
		h1b.run(expected_sequence);

		tikzify(h0.solution());
		tikzify(h1a.solution());
		tikzify(h1b.solution());
		std::cout << h0 << std::endl;
		std::cout << h1a << std::endl;
		std::cout << h1b << std::endl;
		tikz_output_test_file << tikzify ;
	}
	return 0;
}
