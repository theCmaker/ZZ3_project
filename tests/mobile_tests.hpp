#ifndef __MOBILE_TESTS_HPP__
#define __MOBILE_TESTS_HPP__

#include <core/mobile.hpp>

//--------- Tests de mobile.hpp ------------------------------------------------

//----------------------------------------------------------------------------1
TEST (MobileTest, ConstructorVoid)
{
	Mobile m;
	
	EXPECT_DOUBLE_EQ(m.position()._x, 0.);
	EXPECT_DOUBLE_EQ(m.position()._y, 0.);
	EXPECT_DOUBLE_EQ(m.direction()._sx, 0.);
	EXPECT_DOUBLE_EQ(m.direction()._sy, 0.);
	
}

//----------------------------------------------------------------------------2
TEST (MobileTest, ConstructorDistance)
{
	Mobile m(1.,2.,3.,4.);
	
	EXPECT_DOUBLE_EQ(m.position()._x, 1.);
	EXPECT_DOUBLE_EQ(m.position()._y, 2.);
	EXPECT_DOUBLE_EQ(m.direction()._sx, 3.);
	EXPECT_DOUBLE_EQ(m.direction()._sy, 4.);
	
}

//----------------------------------------------------------------------------3
TEST (MobileTest, ConstructorLocDir)
{
	Location l;
	l._x = 1.;
	l._y = 2.;
	
	Direction d;
	d._sx = 3.;
	d._sy = 4.;
	
	Mobile m(l,d);
	
	EXPECT_DOUBLE_EQ(m.position()._x, l._x);
	EXPECT_DOUBLE_EQ(m.position()._y, l._y);
	EXPECT_DOUBLE_EQ(m.direction()._sx, d._sx);
	EXPECT_DOUBLE_EQ(m.direction()._sy, d._sy);
	
}

//----------------------------------------------------------------------------4
TEST (MobileTest, Speed)
{
	Mobile m(1.,2.,2.,1.);
	
	EXPECT_DOUBLE_EQ(m.speed(), sqrt(2.*2.+1.*1.));
}

#endif
