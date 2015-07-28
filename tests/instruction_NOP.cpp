#include "instruction_fixture.h"

using namespace std;

TEST_F(InstructionTest__NOP, assemble) {
    ASSERT_NO_THROW(descriptor.assemble({}));
    ASSERT_THROW(descriptor.assemble({"xyz"}), runtime_error);
    ASSERT_EQ(descriptor.assemble({}).r.funct, descriptor.get_id_code());
}

TEST_F(InstructionTest__NOP, disassemble) {
    Instruction ti;
    ti.r.funct = descriptor.get_id_code();
    ASSERT_EQ(descriptor.disassemble(ti), "NOP");
}

TEST_F(InstructionTest__NOP, execute) {
    auto ti = descriptor.assemble({});

    auto a = core;
    descriptor.execute(core, memory, ti);
    auto b = core;

    ASSERT_EQ(a, b);
}
