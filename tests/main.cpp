#include "gtest/gtest.h"
//  add other test files here
#include "instruction_fixture.h"

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
