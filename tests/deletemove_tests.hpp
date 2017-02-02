#ifndef __DELETEMOVE_TESTS_HPP__
#define __DELETEMOVE_TESTS_HPP__

#include <iostream>
#include "heuristic_sequence.hpp"
#include "move.hpp"

//-------------- Tests de la class DeleteMove dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---DeleteMove---DeletionAtEnd---True---------------------------------------1
TEST (DeleteMoveTest, Scan_DeletionAtEnd_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	std::cout << h << std::endl;
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	DeleteMove move(p, m, 1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}

//---DeleteMove---DeletionAtEnd---False--------------------------------------2
/*TEST (DeleteMoveTest, Scan_DeletionAtEnd_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[1];
	
	DeleteMove move(p, m, 3, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_FALSE(resultScan);
}*/

//---DeleteMove---DeletionInTheMiddle---True---------------------------------3
TEST (DeleteMoveTest, Scan_DeletionInTheMiddle_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[1];
	Interceptor i = p.interceptors()[0];
	
	DeleteMove move(p, m, 0, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}

//---DeleteMove---DeletionInTheMiddle---False--------------------------------4
/*TEST (DeleteMoveTest, Scan_DeletionInTheMiddle_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[1];
	
	DeleteMove move(p, m, 2, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_FALSE(resultScan);
}*/

//---DeleteMove---DeletionAhead---True---------------------------------------5
TEST (DeleteMoveTest, Scan_DeletionAhead_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[0];
	Interceptor i = p.interceptors()[0];
	
	DeleteMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}

//---DeleteMove---DeletionAhead---False--------------------------------------6
/*TEST (DeleteMoveTest, Scan_DeletionAhead_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[1];
	
	DeleteMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_FALSE(resultScan);
}*/

//---CommitTests---------------------------------------------------------------
//---DeleteMove---DeletionAtEnd---Commit-------------------------------------7
/*TEST (DeleteMoveTest, Commit_DeletionAtEnd)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	DeleteMove move(p, m, 1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 0
	EXPECT_EQ(s[0]._first,0);
	// mobile 1
	EXPECT_EQ(s.mobile(0)._next,1);
	// mobile 4 -> deletion at end
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
}*/

//---DeleteMove---DeletionInTheMiddle---Commit-------------------------------8
/*TEST (DeleteMoveTest, Commit_DeletionInTheMiddle)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	DeleteMove move(p, m, 0, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 0
	EXPECT_EQ(s[0]._first,0);
	// mobile 4 -> deletion in the middle
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
}*/

//---DeleteMove---DeletionAhead---Commit-------------------------------------9
/*TEST (DeleteMoveTest, Commit_DeletionAhead)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	DeleteMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 4 -> deletion ahead
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
}*/


#endif
