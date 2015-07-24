#pragma once

#include "core.h"
#include "instruction_set.h"

#include "gtest/gtest.h"

template <typename T>
struct InstructionTest : public ::testing::Test {
    InstructionTest()
        : memory(32) {
    }

    T descriptor;
    Core core;
    std::vector<Instruction> memory;

    void test_r_instruction_3_args_assemble() {
        ASSERT_NO_THROW(descriptor.assemble({"R1", "R2", "R3"}));
        ASSERT_THROW(descriptor.assemble({"R1", "R2", "R33"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({"xyz", "R1", "R2"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({"xyz"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({}), std::runtime_error);

        auto ti = descriptor.assemble({"R1", "R2", "R3"});
        ASSERT_EQ(ti.r.funct, descriptor.get_id_code());
        ASSERT_EQ(ti.r.rd, 1);
        ASSERT_EQ(ti.r.rs, 2);
        ASSERT_EQ(ti.r.rt, 3);
    }

    void test_r_instruction_3_args_disassemble() {
        Instruction ti;
        ti.r.funct = descriptor.get_id_code();
        ti.r.rd = 1;
        ti.r.rs = 2;
        ti.r.rt = 3;
        ASSERT_EQ(descriptor.disassemble(ti), "R1 R2 R3");
    }

    void test_i_instruction_3_args_assemble() {
        ASSERT_NO_THROW(descriptor.assemble({"R1", "R2", "5"}));
        ASSERT_NO_THROW(descriptor.assemble({"R1", "R2", "0x52"}));
        ASSERT_THROW(descriptor.assemble({"R1", "R2", "R3"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({"R1", "R2", "R33"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({"R1", "R2", "xyz"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({"xyz", "R1", "R2"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({"xyz"}), std::runtime_error);
        ASSERT_THROW(descriptor.assemble({}), std::runtime_error);

        auto ti = descriptor.assemble({"R1", "R2", "3"});
        ASSERT_EQ(ti.i.op, descriptor.get_id_code());
        ASSERT_EQ(ti.i.rt, 1);
        ASSERT_EQ(ti.i.rs, 2);
        ASSERT_EQ(ti.i.immediate, 3);

        ti = descriptor.assemble({"R1", "R2", "-53"});
        ASSERT_EQ(ti.i.op, descriptor.get_id_code());
        ASSERT_EQ(ti.i.rt, 1);
        ASSERT_EQ(ti.i.rs, 2);
        ASSERT_EQ(ti.i.immediate, -53);

        ti = descriptor.assemble({"R1", "R2", "0xab"});
        ASSERT_EQ(ti.i.op, descriptor.get_id_code());
        ASSERT_EQ(ti.i.rt, 1);
        ASSERT_EQ(ti.i.rs, 2);
        ASSERT_EQ(ti.i.immediate, 0xab);
    }

    void test_i_instruction_3_args_disassemble() {
        Instruction ti;
        ti.i.op = descriptor.get_id_code();
        ti.i.rt = 1;
        ti.i.rs = 2;
        ti.i.immediate = 56;
        ASSERT_EQ(descriptor.disassemble(ti), "R1 R2 56");
    }
};

class InstructionTest__NOP : public InstructionTest<NOP> {};

class InstructionTest__ADD_R : public InstructionTest<ADD_R> {};
class InstructionTest__ADD_I : public InstructionTest<ADD_I> {};
class InstructionTest__SUB_R : public InstructionTest<SUB_R> {};
class InstructionTest__SUB_I : public InstructionTest<SUB_I> {};
class InstructionTest__MUL_R : public InstructionTest<MUL_R> {};
class InstructionTest__MUL_I : public InstructionTest<MUL_I> {};
class InstructionTest__DIV_R : public InstructionTest<DIV_R> {};
class InstructionTest__DIV_I : public InstructionTest<DIV_I> {};
class InstructionTest__BAND_R : public InstructionTest<BAND_R> {};
class InstructionTest__BAND_I : public InstructionTest<BAND_I> {};
class InstructionTest__BOR_R : public InstructionTest<BOR_R> {};
class InstructionTest__BOR_I : public InstructionTest<BOR_I> {};
class InstructionTest__BNOT_R : public InstructionTest<BNOT_R> {};
class InstructionTest__BNOT_I : public InstructionTest<BNOT_I> {};
class InstructionTest__BXOR_R : public InstructionTest<BXOR_R> {};
class InstructionTest__BXOR_I : public InstructionTest<BXOR_I> {};
class InstructionTest__AND_R : public InstructionTest<AND_R> {};
class InstructionTest__AND_I : public InstructionTest<AND_I> {};
class InstructionTest__OR_R : public InstructionTest<OR_R> {};
class InstructionTest__OR_I : public InstructionTest<OR_I> {};
class InstructionTest__NOT_R : public InstructionTest<NOT_R> {};
class InstructionTest__NOT_I : public InstructionTest<NOT_I> {};
class InstructionTest__MOVE_R : public InstructionTest<MOVE_R> {};
class InstructionTest__MOVE_I : public InstructionTest<MOVE_I> {};
class InstructionTest__OSC_R : public InstructionTest<OSC_R> {};
class InstructionTest__OSC_I : public InstructionTest<OSC_I> {};

class InstructionTest__LOAD : public InstructionTest<LOAD> {};
class InstructionTest__SAVE : public InstructionTest<SAVE> {};
class InstructionTest__JUMP : public InstructionTest<JUMP> {};
class InstructionTest__JE : public InstructionTest<JE> {};
class InstructionTest__JNE : public InstructionTest<JNE> {};
class InstructionTest__JL : public InstructionTest<JL> {};
class InstructionTest__JLE : public InstructionTest<JLE> {};
class InstructionTest__JG : public InstructionTest<JG> {};
class InstructionTest__JGE : public InstructionTest<JGE> {};
class InstructionTest__RND : public InstructionTest<RND> {};
