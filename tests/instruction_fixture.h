#pragma once

#include "core.h"
#include "instruction_set.h"

template<typename T>
struct InstructionTest: public ::testing::Test {
    InstructionTest()
        : memory(32) {
    }

    T descriptor;
    Core core;
    std::vector<Instruction> memory;
};

class InstructionTest__NOP      : public InstructionTest<NOP        > {};

class InstructionTest__ADD_R    : public InstructionTest<ADD_R      > {};
class InstructionTest__ADD_I    : public InstructionTest<ADD_I      > {};
class InstructionTest__SUB_R    : public InstructionTest<SUB_R      > {};
class InstructionTest__SUB_I    : public InstructionTest<SUB_I      > {};
class InstructionTest__MUL_R    : public InstructionTest<MUL_R      > {};
class InstructionTest__MUL_I    : public InstructionTest<MUL_I      > {};
class InstructionTest__DIV_R    : public InstructionTest<DIV_R      > {};
class InstructionTest__DIV_I    : public InstructionTest<DIV_I      > {};
class InstructionTest__BAND_R   : public InstructionTest<BAND_R     > {};
class InstructionTest__BAND_I   : public InstructionTest<BAND_I     > {};
class InstructionTest__BOR_R    : public InstructionTest<BOR_R      > {};
class InstructionTest__BOR_I    : public InstructionTest<BOR_I      > {};
class InstructionTest__BNOT_R   : public InstructionTest<BNOT_R     > {};
class InstructionTest__BNOT_I   : public InstructionTest<BNOT_I     > {};
class InstructionTest__BNOR_R   : public InstructionTest<BNOR_R     > {};
class InstructionTest__BNOR_I   : public InstructionTest<BNOR_I     > {};
class InstructionTest__BNAND_R  : public InstructionTest<BNAND_R    > {};
class InstructionTest__BNAND_I  : public InstructionTest<BNAND_I    > {};
class InstructionTest__BXOR_R   : public InstructionTest<BXOR_R     > {};
class InstructionTest__BXOR_I   : public InstructionTest<BXOR_I     > {};
class InstructionTest__AND_R    : public InstructionTest<AND_R      > {};
class InstructionTest__AND_I    : public InstructionTest<AND_I      > {};
class InstructionTest__OR_R     : public InstructionTest<OR_R       > {};
class InstructionTest__OR_I     : public InstructionTest<OR_I       > {};
class InstructionTest__NOT_R    : public InstructionTest<NOT_R      > {};
class InstructionTest__NOT_I    : public InstructionTest<NOT_I      > {};
class InstructionTest__MOVE_R   : public InstructionTest<MOVE_R     > {};
class InstructionTest__MOVE_I   : public InstructionTest<MOVE_I     > {};
class InstructionTest__OSC_R    : public InstructionTest<OSC_R      > {};
class InstructionTest__OSC_I    : public InstructionTest<OSC_I      > {};

class InstructionTest__LOAD     : public InstructionTest<LOAD       > {};
class InstructionTest__SAVE     : public InstructionTest<SAVE       > {};
class InstructionTest__JUMP     : public InstructionTest<JUMP       > {};
class InstructionTest__JE       : public InstructionTest<JE         > {};
class InstructionTest__JNE      : public InstructionTest<JNE        > {};
class InstructionTest__JL       : public InstructionTest<JL         > {};
class InstructionTest__JLE      : public InstructionTest<JLE        > {};
class InstructionTest__JG       : public InstructionTest<JG         > {};
class InstructionTest__JGE      : public InstructionTest<JGE        > {};
class InstructionTest__RND      : public InstructionTest<RND        > {};

TEST_F(InstructionTest__NOP, assemble) {
    auto to_test = descriptor.assemble({"NOP"});
    //  TODO test exceptions are thrown for invalid inputs etc.
}

TEST_F(InstructionTest__NOP, disassemble) {
    Instruction ti;
    //  TODO set up a few different test instructions, check for exceptions etc.
    auto to_test = descriptor.disassemble(ti);
}

TEST_F(InstructionTest__NOP, execute) {
    Instruction ti;
    //  TODO set up a few different test instructions
    auto a = core;
    descriptor.execute(core, memory, ti);
    auto b = core;

    ASSERT_EQ(a, b);
}
