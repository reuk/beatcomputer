#include "instruction_set.h"

using namespace std;

#define R_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody)           \
    void Name::execute_specific(Core & core, vector<Instruction> & memory,     \
                                uint32_t & rs, uint32_t & rt, uint32_t & rd,   \
                                uint32_t shamt) const {functionbody}           \
    template<> const std::string RInstructionDescriptor<Id>::str = String;

#define I_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody)           \
    void Name::execute_specific(Core & core, vector<Instruction> & memory,     \
                          uint32_t & rs, uint32_t & rt,                        \
                          uint32_t immediate) const {functionbody}             \
    template<> const std::string IInstructionDescriptor<Id>::str = String;

#define J_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody)           \
    void Name::execute_specific(Core & core, vector<Instruction> & memory,     \
                          uint32_t address) const {functionbody}               \
    template<> const std::string JInstructionDescriptor<Id>::str = String;

R_INSTRUCTION_IMPLEMENTATION(NOP    , "NOP"   ,0x00, {});

R_INSTRUCTION_IMPLEMENTATION(ADD_R  , "ADD"   ,0x01, {});
I_INSTRUCTION_IMPLEMENTATION(ADD_I  , "ADD"   ,0x01, {});
R_INSTRUCTION_IMPLEMENTATION(SUB_R  , "SUB"   ,0x02, {});
I_INSTRUCTION_IMPLEMENTATION(SUB_I  , "SUB"   ,0x02, {});
R_INSTRUCTION_IMPLEMENTATION(MUL_R  , "MUL"   ,0x03, {});
I_INSTRUCTION_IMPLEMENTATION(MUL_I  , "MUL"   ,0x03, {});
R_INSTRUCTION_IMPLEMENTATION(DIV_R  , "DIV"   ,0x04, {});
I_INSTRUCTION_IMPLEMENTATION(DIV_I  , "DIV"   ,0x04, {});
R_INSTRUCTION_IMPLEMENTATION(BAND_R , "BAND"  ,0x05, {});
I_INSTRUCTION_IMPLEMENTATION(BAND_I , "BAND"  ,0x05, {});
R_INSTRUCTION_IMPLEMENTATION(BOR_R  , "BOR"   ,0x06, {});
I_INSTRUCTION_IMPLEMENTATION(BOR_I  , "BOR"   ,0x06, {});
R_INSTRUCTION_IMPLEMENTATION(BNOT_R , "BNOT"  ,0x07, {});
I_INSTRUCTION_IMPLEMENTATION(BNOT_I , "BNOT"  ,0x07, {});
R_INSTRUCTION_IMPLEMENTATION(BNOR_R , "BNOR"  ,0x08, {});
I_INSTRUCTION_IMPLEMENTATION(BNOR_I , "BNOR"  ,0x08, {});
R_INSTRUCTION_IMPLEMENTATION(BNAND_R, "BNAND" ,0x09, {});
I_INSTRUCTION_IMPLEMENTATION(BNAND_I, "BNAND" ,0x09, {});
R_INSTRUCTION_IMPLEMENTATION(BXOR_R , "BXOR"  ,0x0A, {});
I_INSTRUCTION_IMPLEMENTATION(BXOR_I , "BXOR"  ,0x0A, {});
R_INSTRUCTION_IMPLEMENTATION(AND_R  , "AND"   ,0x0B, {});
I_INSTRUCTION_IMPLEMENTATION(AND_I  , "AND"   ,0x0B, {});
R_INSTRUCTION_IMPLEMENTATION(OR_R   , "OR"    ,0x0C, {});
I_INSTRUCTION_IMPLEMENTATION(OR_I   , "OR"    ,0x0C, {});
R_INSTRUCTION_IMPLEMENTATION(NOT_R  , "NOT"   ,0x0D, {});
I_INSTRUCTION_IMPLEMENTATION(NOT_I  , "NOT"   ,0x0D, {});
R_INSTRUCTION_IMPLEMENTATION(MOVE_R , "MOVE"  ,0x0E, {});
I_INSTRUCTION_IMPLEMENTATION(MOVE_I , "MOVE"  ,0x0E, {});
R_INSTRUCTION_IMPLEMENTATION(OSC_R  , "OSC"   ,0x0F, {});
I_INSTRUCTION_IMPLEMENTATION(OSC_I  , "OSC"   ,0x0F, {});

I_INSTRUCTION_IMPLEMENTATION(LOAD   , "LOAD"  ,0x10, {});
I_INSTRUCTION_IMPLEMENTATION(SAVE   , "SAVE"  ,0x11, {});
J_INSTRUCTION_IMPLEMENTATION(JUMP   , "JUMP"  ,0x12, {});
I_INSTRUCTION_IMPLEMENTATION(JE     , "JE"    ,0x13, {});
I_INSTRUCTION_IMPLEMENTATION(JNE    , "JNE"   ,0x14, {});
I_INSTRUCTION_IMPLEMENTATION(JL     , "JL"    ,0x15, {});
I_INSTRUCTION_IMPLEMENTATION(JLE    , "JLE"   ,0x16, {});
I_INSTRUCTION_IMPLEMENTATION(JG     , "JG"    ,0x17, {});
I_INSTRUCTION_IMPLEMENTATION(JGE    , "JGE"   ,0x18, {});
R_INSTRUCTION_IMPLEMENTATION(RND    , "RND"   ,0x18, {});
