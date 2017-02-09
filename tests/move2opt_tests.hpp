#ifndef MOVE2OPT_TESTS_H
#define MOVE2OPT_TESTS_H

//-------------- Tests de la class Move2Opt dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---Move2Opt---Scan---------------True---------------------------------------1
TEST (Move2OptTest, Scan_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	std::cout << h << std::endl;

	Move * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;

	EXPECT_TRUE(resultScan);

	delete move;
}

#endif // MOVE2OPT_TESTS_H
