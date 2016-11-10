#ifndef __PROBLEM_TESTS_HPP__
#define __PROBLEM_TESTS_HPP__

#include "problem.hpp"

//--------- Tests de problem.hpp ------------------------------------------------

//----------------------------------------------------------------------------1
TEST (ProblemTest, ConstructorVoid)
{
	Problem p;
	
	EXPECT_EQ(p.nbInterceptors(), 0u);
	EXPECT_EQ(p.nbMobiles(), 0u);
	EXPECT_EQ(p.nbDepots(), 0u);
	
}

//----------------------------------------------------------------------------2
TEST (ProblemTest, ConstructorFile)
{
	Problem p("../examples/test_5m_1i");
	
	// interceptors
	EXPECT_EQ(p.nbInterceptors(), 1u);
	
	EXPECT_EQ(p.interceptors()[0].position(),Location(0.,0.));
	EXPECT_EQ(p.interceptors()[0].speed(),2.);
	
	// mobiles
	EXPECT_EQ(p.nbMobiles(), 5u);
	
	EXPECT_EQ(p.mobiles()[0].position(), Location(1.,2.));
	EXPECT_EQ(p.mobiles()[0].direction(), Direction(-1.,0.));
	
	EXPECT_EQ(p.mobiles()[1].position(), Location(0.,3.));
	EXPECT_EQ(p.mobiles()[1].direction(), Direction(0.,-1.));
	
	EXPECT_EQ(p.mobiles()[2].position(), Location(2.,5.));
	EXPECT_EQ(p.mobiles()[2].direction(), Direction(1.,0.3));
	
	EXPECT_EQ(p.mobiles()[3].position(), Location(4.,1.));
	EXPECT_EQ(p.mobiles()[3].direction(), Direction(0.5,0.5));
	
	EXPECT_EQ(p.mobiles()[4].position(), Location(-1.,-2.));
	EXPECT_EQ(p.mobiles()[4].direction(), Direction(-0.5,0.5));
	
	// depots
	EXPECT_EQ(p.nbDepots(), 1u);
	
	EXPECT_EQ(p.depots()[0].position(), Location(0.,0.));

}

//----------------------------------------------------------------------------3
TEST (ProblemTest, ConstructorFile2)
{
	Problem p("../examples/pb_file_test");
	
	// interceptors
	EXPECT_EQ(p.nbInterceptors(), 2u);
	
	EXPECT_EQ(p.interceptors()[0].position(),Location(0.,0.));
	EXPECT_EQ(p.interceptors()[0].speed(),2.);
	
	EXPECT_EQ(p.interceptors()[1].position(),Location(1.,1.));
	EXPECT_EQ(p.interceptors()[1].speed(),3.);
	
	// mobiles
	EXPECT_EQ(p.nbMobiles(), 3u);
	
	EXPECT_EQ(p.mobiles()[0].position(), Location(1.,0.));
	EXPECT_EQ(p.mobiles()[0].direction(), Direction(-1.,0.));
	
	EXPECT_EQ(p.mobiles()[1].position(), Location(0.,-3.));
	EXPECT_EQ(p.mobiles()[1].direction(), Direction(0.,-1.));
	
	EXPECT_EQ(p.mobiles()[2].position(), Location(4.,5.));
	EXPECT_EQ(p.mobiles()[2].direction(), Direction(1.,0.8));
	
	// depots
	EXPECT_EQ(p.nbDepots(), 2u);
	
	EXPECT_EQ(p.depots()[0].position(), Location(0.,0.));
	EXPECT_EQ(p.depots()[1].position(), Location(1.,1.));
	
}

//----------------------------------------------------------------------------4
TEST (ProblemTest, ConstructorCopy)
{
	
	// constructeur non implementé
	
}


#endif
