#ifndef __FASTEST_TESTS_HPP__
#define __FASTEST_TESTS_HPP__

#include "heuristic_h1.hpp"

//----------------------------------------------------------------------------1
TEST (FastestTest, Test_5m_1i)
{
	Problem p("../examples/test_5m_1i");
	Heuristic_h1 h1(p);
	h1.run();
	
	Solution s = h1.solution();
	
	// interceptor
	EXPECT_EQ(s.first(),0);
	
	// mobile 0 ; pos(0,2) ; t(1)
	EXPECT_EQ(s[0]._first,0);
	EXPECT_EQ(s.mobile(0)._mobile.position(s.mobile(0)._date)._x,0);
	EXPECT_EQ(s.mobile(0)._mobile.position(s.mobile(0)._date)._y,2);
	EXPECT_EQ(s.mobile(0)._date,1);
	
	// mobile 1 ; pos(0,2) ; t(1)
	EXPECT_EQ(s.mobile(0)._next,1);
	EXPECT_EQ(s.mobile(1)._mobile.position(s.mobile(1)._date)._x,0);
	EXPECT_EQ(s.mobile(1)._mobile.position(s.mobile(1)._date)._y,2);
	EXPECT_EQ(s.mobile(1)._date,1);
	
	// mobile 2 ; pos(7.323262,6.596979) ; t(5.323262)
	EXPECT_EQ(s.mobile(1)._next,2);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._x,7.323262,1e-6);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._y,6.596979,1e-6);
	EXPECT_NEAR(s.mobile(2)._date,5.323262,1e-6);
	
	// mobile 3 ; pos(7.248936,4.248936) ; t(6.497872)
	EXPECT_EQ(s.mobile(2)._next,3);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._x,7.248936,1e-6);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._y,4.248936,1e-6);
	EXPECT_NEAR(s.mobile(3)._date,6.497872,1e-6);
	
	// mobile 4 ; pos(-8.089226,5.089226) ; t(14.178453)
	EXPECT_EQ(s.mobile(3)._next,4);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._x,-8.089226,1e-6);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._y,5.089226,1e-6);
	EXPECT_NEAR(s.mobile(4)._date,14.178453,1e-6);
	
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(4)._next,-1);
	
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
	Heuristic_fastest h0(p);
	h0.run();
	
	Solution s = h0.solution();
	
	// interceptor
	EXPECT_EQ(s.first(),0);
	
	// mobile 0 ; pos(7.140297,8.876400) ; t(2.570891)
	EXPECT_EQ(s[0]._first,0);
	EXPECT_NEAR(s.mobile(0)._mobile.position(s.mobile(0)._date)._x,7.140297,1e-6);
	EXPECT_NEAR(s.mobile(0)._mobile.position(s.mobile(0)._date)._y,8.876400,1e-6);
	EXPECT_NEAR(s.mobile(0)._date,2.570891,1e-6);
	
	// mobile 1 ; pos(-1.586399,3.020660) ; t(7.825530)
	EXPECT_EQ(s.mobile(0)._next,1);
	EXPECT_NEAR(s.mobile(1)._mobile.position(s.mobile(1)._date)._x,-1.586399,1e-6);
	EXPECT_NEAR(s.mobile(1)._mobile.position(s.mobile(1)._date)._y,3.020660,1e-6);
	EXPECT_NEAR(s.mobile(1)._date,7.825530,1e-6);
	
	// mobile 2 ; pos(-3.277717,3.651746) ; t(8.728141)
	EXPECT_EQ(s.mobile(1)._next,2);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._x,-3.277717,1e-6);
	EXPECT_NEAR(s.mobile(2)._mobile.position(s.mobile(2)._date)._y,3.651746,1e-6);
	EXPECT_NEAR(s.mobile(2)._date,8.728141,1e-6);
	
	// mobile 3 ; pos(-12.292086,-2.639952) ; t(14.224601)
	EXPECT_EQ(s.mobile(2)._next,3);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._x,-12.292086,1e-6);
	EXPECT_NEAR(s.mobile(3)._mobile.position(s.mobile(3)._date)._y,-2.639952,1e-6);
	EXPECT_NEAR(s.mobile(3)._date,14.224601,1e-6);
	
	// mobile 4 ; pos(-2.280671,6.690032) ; t(21.067061)
	EXPECT_EQ(s.mobile(3)._next,4);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._x,-2.280671,1e-6);
	EXPECT_NEAR(s.mobile(4)._mobile.position(s.mobile(4)._date)._y,6.690032,1e-6);
	EXPECT_NEAR(s.mobile(4)._date,21.067061,1e-6);
	
	// mobile 5 ; pos(-3.539516,-3.591557) ; t(26.246245)
	EXPECT_EQ(s.mobile(4)._next,5);
	EXPECT_NEAR(s.mobile(5)._mobile.position(s.mobile(5)._date)._x,-3.539516,1e-6);
	EXPECT_NEAR(s.mobile(5)._mobile.position(s.mobile(5)._date)._y,-3.591557,1e-6);
	EXPECT_NEAR(s.mobile(5)._date,26.246245,1e-6);
	
	// pas d'autre mobile
	EXPECT_EQ(s.mobile(3)._next,-1);
	
	// pas d'autre intercepteur
	EXPECT_EQ(s[0]._next,-1);
}

#endif
