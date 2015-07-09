#pragma once

#include "instruction_list.h"

#include <string>
#include <vector>
#include <cstdint>

class Assembler {
public:
    Assembler(InstructionList & instruction_list);
    Instruction assemble(std::string & str) const;
private:
    InstructionList & instruction_list;
};
