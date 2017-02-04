#ifndef __INSERTMOVE_TESTS_HPP__
#define __INSERTMOVE_TESTS_HPP__

#include <iostream>
#include "heuristic_sequence.hpp"
#include "move.hpp"

//-------------- Tests de la class InsertMove dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---InsertMove---InsertionAtEnd---True---------------------------------------1
TEST (InsertMoveTest, Scan_InsertionAtEnd_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	InsertMove move(p, m, 1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}

//---InsertMove---InsertionAtEnd---False--------------------------------------2
TEST (InsertMoveTest, Scan_InsertionAtEnd_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[1];
	
	InsertMove move(p, m, 3, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_FALSE(resultScan);
}

//---InsertMove---InsertionInTheMiddle---True---------------------------------3
TEST (InsertMoveTest, Scan_InsertionInTheMiddle_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	InsertMove move(p, m, 0, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}

//---InsertMove---InsertionInTheMiddle---False--------------------------------4
TEST (InsertMoveTest, Scan_InsertionInTheMiddle_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[1];
	
	InsertMove move(p, m, 2, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_FALSE(resultScan);
}

//---InsertMove---InsertionAhead---True---------------------------------------5
TEST (InsertMoveTest, Scan_InsertionAhead_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	InsertMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}

//---InsertMove---InsertionAhead---False--------------------------------------6
TEST (InsertMoveTest, Scan_InsertionAhead_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[1];
	
	InsertMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_FALSE(resultScan);
}

//---CommitTests---------------------------------------------------------------
//---InsertMove---InsertionAtEnd---Commit-------------------------------------7
TEST (InsertMoveTest, Commit_InsertionAtEnd)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	InsertMove move(p, m, 1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 0
	EXPECT_EQ(s[0]._first,0);
	// mobile 1
	EXPECT_EQ(s.mobile(0)._next,1);
	// mobile 4 -> insertion at end
	EXPECT_EQ(s.mobile(1)._next,4);
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(4)._next,-1);
	
	//intercepteur 1
	EXPECT_EQ(s[0]._next,1);
	// mobile 2
	EXPECT_EQ(s[1]._first,2);
	// mobile 3
	EXPECT_EQ(s.mobile(2)._next,3);
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(3)._next,-1);
	
	// pas d'autre intercepteur
	EXPECT_EQ(s[1]._next,-1);
}

//---InsertMove---InsertionInTheMiddle---Commit-------------------------------8
TEST (InsertMoveTest, Commit_InsertionInTheMiddle)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	InsertMove move(p, m, 0, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 0
	EXPECT_EQ(s[0]._first,0);
	// mobile 4 -> insertion in the middle
	EXPECT_EQ(s.mobile(0)._next,4);
	// mobile 1
	EXPECT_EQ(s.mobile(4)._next,1);
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(1)._next,-1);
	
	//intercepteur 1
	EXPECT_EQ(s[0]._next,1);
	// mobile 2
	EXPECT_EQ(s[1]._first,2);
	// mobile 3
	EXPECT_EQ(s.mobile(2)._next,3);
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(3)._next,-1);
	
	// pas d'autre intercepteur
	EXPECT_EQ(s[1]._next,-1);
}

//---InsertMove---InsertionAhead---Commit-------------------------------------9
TEST (InsertMoveTest, Commit_InsertionAhead)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	InsertMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.lastInterceptionTime(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.lastInterceptionTime(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 4 -> insertion ahead
	EXPECT_EQ(s[0]._first,4);
	// mobile 0
	EXPECT_EQ(s.mobile(4)._next,0);
	// mobile 1
	EXPECT_EQ(s.mobile(0)._next,1);
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(1)._next,-1);
	
	//intercepteur 1
	EXPECT_EQ(s[0]._next,1);
	// mobile 2
	EXPECT_EQ(s[1]._first,2);
	// mobile 3
	EXPECT_EQ(s.mobile(2)._next,3);
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(3)._next,-1);
	
	// pas d'autre intercepteur
	EXPECT_EQ(s[1]._next,-1);
}


#endif
