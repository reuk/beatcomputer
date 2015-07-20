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

R_INSTRUCTION_DEFINITION(NOP    , "NOP"   , 0x00);

R_INSTRUCTION_DEFINITION(ADD_R  , "ADD"   , 0x01);
I_INSTRUCTION_DEFINITION(ADD_I  , "ADD"   , 0x01);
R_INSTRUCTION_DEFINITION(SUB_R  , "SUB"   , 0x02);
I_INSTRUCTION_DEFINITION(SUB_I  , "SUB"   , 0x02);
R_INSTRUCTION_DEFINITION(MUL_R  , "MUL"   , 0x03);
I_INSTRUCTION_DEFINITION(MUL_I  , "MUL"   , 0x03);
R_INSTRUCTION_DEFINITION(DIV_R  , "DIV"   , 0x04);
I_INSTRUCTION_DEFINITION(DIV_I  , "DIV"   , 0x04);
R_INSTRUCTION_DEFINITION(BAND_R , "BAND"  , 0x05);
I_INSTRUCTION_DEFINITION(BAND_I , "BAND"  , 0x05);
R_INSTRUCTION_DEFINITION(BOR_R  , "BOR"   , 0x06);
I_INSTRUCTION_DEFINITION(BOR_I  , "BOR"   , 0x06);
R_INSTRUCTION_DEFINITION(BNOT_R , "BNOT"  , 0x07);
I_INSTRUCTION_DEFINITION(BNOT_I , "BNOT"  , 0x07);
R_INSTRUCTION_DEFINITION(BNOR_R , "BNOR"  , 0x08);
I_INSTRUCTION_DEFINITION(BNOR_I , "BNOR"  , 0x08);
R_INSTRUCTION_DEFINITION(BNAND_R, "BNAND" , 0x09);
I_INSTRUCTION_DEFINITION(BNAND_I, "BNAND" , 0x09);
R_INSTRUCTION_DEFINITION(BXOR_R , "BXOR"  , 0x0A);
I_INSTRUCTION_DEFINITION(BXOR_I , "BXOR"  , 0x0A);
R_INSTRUCTION_DEFINITION(AND_R  , "AND"   , 0x0B);
I_INSTRUCTION_DEFINITION(AND_I  , "AND"   , 0x0B);
R_INSTRUCTION_DEFINITION(OR_R   , "OR"    , 0x0C);
I_INSTRUCTION_DEFINITION(OR_I   , "OR"    , 0x0C);
R_INSTRUCTION_DEFINITION(NOT_R  , "NOT"   , 0x0D);
I_INSTRUCTION_DEFINITION(NOT_I  , "NOT"   , 0x0D);
R_INSTRUCTION_DEFINITION(MOVE_R , "MOVE"  , 0x0E);
I_INSTRUCTION_DEFINITION(MOVE_I , "MOVE"  , 0x0E);
R_INSTRUCTION_DEFINITION(OSC_R  , "OSC"   , 0x0F);
I_INSTRUCTION_DEFINITION(OSC_I  , "OSC"   , 0x0F);

I_INSTRUCTION_DEFINITION(LOAD   , "LOAD"  , 0x10);
I_INSTRUCTION_DEFINITION(SAVE   , "SAVE"  , 0x11);
J_INSTRUCTION_DEFINITION(JUMP   , "JUMP"  , 0x12);
I_INSTRUCTION_DEFINITION(JE     , "JE"    , 0x13);
I_INSTRUCTION_DEFINITION(JNE    , "JNE"   , 0x14);
I_INSTRUCTION_DEFINITION(JL     , "JL"    , 0x15);
I_INSTRUCTION_DEFINITION(JLE    , "JLE"   , 0x16);
I_INSTRUCTION_DEFINITION(JG     , "JG"    , 0x17);
I_INSTRUCTION_DEFINITION(JGE    , "JGE"   , 0x18);
R_INSTRUCTION_DEFINITION(RND    , "RND"   , 0x18);
