#ifndef __MOVESWAP2ROUTES_TESTS_HPP__
#define __MOVESWAP2ROUTES_TESTS_HPP__

//---------- Tests de la classe MoveSwap2Routes dans move.hpp -----------------

//---ScanTests-----------------------------------------------------------------
//---MoveSwap2Routes---Scan---------------True--------------------------------1
TEST (MoveSwap2Routes, Scan_true)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveSwap2Routes<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	EXPECT_TRUE(resultScan);

	delete move;
}

//---MoveSwap2Routes---Commit-------------------------------------------------2
TEST (MoveSwap2Routes, Commit)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveSwap2Routes<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 0
	EXPECT_EQ(s[0]._first,1);
	EXPECT_EQ(s.mobile(1)._next,-1);

	// interceptor 1
	EXPECT_EQ(s[1]._first,3);
	EXPECT_EQ(s.mobile(3)._next,7);
	EXPECT_EQ(s.mobile(7)._next,-1);

	delete move;
}

//---MoveSwap2Routes---Commit-Date--------------------------------------------3
TEST (MoveSwap2Routes, CommitDate)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveSwap2Routes<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	Problem p0("../tests/data/test_40m_1i_0");
	Heuristic_sequence h0(p0);
	h0.run({1});
	Solution s0 = h0.solution();

	EXPECT_NEAR(s.lastInterceptionTime(0),s0.lastInterceptionTime(0),1e-6);
	delete move;
}

#endif
