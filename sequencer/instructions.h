#pragma once

#include <cstdint>

struct InstructionR {
    uint32_t op : 6;
    uint32_t rs : 5;
    uint32_t rt : 5;
    uint32_t rd : 5;
    uint32_t shamt : 5;
    uint32_t funct : 6;
};

struct InstructionI {
    uint32_t op : 6;
    uint32_t rs : 5;
    uint32_t rt : 5;
    uint32_t immediate : 16;
};

struct InstructionJ {
    uint32_t op : 6;
    uint32_t address : 26;
};

struct Instruction {
    Instruction(uint32_t raw = 0);
    Instruction(InstructionR instr);
    Instruction(InstructionI instr);
    Instruction(InstructionJ instr);

    union {
        InstructionR r;
        InstructionI i;
        InstructionJ j;
        uint32_t raw;
    };

    operator InstructionR() const;
    operator InstructionI() const;
    operator InstructionJ() const;
};

enum class OpType { R, I, J };
