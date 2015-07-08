#pragma once

#include <cstdint>

//  funct codes for r instructions
enum InstructionsR : uint32_t {
    ADD = 0x20,
    ADDU = 0x21,
    AND = 0x24,
    DIV = 0x1A,
    DIVU = 0x1B,
    JR = 0x08,
    MFHI = 0x10,
    MFLO = 0x12,
    MULT = 0x18,
    MULTU = 0x19,
    NOR = 0x27,
    XOR = 0x26,
    OR = 0x25,
    SLT = 0x2A,
    SLTU = 0x2B,
    SLL = 0x00,
    SRL = 0x02,
    SUB = 0x22,
    SUBU = 0x23,
};

//  op codes for i instructions
enum InstructionsI : uint32_t {
    ADDI = 0x08,
    ADDIU = 0x09,
    ANDI = 0x0C,
    BEQ = 0x04,
    BNE = 0x05,
    LW = 0x23,
    LUI = 0x0F,
    ORI = 0x0D,
    SLTI = 0x0A,
    SLTIU = 0x0B,
    SW = 0x2B,
};

//  op codes for j instructions
enum InstructionsJ : uint32_t {
    J = 0x02,
    JAL = 0x03,
};
