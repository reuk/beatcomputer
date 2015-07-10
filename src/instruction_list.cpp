#include "instruction_list.h"

#include "instruction_structs.h"

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

shared_ptr<InstructionDescriptor> InstructionList::descriptor_for_instruction(Instruction instr) const {
    auto type = get_op_type(instr.r.op);

    switch (type) {
        case OpType::R: {
            auto i = execution_r_table.find(instr.r.funct);
            if (i == execution_r_table.end()) {
                stringstream ss;
                ss << "no such function code: " << instr.r.funct << endl;
                ss << "full instruction: " << instr.raw << " ("
                   << bitset<32>(instr.raw) << ")" << endl;
                throw runtime_error(ss.str());
            }
            return i->second;
        }

        case OpType::I: {
            auto i = execution_i_table.find(instr.i.op);
            if (i == execution_i_table.end()) {
                stringstream ss;
                ss << "no such op code: " << instr.i.op << endl;
                ss << "full instruction: " << instr.raw << " ("
                   << bitset<32>(instr.raw) << ")" << endl;
                throw runtime_error(ss.str());
            }
            return i->second;
        }

        case OpType::J: {
            auto i = execution_j_table.find(instr.j.op);
            if (i == execution_j_table.end()) {
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

void InstructionList::execute(Core & core, vector<Instruction> & memory) const {
    auto instr = memory[core.ip];
    descriptor_for_instruction(instr)->execute(core, memory, instr);
    core.ip += 1;
}

void InstructionList::build_assembly_table(const InstructionManager & im) {
    for (auto i : im.r_instructions)
        assembly_table[i->get_string()] = i;
    for (auto i : im.i_instructions)
        assembly_table[i->get_string()] = i;
    for (auto i : im.j_instructions)
        assembly_table[i->get_string()] = i;
}

void InstructionList::build_execution_tables(const InstructionManager & im) {
    for (auto i : im.r_instructions)
        execution_r_table[i->get_id_code()] = i;
    for (auto i : im.i_instructions)
        execution_i_table[i->get_id_code()] = i;
    for (auto i : im.j_instructions)
        execution_j_table[i->get_id_code()] = i;
}
