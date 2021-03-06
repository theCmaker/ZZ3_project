#ifndef __INSERTMOVE_TESTS_HPP__
#define __INSERTMOVE_TESTS_HPP__

#include <heuristics/heuristic_sequence.hpp>
#include <movements/move_insert.hpp>

//---------- Tests de la class InsertMove dans move_insert.hpp ----------------

//---ScanTests-----------------------------------------------------------------
//---InsertMove---Scan---False------------------------------------------------1
TEST (InsertMoveTest, Scan_false)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,5,6});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();

	Move * move = new MoveInsert<FirstAvailablePolicy>;
	resultScan = move->scan(s);
	
	EXPECT_FALSE(resultScan);

	delete move;
}

//---CommitTests---------------------------------------------------------------
//---InsertMove---InsertionEmptyRoute---Commit--------------------------------2
TEST (InsertMoveTest, Commit_InsertionEmptyRoute)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,5,6});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	AvailablePolicy::maxAcceptableTime() = 60.;
	AvailablePolicy::minAcceptableCount() = 1;
	Move * move = new MoveInsert<FirstAvailablePolicy>;
	resultScan = move->scan(s);
	
	ASSERT_TRUE(resultScan);
	
	move->commit(s);
	
	// interceptor 0
	EXPECT_EQ(s[0]._first,4);
	EXPECT_EQ(s.mobile(4)._next,-1);

	Problem p0("../tests/data/test_40m_1i_0");
	Heuristic_sequence h0(p0);
	h0.run({4});
	Solution s0 = h0.solution();

	EXPECT_NEAR(s.lastInterceptionTime(0),s0.lastInterceptionTime(0),1e-6);

	delete move;
}

//---InsertMove---InsertionAhead---Commit-------------------------------------3
TEST (InsertMoveTest, Commit_InsertionAhead)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9,10});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Move * move = new MoveInsert<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);
	
	// interceptor 0
	EXPECT_EQ(s[0]._first,11);
	EXPECT_EQ(s.mobile(11)._next,9);
	EXPECT_EQ(s.mobile(9)._next,-1);

	Problem p0("../tests/data/test_40m_1i_0");
	Heuristic_sequence h0(p0);
	h0.run({11,9});
	Solution s0 = h0.solution();

	EXPECT_NEAR(s.lastInterceptionTime(0),s0.lastInterceptionTime(0),1e-6);

	delete move;
}

//---InsertMove---InsertionInTheMiddle---Commit-------------------------------------4
TEST (InsertMoveTest, Commit_InsertionInTheMiddle)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4,5,6,7,8,9});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Move * move = new MoveInsert<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);
	
	// interceptor 1
	EXPECT_EQ(s[1]._first,10);
	EXPECT_EQ(s.mobile(10)._next,3);
	EXPECT_EQ(s.mobile(3)._next,7);
	EXPECT_EQ(s.mobile(7)._next,-1);

	Problem p1("../tests/data/test_40m_1i_1");
	Heuristic_sequence h1(p1);
	h1.run({10,3,7});
	Solution s1 = h1.solution();

	EXPECT_NEAR(s.lastInterceptionTime(1),s1.lastInterceptionTime(0),1e-6);

	delete move;
}

//---InsertMove---InsertionAtEnd---Commit-------------------------------------5
TEST (InsertMoveTest, Commit_InsertionAtEnd)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,5,6,7,8,9,10,11});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveInsert<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 0
	EXPECT_EQ(s[0]._first,4);
	EXPECT_EQ(s.mobile(4)._next,9);
	EXPECT_EQ(s.mobile(9)._next,-1);

	Problem p0("../tests/data/test_40m_1i_0");
	Heuristic_sequence h0(p0);
	h0.run({4,9});
	Solution s0 = h0.solution();

	EXPECT_NEAR(s.lastInterceptionTime(0),s0.lastInterceptionTime(0),1e-6);

	delete move;
}


#endif
