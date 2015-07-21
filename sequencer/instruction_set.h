#pragma once

#include "instruction_descriptor.h"

#define R_INSTRUCTION_DEFINITION(Name, String, Id)                             \
    struct Name: RInstructionDescriptor<Id> {                                  \
        void execute_specific(Core & core, std::vector<Instruction> & memory,  \
                              uint32_t & rs, uint32_t & rt, uint32_t & rd,     \
                              uint32_t shamt) const override;                  \
    };

#define I_INSTRUCTION_DEFINITION(Name, String, Id)                             \
    struct Name: IInstructionDescriptor<Id> {                                  \
        void execute_specific(Core & core, std::vector<Instruction> & memory,  \
                              uint32_t & rs, uint32_t & rt,                    \
                              uint32_t immediate) const override;              \
    };

#define J_INSTRUCTION_DEFINITION(Name, String, Id)                             \
    struct Name: JInstructionDescriptor<Id> {                                  \
    void execute_specific(Core & core, std::vector<Instruction> & memory,      \
                          uint32_t address) const override;                    \
    };

#define BINARY_INSTRUCTION_DEFINITION(Name, String, Id)                        \
    R_INSTRUCTION_DEFINITION(Name##_R, String, Id)                             \
    I_INSTRUCTION_DEFINITION(Name##_I, String, Id)

R_INSTRUCTION_DEFINITION        (NOP    , "NOP"   , 0x00);

BINARY_INSTRUCTION_DEFINITION   (ADD    , "ADD"   , 0x01);
BINARY_INSTRUCTION_DEFINITION   (SUB    , "SUB"   , 0x02);
BINARY_INSTRUCTION_DEFINITION   (MUL    , "MUL"   , 0x03);
BINARY_INSTRUCTION_DEFINITION   (DIV    , "DIV"   , 0x04);
BINARY_INSTRUCTION_DEFINITION   (BAND   , "BAND"  , 0x05);
BINARY_INSTRUCTION_DEFINITION   (BOR    , "BOR"   , 0x06);
BINARY_INSTRUCTION_DEFINITION   (BNOT   , "BNOT"  , 0x07);
BINARY_INSTRUCTION_DEFINITION   (BXOR   , "BXOR"  , 0x0A);
BINARY_INSTRUCTION_DEFINITION   (AND    , "AND"   , 0x0B);
BINARY_INSTRUCTION_DEFINITION   (OR     , "OR"    , 0x0C);
BINARY_INSTRUCTION_DEFINITION   (NOT    , "NOT"   , 0x0D);
BINARY_INSTRUCTION_DEFINITION   (MOVE   , "MOVE"  , 0x0E);
BINARY_INSTRUCTION_DEFINITION   (OSC    , "OSC"   , 0x0F);

I_INSTRUCTION_DEFINITION        (LOAD   , "LOAD"  , 0x10);
I_INSTRUCTION_DEFINITION        (SAVE   , "SAVE"  , 0x11);
J_INSTRUCTION_DEFINITION        (JUMP   , "JUMP"  , 0x12);
I_INSTRUCTION_DEFINITION        (JE     , "JE"    , 0x13);
I_INSTRUCTION_DEFINITION        (JNE    , "JNE"   , 0x14);
I_INSTRUCTION_DEFINITION        (JL     , "JL"    , 0x15);
I_INSTRUCTION_DEFINITION        (JLE    , "JLE"   , 0x16);
I_INSTRUCTION_DEFINITION        (JG     , "JG"    , 0x17);
I_INSTRUCTION_DEFINITION        (JGE    , "JGE"   , 0x18);
R_INSTRUCTION_DEFINITION        (RND    , "RND"   , 0x18);
