#ifndef __MOVE2OPT_TESTS_HPP__
#define __MOVE2OPT_TESTS_HPP__

//-------------- Tests de la class Move2Opt dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---Move2Opt---Scan---------------True---------------------------------------1
TEST (Move2OptTest, Scan_true)
{
	bool resultScan;
	Problem p("../examples/test_30m_4i_3d");
	Heuristic_fastest<> h(p);
	h.run();

	Solution s = h.solution();

	AvailablePolicy::maxAcceptableTime() = 256.;
	AvailablePolicy::minAcceptableCount() = 1;

	Move2Opt<FirstAvailablePolicy> * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	EXPECT_TRUE(resultScan);

	delete move;
}

//---Move2Opt---Commit--------------------------------------------------------2
TEST (Move2OptTest, Commit)
{
	bool resultScan;
	Problem p("../examples/test_30m_4i_3d");
	Heuristic_fastest<> h(p);
	h.run();

	Solution s = h.solution();

	Move * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 0
	EXPECT_EQ(s[0]._first,21);
	EXPECT_EQ(s.mobile(21)._next,16);
	EXPECT_EQ(s.mobile(16)._next,24);
	EXPECT_EQ(s.mobile(24)._next,17);
	EXPECT_EQ(s.mobile(17)._next,15);
	EXPECT_EQ(s.mobile(15)._next,26);
	EXPECT_EQ(s.mobile(26)._next,23);
	EXPECT_EQ(s.mobile(23)._next,19);
	EXPECT_EQ(s.mobile(19)._next,7);
	EXPECT_EQ(s.mobile(7)._next,4);
	EXPECT_EQ(s.mobile(4)._next,1);
	EXPECT_EQ(s.mobile(1)._next,-1);

	// interceptor 1
	EXPECT_EQ(s[1]._first,10);
	EXPECT_EQ(s.mobile(10)._next,8);
	EXPECT_EQ(s.mobile(8)._next,11);
	EXPECT_EQ(s.mobile(11)._next,-1);

	delete move;
}

//---Move2Opt---Commit-Date---------------------------------------------------3
TEST (Move2OptTest, CommitDate)
{
	bool resultScan;
	Problem p("../examples/test_30m_4i_3d");
	Heuristic_fastest<> h(p);
	h.run();

	Solution s = h.solution();

	Move * move = new Move2Opt<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	Problem p0("../examples/test_30m_4i_3d");
	Heuristic_sequence h0(p0);
	h0.run({20,10,8,11});
	Solution s0 = h0.solution();

	EXPECT_NEAR(s.lastInterceptionTime(1),s0.lastInterceptionTime(1),1e-6);

	delete move;
}

#endif // MOVE2OPT_TESTS_H
