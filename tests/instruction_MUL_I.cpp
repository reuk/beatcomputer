#include "instruction_fixture.h"

using namespace std;

TEST_F(InstructionTest__MUL_I, assemble) {
    test_i_instruction_3_args_assemble();
}

TEST_F(InstructionTest__MUL_I, disassemble) {
    test_i_instruction_3_args_disassemble();
}

TEST_F(InstructionTest__MUL_I, execute) {
    auto ti = descriptor.assemble({"R1", "R2", "5"});

    core.reg[1] = 0;
    core.reg[2] = 2;

    descriptor.execute(core, memory, ti);

    ASSERT_EQ(10, core.reg[1]);
    ASSERT_EQ(2, core.reg[2]);

    core.reg[1] = 0;
    core.reg[2] = -2;

    descriptor.execute(core, memory, ti);

    ASSERT_EQ(-10, core.reg[1]);
    ASSERT_EQ(-2, core.reg[2]);
}
