#ifndef __CROSSMOVE_TESTS_HPP__
#define __CROSSMOVE_TESTS_HPP__

#include <iostream>
#include "heuristic_sequence.hpp"
#include "move.hpp"

//-------------- Tests de la class CrossMove dans move.hpp -------------------

//---ScanTests-----------------------------------------------------------------
//---CrossMove---CrossAtEnd---False-------------------------------------------1
// CrossAtEnd will never improve routes
TEST (CrossMoveTest, Scan_CrossAtEnd_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	std::cout << h << std::endl;
	
	Solution s = h.solution();
	
	CrossMove move(p, p.mobiles()[4].id(), p.mobiles()[3].id());
	resultScan = move.scan(s);
	
	EXPECT_FALSE(resultScan);
}

//---CrossMove---CrossInTheMiddle---True---------------------------------3
/*TEST (CrossMoveTest, Scan_CrossInTheMiddle_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[1];
	Interceptor i = p.interceptors()[0];
	
	CrossMove move(p, m, 0, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}*/

//---CrossMove---CrossInTheMiddle---False--------------------------------4
TEST (CrossMoveTest, Scan_CrossInTheMiddle_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Interceptor i = p.interceptors()[1];
	
	CrossMove move(p, p.mobiles()[1].id(), p.mobiles()[2].id());
	resultScan = move.scan(s);
	
	EXPECT_FALSE(resultScan);
}

//---CrossMove---CrossAhead---True---------------------------------------5
/*TEST (CrossMoveTest, Scan_CrossAhead_true)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[0];
	Interceptor i = p.interceptors()[0];
	
	CrossMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
}*/

//---CrossMove---CrossAhead---False--------------------------------------6
TEST (CrossMoveTest, Scan_CrossAhead_false)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3,4});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	CrossMove move(p, p.mobiles()[0].id(), p.mobiles()[2].id());
	resultScan = move.scan(s);
	
	EXPECT_FALSE(resultScan);
}

//---CommitTests---------------------------------------------------------------
//---CrossMove---CrossAtEnd---Commit-------------------------------------7
/*TEST (CrossMoveTest, Commit_CrossAtEnd)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	CrossMove move(p, m, 1, i);
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
	// mobile 4 -> Cross at end
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

//---CrossMove---CrossInTheMiddle---Commit-------------------------------8
/*TEST (CrossMoveTest, Commit_CrossInTheMiddle)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	CrossMove move(p, m, 0, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 0
	EXPECT_EQ(s[0]._first,0);
	// mobile 4 -> Cross in the middle
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

//---CrossMove---CrossAhead---Commit-------------------------------------9
/*TEST (CrossMoveTest, Commit_CrossAhead)
{
	bool resultScan;
	Problem p("../examples/test_5m_2i");
	std::vector<unsigned> expected_sequence({0,1,2,3});
	Heuristic_sequence h(p);
	h.run(expected_sequence);
	
	Solution s = h.solution();
	
	Mobile m = p.mobiles()[4];
	Interceptor i = p.interceptors()[0];
	
	CrossMove move(p, m, -1, i);
	resultScan = move.scan(s);
	
	std::cout << "Interceptor 0 last time: " << s.last_interception_time(p.interceptors()[0]) << std::endl;
	std::cout << "Interceptor 1 last time: " << s.last_interception_time(p.interceptors()[1]) << std::endl;
	
	EXPECT_TRUE(resultScan);
	
	move.commit(s);
	
	// interceptor 0
	EXPECT_EQ(s.first(),0);
	// mobile 4 -> Cross ahead
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
