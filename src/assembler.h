#pragma once

#include "instructions.h"

#include <string>
#include <vector>
#include <cstdint>

class Assembler {
public:
    Instruction assemble(std::string & str) const;

private:
    InstructionR assemble_r(const std::vector<std::string> & str) const;
    InstructionI assemble_i(const std::vector<std::string> & str) const;
    InstructionJ assemble_j(const std::vector<std::string> & str) const;
};
