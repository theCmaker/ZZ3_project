#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include <core/problem.hpp>
#include <export/tikzifyer.hpp>


int main(int argc, const char *argv[]){
	if (argc == 5) {
		Problem p((unsigned) atoi(argv[3]), (unsigned) atoi(argv[2]), (unsigned) atoi(argv[4]));
		p.write(argv[1]);

		Problem p2(argv[1]);
		Tikzifyer::addStyle("thick,color=OrangeRed,dotted,line width=1mm");
		Tikzifyer::addStyle("thick,color=Green,dashed");
		Tikzifyer tikzify;
		std::ofstream tikz_output_test_file("../examples/tex/tikz-content.tex");

		tikzify(p2);

		tikz_output_test_file << tikzify;
	}
}
