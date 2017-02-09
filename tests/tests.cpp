#include <gtest/gtest.h>

#include "depot_tests.hpp"
#include "mobile_tests.hpp"
#include "interceptor_tests.hpp"
#include "problem_tests.hpp"
#include "fastest_tests.hpp"
#include "sequential_tests.hpp"
#include "insertmove_tests.hpp"
//#include "deletemove_tests.hpp"
//#include "crossmove_tests.hpp"
#include "move2opt_tests.hpp"

int main(int argc,char * argv[]) {
 ::testing::InitGoogleTest(&argc,argv);
 //::testing::GTEST_FLAG(filter) = "SequentialTest*:FastestTest*:ProblemTest.ConstructorFile*";
 ::testing::GTEST_FLAG(filter) = "Move2Opt*"; //"InsertMoveTest*:DeleteMoveTest*";
 return RUN_ALL_TESTS();
}
