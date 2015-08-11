#pragma once

#include "instruction_descriptor.h"

#include <vector>
#include <memory>

class InstructionManager {
public:
    InstructionManager(int osc_out_port,
                       const std::string & osc_out_prefix,
                       const std::string & osc_out_address);

    std::vector<std::shared_ptr<InstructionDescriptor>> fu_instructions;
    std::vector<std::shared_ptr<InstructionDescriptor>> op_instructions;
};
