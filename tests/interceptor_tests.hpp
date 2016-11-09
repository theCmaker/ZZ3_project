#ifndef __INTERCEPTOR_TESTS_HPP__
#define __INTERCEPTOR_TESTS_HPP__

#include "interceptor.hpp"

//--------- Tests de interceptor.hpp ------------------------------------------------

//----------------------------------------------------------------------------1
TEST (InterceptorTest, ConstructorVoid)
{
	Interceptor i;
	
	EXPECT_DOUBLE_EQ(i.position()._x, 0.);
	EXPECT_DOUBLE_EQ(i.position()._y, 0.);
	EXPECT_DOUBLE_EQ(i.speed(), 0.);
	
}

//----------------------------------------------------------------------------2
TEST (InterceptorTest, ConstructorDistSpeed)
{
	Interceptor i(1.,3.,2.);
	
	EXPECT_DOUBLE_EQ(i.position()._x, 1.);
	EXPECT_DOUBLE_EQ(i.position()._y, 3.);
	EXPECT_DOUBLE_EQ(i.speed(), 2.);
	
}

//----------------------------------------------------------------------------3
TEST (InterceptorTest, ConstructorLocSpeed)
{
	Location l;
	l._x = 1.;
	l._y = 2.;
	
	Interceptor i(l,4.);
	
	EXPECT_DOUBLE_EQ(i.position()._x, l._x);
	EXPECT_DOUBLE_EQ(i.position()._y, l._y);
	EXPECT_DOUBLE_EQ(i.speed(), 4.);
	
}


#endif
