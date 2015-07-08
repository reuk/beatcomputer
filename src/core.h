#pragma once

#include "instructions.h"

#include <cstdint>
#include <vector>
#include <array>

class Core {
public:
    using Ram = std::vector<Instruction>;

    Core();
    void execute_instruction(Ram & memory);

    void print_state() const;

private:
    enum class OpType { R, I, J };
    OpType get_op_type(uint32_t operation);

    std::array<uint32_t, 32> reg;
    uint32_t lo;
    uint32_t hi;

    uint32_t & sp;  //  stack pointer
    uint32_t ip;    //  instruction pointer

    void execute_r(InstructionR r);
    void execute_i(InstructionI i, Ram & memory);
    void execute_j(InstructionJ j);
};
