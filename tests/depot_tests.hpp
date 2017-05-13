#ifndef __DEPOT_TESTS_HPP__
#define __DEPOT_TESTS_HPP__

#include <core/depot.hpp>

//--------- Tests de depot.hpp ------------------------------------------------

//----------------------------------------------------------------------------1
TEST (DepotTest, ConstructorVoid)
{
	Depot d;
	
	EXPECT_DOUBLE_EQ(d.position()._x, 0.);
	EXPECT_DOUBLE_EQ(d.position()._y, 0.);
	
	// + tester le vecteur _interceptors
	
}

//----------------------------------------------------------------------------2
TEST (DepotTest, ConstructorDistance)
{
	Depot d(1.,1.);
	
	EXPECT_DOUBLE_EQ(d.position()._x, 1.);
	EXPECT_DOUBLE_EQ(d.position()._y, 1.);
	
	// + tester le vecteur _interceptors
}

//----------------------------------------------------------------------------3
TEST (DepotTest, ConstructorLocation)
{
	Location l;
	l._x = 1.;
	l._y = 1.;
	
	Depot d(l);
	
	EXPECT_DOUBLE_EQ(d.position()._x, l._x);
	EXPECT_DOUBLE_EQ(d.position()._y, l._y);
	
	// + tester le vecteur _interceptors
}



#endif
