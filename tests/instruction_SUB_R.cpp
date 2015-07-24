#include "instruction_fixture.h"

using namespace std;

TEST_F(InstructionTest__SUB_R, assemble) {
    test_r_instruction_3_args_assemble();
}

TEST_F(InstructionTest__SUB_R, disassemble) {
    test_r_instruction_3_args_disassemble();
}

TEST_F(InstructionTest__SUB_R, execute) {
    auto ti = descriptor.assemble({"R1", "R2", "R3"});

    core.reg[1] = 0;
    core.reg[2] = 2;
    core.reg[3] = 3;

    descriptor.execute(core, memory, ti);

    ASSERT_EQ(-1, core.reg[1]);
    ASSERT_EQ(2, core.reg[2]);
    ASSERT_EQ(3, core.reg[3]);

    core.reg[1] = 0;
    core.reg[2] = -2;
    core.reg[3] = -4;

    descriptor.execute(core, memory, ti);

    ASSERT_EQ(2, core.reg[1]);
    ASSERT_EQ(-2, core.reg[2]);
    ASSERT_EQ(-4, core.reg[3]);
}

