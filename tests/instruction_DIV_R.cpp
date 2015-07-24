#include "instruction_fixture.h"

using namespace std;

TEST_F(InstructionTest__DIV_R, assemble) {
    test_r_instruction_3_args_assemble();
}

TEST_F(InstructionTest__DIV_R, disassemble) {
    test_r_instruction_3_args_disassemble();
}

TEST_F(InstructionTest__DIV_R, execute) {
    auto ti = descriptor.assemble({"R1", "R2", "R3"});

    core.reg[1] = 0;
    core.reg[2] = 10;
    core.reg[3] = 2;

    descriptor.execute(core, memory, ti);

    ASSERT_EQ(5, core.reg[1]);
    ASSERT_EQ(10, core.reg[2]);
    ASSERT_EQ(2, core.reg[3]);

    core.reg[1] = 0;
    core.reg[2] = -8;
    core.reg[3] = -4;

    descriptor.execute(core, memory, ti);

    ASSERT_EQ(2, core.reg[1]);
    ASSERT_EQ(-8, core.reg[2]);
    ASSERT_EQ(-4, core.reg[3]);
}

