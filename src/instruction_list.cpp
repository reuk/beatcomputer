#include "instruction_list.h"

#include "instruction_structs.h"

#include <istream>
#include <sstream>

using namespace std;

InstructionList::InstructionList()
    : r_instructions({
            make_shared<    Instruction__ADD    >(),
            make_shared<    Instruction__AND    >(),
            make_shared<    Instruction__DIV    >(),
            make_shared<    Instruction__JR     >(),
            make_shared<    Instruction__MFHI   >(),
            make_shared<    Instruction__MFLO   >(),
            make_shared<    Instruction__MULT   >(),
            make_shared<    Instruction__NOR    >(),
            make_shared<    Instruction__XOR    >(),
            make_shared<    Instruction__OR     >(),
            make_shared<    Instruction__SLT    >(),
            make_shared<    Instruction__SLL    >(),
            make_shared<    Instruction__SRL    >(),
            make_shared<    Instruction__SUB    >(),
        })
    , i_instructions({
            make_shared<    Instruction__ADDI   >(),
            make_shared<    Instruction__ANDI   >(),
            make_shared<    Instruction__BEQ    >(),
            make_shared<    Instruction__BNE    >(),
            make_shared<    Instruction__LW     >(),
            make_shared<    Instruction__LUI    >(),
            make_shared<    Instruction__ORI    >(),
            make_shared<    Instruction__SLTI   >(),
            make_shared<    Instruction__SW     >(),
        })
    , j_instructions({
            make_shared<    Instruction__J      >(),
            make_shared<    Instruction__JAL    >(),
        })
{
    build_assembly_table();
    build_execution_table();
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
    return i->second->parse(tokens);
}

void InstructionList::execute(Core & core, vector<Instruction> & memory, Instruction instr) const {

}

void InstructionList::build_assembly_table() {

}

void InstructionList::build_execution_table() {

}
