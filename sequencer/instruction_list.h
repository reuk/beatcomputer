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
    std::string disassemble(Instruction instr) const;
    std::string tooltip(Instruction instr) const;
    void execute(Core & core, std::vector<Instruction> & memory) const;

    OpType get_op_type(Instruction instr) const;

private:
    void build_assembly_table(const InstructionManager & im);
    void build_execution_tables(const InstructionManager & im);

    std::shared_ptr<InstructionDescriptor> descriptor_for_instruction(
        Instruction r) const;

    std::map<std::string, std::shared_ptr<InstructionDescriptor>>
        assembly_table;

    std::map<uint32_t, std::shared_ptr<InstructionDescriptor>>
        execution_op_table;
    std::map<uint32_t, std::shared_ptr<InstructionDescriptor>>
        execution_fu_table;
};
