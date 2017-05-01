#include <iostream>
#include <VND_benchmark/VNDBenchmark.hpp>
int main(int argc, const char * argv[]) {
	//Run the benchmark for 50 instances and 200 variants
	std::cout << "Running for 100 instances, 1000 VNDs..." << std::endl;
	VNDBenchmark<100u,1000u> benchmark;
	for (auto sol : benchmark()) {;
		std::cout << sol << std::endl;
	}

	return 0;
}

