#include <gtest/gtest.h>

#include "depot_tests.hpp"
#include "mobile_tests.hpp"
#include "interceptor_tests.hpp"
#include "problem_tests.hpp"
#include "fastest_tests.hpp"
#include "sequential_tests.hpp"
#include "insertmove_tests.hpp"
#include "extractmove_tests.hpp"
#include "move2opt_tests.hpp"
#include "moveswap1route_tests.hpp"
#include "moveswap2routes_tests.hpp"
#include "movemove1route_tests.hpp"
#include "movereplace_tests.hpp"

int main(int argc,char * argv[]) {
 ::testing::InitGoogleTest(&argc,argv);
 //::testing::GTEST_FLAG(filter) = "SequentialTest*:FastestTest*:ProblemTest.ConstructorFile*";
 //::testing::GTEST_FLAG(filter) = "MoveReplace*"; //"InsertMoveTest*:DeleteMoveTest*";
 return RUN_ALL_TESTS();
}
