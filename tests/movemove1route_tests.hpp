#ifndef __MOVEMOVE1ROUTE_TESTS_HPP
#define __MOVEMOVE1ROUTE_TESTS_HPP

//---------- Tests de la classe MoveMove1Route dans move.hpp -----------------

//---ScanTests-----------------------------------------------------------------
//---MoveMove1Route---Scan---------------True--------------------------------1
TEST (MoveMove1Route, Scan_true)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9,10});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	std::cout << h << std::endl;

	Move * move = new MoveMove1Route<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	EXPECT_TRUE(resultScan);

	delete move;
}

//---MoveMove1Route---Commit--------------------------------------------------2
TEST (MoveMove1Route, Commit)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveMove1Route<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 3
	EXPECT_EQ(s[3]._first,5);
	EXPECT_EQ(s.mobile(5)._next,4);
	EXPECT_EQ(s.mobile(4)._next,6);
	EXPECT_EQ(s.mobile(6)._next,-1);

	delete move;
}

//---MoveMove1Route---Commit-Date---------------------------------------------3
TEST (MoveMove1Route, CommitDate)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9,10});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveMove1Route<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	Problem p3("../tests/data/test_40m_1i_3");
	Heuristic_sequence h3(p3);
	h3.run({5,4,6});
	Solution s3 = h3.solution();

	EXPECT_NEAR(s.lastInterceptionTime(3),s3.lastInterceptionTime(0),1e-6);

	delete move;
}

#endif
