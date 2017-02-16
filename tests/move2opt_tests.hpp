#ifndef __MOVE2OPT_TESTS_HPP__
#define __MOVE2OPT_TESTS_HPP__

//-------------- Tests de la class Move2Opt dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---Move2Opt---Scan---------------True---------------------------------------1
TEST (Move2OptTest, Scan_true)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move2Opt<FirstAvailablePolicy> * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	EXPECT_TRUE(resultScan);

	delete move;
}

//---Move2Opt---Commit--------------------------------------------------------2
TEST (Move2OptTest, Commit)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 0
	EXPECT_EQ(s[0]._first,3);
	EXPECT_EQ(s.mobile(3)._next,7);
	EXPECT_EQ(s.mobile(7)._next,-1);

	// interceptor 1
	EXPECT_EQ(s[1]._first,9);
	EXPECT_EQ(s.mobile(9)._next,-1);

	delete move;
}

//---Move2Opt---Commit-Date---------------------------------------------------3
TEST (Move2OptTest, CommitDate)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	Problem p0("../tests/data/test_40m_1i_0");
	Heuristic_sequence h0(p0);
	h0.run({3,7});
	Solution s0 = h0.solution();

	EXPECT_NEAR(s.lastInterceptionTime(0),s0.lastInterceptionTime(0),1e-6);

	Problem p1("../tests/data/test_40m_1i_1");
	Heuristic_sequence h1(p1);
	h1.run({9});
	Solution s1 = h1.solution();

	EXPECT_NEAR(s.lastInterceptionTime(1),s1.lastInterceptionTime(0),1e-6);

	delete move;
}

#endif // MOVE2OPT_TESTS_H
