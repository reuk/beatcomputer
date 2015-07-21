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
    std::map<std::string, std::vector<std::shared_ptr<InstructionDescriptor>>>
        build_assembly_table(const InstructionManager & im) const;
    std::map<uint32_t, std::shared_ptr<InstructionDescriptor>>
        build_op_table(const InstructionManager & im) const;
    std::map<uint32_t, std::shared_ptr<InstructionDescriptor>>
        build_fu_table(const InstructionManager & im) const;

    std::shared_ptr<InstructionDescriptor> descriptor_for_instruction(
        Instruction r) const;

    const
    std::map<std::string, std::vector<std::shared_ptr<InstructionDescriptor>>>
        assembly_table;

    const
    std::map<uint32_t, std::shared_ptr<InstructionDescriptor>>
        execution_op_table;
    const
    std::map<uint32_t, std::shared_ptr<InstructionDescriptor>>
        execution_fu_table;
};
