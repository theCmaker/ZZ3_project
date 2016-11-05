#include <iostream>
#include "heuristic_fastest.hpp"

int main(int argc, const char *argv[]){
	if (argc > 1) {
		Problem p(argv[1]);
		std::cout << p << std::endl;
		Heuristic_fastest h0(p);
		h0.run();
		std::cout << h0 << std::endl;
	}
	return 0;
}
