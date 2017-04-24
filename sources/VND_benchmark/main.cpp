#include <iostream>
#include <VND_benchmark/VNDBenchmark.hpp>
int main(int argc, const char * argv[]) {
	//Run the benchmark for 50 instances and 200 variants
	VNDBenchmark<50u,2000u> benchmark;
	for (auto sol : benchmark()) {
		for (auto id : sol)
			std::cout << id << " ";
		std::cout << std::endl;
	}

	return 0;
}

