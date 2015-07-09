#pragma once

#include "instruction_descriptor.h"

#include <vector>
#include <memory>

class InstructionManager {
public:
    InstructionManager();

    std::vector<std::shared_ptr<InstructionRDescriptor>> r_instructions;
    std::vector<std::shared_ptr<InstructionIDescriptor>> i_instructions;
    std::vector<std::shared_ptr<InstructionJDescriptor>> j_instructions;
};
