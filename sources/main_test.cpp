#include <iostream>
#include <fstream>
#include "heuristic_fastest.hpp"
#include "tikzifyer.hpp"

int main(int argc, const char *argv[]){
	if (argc > 1) {
		Tikzifyer tikzify;
		std::ofstream tikz_output_test_file("../tests/tikz-content.tex");

		Problem p(argv[1]);
		std::cout << p << std::endl;

		tikzify(p);

		Heuristic_fastest h0(p);
		h0.run();

		tikzify(h0.solution());
		std::cout << h0 << std::endl;
		tikz_output_test_file << tikzify ;
	}
	return 0;
}
