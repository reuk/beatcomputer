#pragma once

#include "instruction_manager.h"

#include <map>
#include <vector>
#include <memory>

#include <iostream>

class InstructionList {
public:
    InstructionList(const InstructionManager & im = InstructionManager());

    Instruction assemble(std::string & str) const;
    void execute(Core & core, std::vector<Instruction> & memory) const;

private:
    void build_assembly_table(const InstructionManager & im);
    void build_execution_tables(const InstructionManager & im);

    std::map<std::string, std::shared_ptr<InstructionDescriptor>>
        assembly_table;

    std::map<uint32_t, std::shared_ptr<InstructionRDescriptor>>
        execution_r_table;
    std::map<uint32_t, std::shared_ptr<InstructionIDescriptor>>
        execution_i_table;
    std::map<uint32_t, std::shared_ptr<InstructionJDescriptor>>
        execution_j_table;
};
