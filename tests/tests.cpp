#include <gtest/gtest.h>

#include "depot_tests.hpp"
#include "mobile_tests.hpp"
#include "interceptor_tests.hpp"
#include "problem_tests.hpp"
#include "fastest_tests.hpp"
#include "sequential_tests.hpp"
#include "insertmove_tests.hpp"
#include "deletemove_tests.hpp"

int main(int argc,char * argv[]) {
 ::testing::InitGoogleTest(&argc,argv);
 //::testing::GTEST_FLAG(filter) = "SequentialTest*:FastestTest*:ProblemTest.ConstructorFile*";
 ::testing::GTEST_FLAG(filter) = "DeleteMoveTest*"; //"InsertMoveTest*:DeleteMoveTest*";
 return RUN_ALL_TESTS();
}
