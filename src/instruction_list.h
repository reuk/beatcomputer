#pragma once

#include "instruction_descriptor.h"

#include <map>
#include <vector>
#include <memory>

class InstructionList {
public:
    InstructionList();

    Instruction assemble(std::string & str) const;
    void execute(Core & core, std::vector<Instruction> & memory, Instruction instr) const;

private:
    void build_assembly_table();
    void build_execution_tables();

    std::vector<std::shared_ptr<InstructionRDescriptor>> r_instructions;
    std::vector<std::shared_ptr<InstructionIDescriptor>> i_instructions;
    std::vector<std::shared_ptr<InstructionJDescriptor>> j_instructions;

    std::map<std::string, std::shared_ptr<InstructionDescriptor>> assembly_table;

    std::map<uint32_t, std::shared_ptr<InstructionRDescriptor>> execution_r_table;
    std::map<uint32_t, std::shared_ptr<InstructionIDescriptor>> execution_i_table;
    std::map<uint32_t, std::shared_ptr<InstructionJDescriptor>> execution_j_table;
};
