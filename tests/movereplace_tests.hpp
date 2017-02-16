#ifndef __MOVEREPLACE_TESTS_HPP__
#define __MOVEREPLACE_TESTS_HPP__

#include <iostream>
#include "heuristic_sequence.hpp"
#include "move.hpp"

//-------------- Tests de la class ReplaceMove dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---ReplaceMove---Scan---True------------------------------------------------1
TEST (MoveReplaceTest, Scan_true)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,5,6});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	std::cout << h << std::endl;

	Move * move = new MoveReplace<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	EXPECT_TRUE(resultScan);

	delete move;
}

//---CommitTests---------------------------------------------------------------
//---MoveReplace---ReplaceRoute1Mobile---Commit-------------------------------2
TEST (MoveReplaceTest, Commit_ReplaceRoute1Mobile)
{
	bool resultScan = false;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,5,6});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveReplace<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 1
	EXPECT_EQ(s[1]._first,4);
	EXPECT_EQ(s.mobile(4)._next,-1);

	Problem p1("../tests/data/test_40m_1i_1");
	Heuristic_sequence h1(p1);
	h1.run({4});
	Solution s1 = h1.solution();

	EXPECT_NEAR(s.lastInterceptionTime(1),s1.lastInterceptionTime(0),1e-6);

	delete move;
}

#endif
