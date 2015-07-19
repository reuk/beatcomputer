#pragma once

#include "instruction_descriptor.h"

#include <vector>
#include <memory>

class InstructionManager {
public:
    InstructionManager();

    std::vector<std::shared_ptr<InstructionDescriptor>> fu_instructions;
    std::vector<std::shared_ptr<InstructionDescriptor>> op_instructions;
};
