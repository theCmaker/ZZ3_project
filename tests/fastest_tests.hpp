#ifndef __FASTEST_TESTS_HPP__
#define __FASTEST_TESTS_HPP__

#include <heuristics/heuristic_fastest.hpp>

//----------------------------------------------------------------------------1
TEST (FastestTest, Test_5m_1i)
{
	Problem p("../examples/test_5m_1i");
	Heuristic_fastest<> h0(p);
	h0.run();
	
	Solution s = h0.solution();
	Location expected(0.,2.);
	
	// interceptor
	EXPECT_EQ(s.first(),0);
	
	// mobile 0 ; pos(0,2) ; t(1)
	EXPECT_EQ(s[0]._first,0);
	EXPECT_EQ(expected.equals(s.mobile(0)._mobile.position(s.mobile(0)._date),1e-6),true);
	EXPECT_NEAR(s.mobile(0)._date,1.0,1e-6);
	
	// mobile 1 ; pos(0,2) ; t(1)
	EXPECT_EQ(s.mobile(0)._next,1);
	EXPECT_EQ(expected.equals(s.mobile(1)._mobile.position(s.mobile(1)._date),1e-6),true);
	EXPECT_NEAR(s.mobile(1)._date,1.0,1e-6);
	
	// mobile 4 ; pos(-2.384821,-0.615179) ; t(2.769642)
	EXPECT_EQ(s.mobile(1)._next,4);
	//EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date),Location(-2.384821,-0.615179),1e-6);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._x,-2.384821,1e-6);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._y,-0.615179,1e-6);
	EXPECT_NEAR(s.mobile(4)._date,2.769642,1e-6);
	
	// mobile 3 ; pos(8.509197,5.509197) ; t(9.018394)
	EXPECT_EQ(s.mobile(4)._next,3);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._x,8.509197,1e-6);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._y,5.509197,1e-6);
	EXPECT_NEAR(s.mobile(3)._date,9.018394,1e-6);
	
	// mobile 2 ; pos(14.344612,8.703384) ; t(12.344612)
	EXPECT_EQ(s.mobile(3)._next,2);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._x,14.344612,1e-6);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._y,8.703384,1e-6);
	EXPECT_NEAR(s.mobile(2)._date,12.344612,1e-6);
	
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(2)._next,-1);
	
	// pas d'autre intercepteur
	EXPECT_EQ(s[0]._next,-1);

}


TEST (FastestTest, Test_5m_2i)
{
	//TODO
}

TEST (FastestTest, Test_6m_1i)
{
	Problem p("../examples/test_6m_1i");
	Heuristic_fastest<> h0(p);
	h0.run();
	
	Solution s = h0.solution();
	
	// interceptor
	EXPECT_EQ(s.first(),0);
	
	// mobile 1 ; pos(2.957371,4.644764) ; t(2.229610)
	EXPECT_EQ(s[0]._first,1);
	EXPECT_NEAR(s.mobile(1)._mobile.position(s.mobile(1)._date)._x,2.957371,1e-6);
	EXPECT_NEAR(s.mobile(1)._mobile.position(s.mobile(1)._date)._y,4.644764,1e-6);
	EXPECT_NEAR(s.mobile(1)._date,2.229610,1e-6);
	
	// mobile 4 ; pos(6.317120,6.026740) ; t(4.046046)
	EXPECT_EQ(s.mobile(1)._next,4);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._x,6.317120,1e-6);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._y,6.026740,1e-6);
	EXPECT_NEAR(s.mobile(4)._date,4.046046,1e-6);
	
	// mobile 0 ; pos(7.140297,8.876400) ; t(5.529133)
	EXPECT_EQ(s.mobile(4)._next,0);
	EXPECT_NEAR(s.mobile(0)._mobile.position(s.mobile(0)._date)._x,7.140297,1e-6);
	EXPECT_NEAR(s.mobile(0)._mobile.position(s.mobile(0)._date)._y,8.876400,1e-6);
	EXPECT_NEAR(s.mobile(0)._date,5.529133,1e-6);
	
	// mobile 2 ; pos(-2.411295,5.356014) ; t(10.618976)
	EXPECT_EQ(s.mobile(0)._next,2);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._x,-2.411295,1e-6);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._y,5.356014,1e-6);
	EXPECT_NEAR(s.mobile(2)._date,10.618976,1e-6);
	
	// mobile 5 ; pos(-1.263106,-5.622933) ; t(16.138388)
	EXPECT_EQ(s.mobile(2)._next,5);
	EXPECT_NEAR(s.mobile(5)._mobile.position(s.mobile(5)._date)._x,-1.263106,1e-6);
	EXPECT_NEAR(s.mobile(5)._mobile.position(s.mobile(5)._date)._y,-5.622933,1e-6);
	EXPECT_NEAR(s.mobile(5)._date,16.138388,1e-6);
	
	// mobile 3 ; pos(-14.218112,1.088498) ; t(23.433514)
	EXPECT_EQ(s.mobile(5)._next,3);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._x,-14.218112,1e-6);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._y,1.088498,1e-6);
	EXPECT_NEAR(s.mobile(3)._date,23.433514,1e-6);
	
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(3)._next,-1);
	
	// pas d'autre intercepteur
	EXPECT_EQ(s[0]._next,-1);
}

#endif
