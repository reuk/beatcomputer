#include "instruction_set.h"

using namespace std;

#define R_INSTRUCTION_IMPLEMENTATION(Name, String, functionbody)               \
    void Name::execute_specific(Core & core, vector<Instruction> & memory,     \
                                uint32_t & rs, uint32_t & rt, uint32_t & rd,   \
                                uint32_t shamt) const {functionbody}

#define I_INSTRUCTION_IMPLEMENTATION(Name, String, functionbody)               \
    void Name::execute_specific(Core & core, vector<Instruction> & memory,     \
                          uint32_t & rs, uint32_t & rt,                        \
                          uint32_t immediate) const {functionbody}

#define J_INSTRUCTION_IMPLEMENTATION(Name, String, functionbody)               \
    void Name::execute_specific(Core & core, vector<Instruction> & memory,     \
                          uint32_t address) const {functionbody}

R_INSTRUCTION_IMPLEMENTATION(NOP    , "NOP"   , {});

R_INSTRUCTION_IMPLEMENTATION(ADD_R  , "ADD"   , {});
I_INSTRUCTION_IMPLEMENTATION(ADD_I  , "ADD"   , {});
R_INSTRUCTION_IMPLEMENTATION(SUB_R  , "SUB"   , {});
I_INSTRUCTION_IMPLEMENTATION(SUB_I  , "SUB"   , {});
R_INSTRUCTION_IMPLEMENTATION(MUL_R  , "MUL"   , {});
I_INSTRUCTION_IMPLEMENTATION(MUL_I  , "MUL"   , {});
R_INSTRUCTION_IMPLEMENTATION(DIV_R  , "DIV"   , {});
I_INSTRUCTION_IMPLEMENTATION(DIV_I  , "DIV"   , {});
R_INSTRUCTION_IMPLEMENTATION(BAND_R , "BAND"  , {});
I_INSTRUCTION_IMPLEMENTATION(BAND_I , "BAND"  , {});
R_INSTRUCTION_IMPLEMENTATION(BOR_R  , "BOR"   , {});
I_INSTRUCTION_IMPLEMENTATION(BOR_I  , "BOR"   , {});
R_INSTRUCTION_IMPLEMENTATION(BNOT_R , "BNOT"  , {});
I_INSTRUCTION_IMPLEMENTATION(BNOT_I , "BNOT"  , {});
R_INSTRUCTION_IMPLEMENTATION(BNOR_R , "BNOR"  , {});
I_INSTRUCTION_IMPLEMENTATION(BNOR_I , "BNOR"  , {});
R_INSTRUCTION_IMPLEMENTATION(BNAND_R, "BNAND" , {});
I_INSTRUCTION_IMPLEMENTATION(BNAND_I, "BNAND" , {});
R_INSTRUCTION_IMPLEMENTATION(BXOR_R , "BXOR"  , {});
I_INSTRUCTION_IMPLEMENTATION(BXOR_I , "BXOR"  , {});
R_INSTRUCTION_IMPLEMENTATION(AND_R  , "AND"   , {});
I_INSTRUCTION_IMPLEMENTATION(AND_I  , "AND"   , {});
R_INSTRUCTION_IMPLEMENTATION(OR_R   , "OR"    , {});
I_INSTRUCTION_IMPLEMENTATION(OR_I   , "OR"    , {});
R_INSTRUCTION_IMPLEMENTATION(NOT_R  , "NOT"   , {});
I_INSTRUCTION_IMPLEMENTATION(NOT_I  , "NOT"   , {});
R_INSTRUCTION_IMPLEMENTATION(MOVE_R , "MOVE"  , {});
I_INSTRUCTION_IMPLEMENTATION(MOVE_I , "MOVE"  , {});
R_INSTRUCTION_IMPLEMENTATION(OSC_R  , "OSC"   , {});
I_INSTRUCTION_IMPLEMENTATION(OSC_I  , "OSC"   , {});

I_INSTRUCTION_IMPLEMENTATION(LOAD   , "LOAD"  , {});
I_INSTRUCTION_IMPLEMENTATION(SAVE   , "SAVE"  , {});
J_INSTRUCTION_IMPLEMENTATION(JUMP   , "JUMP"  , {});
I_INSTRUCTION_IMPLEMENTATION(JE     , "JE"    , {});
I_INSTRUCTION_IMPLEMENTATION(JNE    , "JNE"   , {});
I_INSTRUCTION_IMPLEMENTATION(JL     , "JL"    , {});
I_INSTRUCTION_IMPLEMENTATION(JLE    , "JLE"   , {});
I_INSTRUCTION_IMPLEMENTATION(JG     , "JG"    , {});
I_INSTRUCTION_IMPLEMENTATION(JGE    , "JGE"   , {});
R_INSTRUCTION_IMPLEMENTATION(RND    , "RND"   , {});
