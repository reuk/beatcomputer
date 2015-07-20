#include "instruction_list.h"

#include "instruction_set.h"

#include <istream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <iterator>
#include <bitset>

using namespace std;

InstructionList::InstructionList(const InstructionManager & im) {
    build_assembly_table(im);
    build_execution_tables(im);
}

OpType InstructionList::get_op_type(Instruction instr) const {
    if (instr.r.op == 0x00)
        return OpType::R;

    auto i = execution_op_table.find(instr.r.op);
    if (i != execution_op_table.end())
        return i->second->get_op_type();

    throw runtime_error("no such opcode");
}

Instruction InstructionList::assemble(string & str) const {
    trim(str);
    transform(begin(str), end(str), begin(str),
              [](auto i) { return toupper(i); });
    istringstream to_tokenize(str);
    vector<string> tokens((istream_iterator<string>(to_tokenize)),
                          istream_iterator<string>());

    auto i = assembly_table.find(tokens.front());
    if (i == assembly_table.end()) {
        throw runtime_error("no such instruction");
    }
    return i->second->assemble(tokens);
}

shared_ptr<InstructionDescriptor> InstructionList::descriptor_for_instruction(
    Instruction instr) const {
    auto type = get_op_type(instr.r.op);

    switch (type) {
        case OpType::R: {
            auto i = execution_fu_table.find(instr.r.funct);
            if (i == execution_fu_table.end()) {
                stringstream ss;
                ss << "no such function code: " << instr.r.funct << endl;
                ss << "full instruction: " << instr.raw << " ("
                   << bitset<32>(instr.raw) << ")" << endl;
                throw runtime_error(ss.str());
            }
            return i->second;
        }

        case OpType::I:
        case OpType::J: {
            auto i = execution_op_table.find(instr.j.op);
            if (i == execution_op_table.end()) {
                stringstream ss;
                ss << "no such op code: " << instr.j.op;
                ss << "full instruction: " << instr.raw << " ("
                   << bitset<32>(instr.raw) << ")" << endl;
                throw runtime_error(ss.str());
            }
            return i->second;
        }
    }
}

string InstructionList::disassemble(Instruction instr) const {
    return descriptor_for_instruction(instr)->disassemble(instr);
}

string InstructionList::tooltip(Instruction instr) const {
    return descriptor_for_instruction(instr)->get_tooltip();
}

void InstructionList::execute(Core & core, vector<Instruction> & memory) const {
    auto instr = memory[core.ip];
    descriptor_for_instruction(instr)->execute(core, memory, instr);

    if (get_op_type(instr.r.op) != OpType::J)
        core.ip += 1;
}

void InstructionList::build_assembly_table(const InstructionManager & im) {
    for (auto i : im.op_instructions)
        assembly_table[i->get_string()] = i;
    for (auto i : im.fu_instructions)
        assembly_table[i->get_string()] = i;
}

void InstructionList::build_execution_tables(const InstructionManager & im) {
    for (auto i : im.op_instructions)
        execution_op_table[i->get_id_code()] = i;
    for (auto i : im.fu_instructions)
        execution_fu_table[i->get_id_code()] = i;
}
