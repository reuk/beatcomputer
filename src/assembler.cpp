#include "assembler.h"

#include "trim.h"
#include "opcodes.h"
#include "parse_functions.h"

#include <map>
#include <istream>
#include <sstream>
#include <iostream>
#include <bitset>

using namespace std;

Assembler::Assembler(InstructionList & instruction_list)
    : instruction_list(instruction_list) {}

Instruction Assembler::assemble(string & str) const {
    trim(str);
    transform(begin(str), end(str), begin(str),
              [](auto i) { return toupper(i); });
    istringstream to_tokenize(str);
    vector<string> tokens((istream_iterator<string>(to_tokenize)),
                          istream_iterator<string>());

    /*
    auto r = assemble_r(tokens);
    auto i = assemble_i(tokens);
    auto j = assemble_j(tokens);

    Instruction ir;
    ir.r = r;
    Instruction ii;
    ii.i = i;
    Instruction ij;
    ij.j = j;

    uint32_t raw = ir.raw | ii.raw | ij.raw;

    Instruction iraw;
    iraw.raw = raw;
    return iraw;
    */

    return instruction_list.parse(tokens);
}

/*
InstructionR Assembler::assemble_r(const vector<string> & str) const {
    map<string, uint32_t> opcode_strings = {
        {"ADD", ADD},
        {"ADDU", ADDU},
        {"AND", AND},
        {"DIV", DIV},
        {"DIVU", DIVU},
        {"JR", JR},
        {"MFHI", MFHI},
        {"MFLO", MFLO},
        {"MULT", MULT},
        {"MULTU", MULTU},
        {"NOR", NOR},
        {"XOR", XOR},
        {"OR", OR},
        {"SLT", SLT},
        {"SLTU", SLTU},
        {"SLL", SLL},
        {"SRL", SRL},
        {"SUB", SUB},
        {"SUBU", SUBU},
    };

    auto i = opcode_strings.find(str.front());
    if (i != opcode_strings.end()) {
        InstructionR r;
        r.op = 0x0;
        r.funct = i->second;

        //  TODO there's more than one format for r instructions
        //  TODO handle incorrect register parsing
        switch (r.funct) {
            case ADD:
            case ADDU:
            case AND:
            case DIV:
            case DIVU:
            case JR:
            case MFHI:
            case MFLO:
            case MULT:
            case MULTU:
            case NOR:
            case XOR:
            case OR:
            case SLT:
            case SLTU:
            case SLL:
            case SRL:
            case SUB:
            case SUBU:
                r.rd = parse_register(str[1]);
                r.rs = parse_register(str[2]);
                r.rt = parse_register(str[3]);
                break;
        }

        r.shamt = 0x0;

        return r;
    }

    return InstructionR{0};
}

InstructionI Assembler::assemble_i(const vector<string> & str) const {
    map<string, uint32_t> opcode_strings = {
        {"ADDI", ADDI},
        {"ADDIU", ADDIU},
        {"ANDI", ANDI},
        {"BEQ", BEQ},
        {"BNE", BNE},
        {"LW", LW},
        {"LUI", LUI},
        {"ORI", ORI},
        {"SLTI", SLTI},
        {"SLTIU", SLTIU},
        {"SW", SW},
    };

    auto i = opcode_strings.find(str.front());
    if (i != opcode_strings.end()) {
        InstructionI r;
        r.op = i->second;

        switch (r.op) {
            case ADDI:
            case ADDIU:
            case ANDI:
            case BEQ:
            case BNE:
            case LW:
            case LUI:
            case ORI:
            case SLTI:
            case SLTIU:
            case SW:
                r.rt = parse_register(str[1]);
                r.rs = parse_register(str[2]);
                r.immediate = parse_immediate(str[3]);
                break;
        }

        return r;
    }

    return InstructionI{0};
}

InstructionJ Assembler::assemble_j(const vector<string> & str) const {
    map<string, uint32_t> opcode_strings = {
        {"J", J}, {"JAL", JAL},
    };

    auto i = opcode_strings.find(str.front());
    if (i != opcode_strings.end()) {
        InstructionJ r;
        r.op = i->second;
        r.address = parse_address(str[1]);

        return r;
    }

    return InstructionJ{0};
}
*/
