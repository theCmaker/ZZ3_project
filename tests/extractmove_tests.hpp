#ifndef __EXTRACTMOVE_TESTS_HPP__
#define __EXTRACTMOVE_TESTS_HPP__

#include <iostream>
#include "heuristic_sequence.hpp"
#include "move.hpp"

//-------------- Tests de la class ExtractMove dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---ExtractMove---Scan---True-------------------------------------------------1
TEST (ExtractMoveTest, Scan_true)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	std::cout << h << std::endl;

	Solution s = h.solution();

	Move * move = new MoveExtract<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	EXPECT_TRUE(resultScan);

	delete move;
}

//---CommitTests---------------------------------------------------------------
//---ExtractMove---DeletionAhead---Commit-------------------------------------2
TEST (ExtractMoveTest, Commit_DeletionAhead)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	Move * move = new MoveExtract<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 2
	EXPECT_EQ(s[2]._first,2);
	EXPECT_EQ(s.mobile(2)._next,-1);

	Problem p2("../tests/data/test_40m_1i_2");
	Heuristic_sequence h2(p2);
	h2.run({2});
	Solution s2 = h2.solution();

	EXPECT_NEAR(s.lastInterceptionTime(2),s2.lastInterceptionTime(0),1e-6);
}

//---ExtractMove---DeletionLastMobile---Commit--------------------------------3
TEST (ExtractMoveTest, Commit_DeletionLastMobile)
{
	bool resultScan;
	Problem p("../examples/test_40m_5i_4d");
	std::vector<unsigned> expected_sequence({0,1,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);

	Solution s = h.solution();

	std::cout << h << std::endl;

	Move * move = new MoveExtract<FirstAvailablePolicy>;
	resultScan = move->scan(s);

	ASSERT_TRUE(resultScan);

	move->commit(s);

	// interceptor 2
	EXPECT_EQ(s[2]._first,-1);
	EXPECT_EQ(s[2]._last,-1);
}


#endif
