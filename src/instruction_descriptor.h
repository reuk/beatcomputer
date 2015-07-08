#pragma once

#include "instructions.h"

class InstructionDescriptor {
    virtual Instruction parse(const std::vector<std::string> & str) const = 0;
    virtual void execute(Core & core) const = 0;
};
