#pragma once

#include <cstdint>

//  funct codes for r instructions
enum InstructionsR : uint32_t {
    ADD = 0x20,
    AND = 0x24,
    DIV = 0x1A,
    JR = 0x08,
    MFHI = 0x10,
    MFLO = 0x12,
    MULT = 0x18,
    NOR = 0x27,
    XOR = 0x26,
    OR = 0x25,
    SLT = 0x2A,
    SLL = 0x00,
    SRL = 0x02,
    SUB = 0x22,
};

//  op codes for i instructions
enum InstructionsI : uint32_t {
    ADDI = 0x08,
    ANDI = 0x0C,
    BEQ = 0x04,
    BNE = 0x05,
    LW = 0x23,
    LUI = 0x0F,
    ORI = 0x0D,
    SLTI = 0x0A,
    SW = 0x2B,
};

//  op codes for j instructions
enum InstructionsJ : uint32_t {
    J = 0x02,
    JAL = 0x03,
};
