#include "core.h"

#include "opcodes.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Core::Core()
    : sp(reg[29])
    , ip(0) {
}

void Core::execute_instruction(Ram & memory) {
    auto instruction = memory[ip];
    auto operation = instruction.r.op;
    auto op_type = get_op_type(operation);

    switch (op_type) {
        case OpType::R:
            execute_r(instruction.r);
            ip += 1;
            break;
        case OpType::I:
            execute_i(instruction.i, memory);
            ip += 1;
            break;
        case OpType::J:
            execute_j(instruction.j);
            break;
    }
}

void Core::execute_r(InstructionR r) {
    uint32_t & _rs = reg[r.rs];
    uint32_t & _rt = reg[r.rt];
    uint32_t & _rd = reg[r.rd];

    switch (r.funct) {
        case ADD:  //  TODO overflow not implemented
        case ADDU:
            _rd = _rs + _rt;
            break;
        case AND:
            _rd = _rs & _rt;
            break;
        case DIV:  //  TODO overflow not implemented
        case DIVU:
            lo = _rs / _rt;
            hi = _rs % _rt;
            break;
        case JR:
            ip = _rs;
            break;
        case MFHI:
            _rd = hi;
            break;
        case MFLO:
            _rd = lo;
            break;
        case MULT:  //  TODO overflow not implemented
        case MULTU: {
            uint64_t _rs_ = _rs;
            uint64_t _rt_ = _rt;
            lo = ((_rs_ * _rt_) << 32) >> 32;
            hi = (_rs_ * _rt_) >> 32;
            break;
        }
        case NOR:
            _rd = ~(_rs | _rt);
            break;
        case XOR:
            _rd = _rs ^ _rt;
            break;
        case OR:
            _rd = _rs | _rt;
            break;
        case SLT:  //  TODO overflow not implemented
        case SLTU:
            _rd = _rs < _rt;
            break;
        case SLL:
            _rd = _rt << r.shamt;
            break;
        case SRL:
            _rd = _rt >> r.shamt;
            break;
        case SUB:  //  TODO overflow not implemented
        case SUBU:
            _rd = _rs - _rt;
            break;
    }
}

void Core::execute_i(InstructionI i, Ram & memory) {
    uint32_t & _rs = reg[i.rs];
    uint32_t & _rt = reg[i.rt];

    switch (i.op) {
        case ADDI:
        case ADDIU:
            _rt = _rs + i.immediate;
            break;
        case ANDI:
            _rt = _rs & i.immediate;
            break;
        case BEQ:
            if (_rs == _rt)
                ip = i.immediate;
            break;
        case BNE:
            if (_rs != _rt)
                ip = i.immediate;
            break;
        case LW:
            _rt = memory[_rs + i.immediate].raw;
            break;
        case LUI:
            _rt = i.immediate << 16;
            break;
        case ORI:
            _rt = _rs | i.immediate;
            break;
        case SLTI:
        case SLTIU:
            _rt = _rs < i.immediate;
            break;
        case SW:
            memory[_rs + i.immediate].raw = _rt;
            break;
    }
}

void Core::execute_j(InstructionJ j) {
    switch (j.op) {
        case J:
            ip = j.address;
            break;
        case JAL:
            reg[31] = ip + 1;
            ip = j.address;
            break;
    }
}

Core::OpType Core::get_op_type(uint32_t operation) {
    switch (operation) {
        case 0x00:
            return OpType::R;
        case 0x02:
        case 0x03:
            return OpType::J;
        default:
            return OpType::I;
    }
}

void Core::print_state() const {
    auto pr32 = [](auto name, auto i) {
        cout << setw(8) << name << ": " << setw(16) << i << " ("
             << bitset<32>(i) << ")" << endl;
    };

    for (auto i = 0; i != reg.size(); ++i) {
        stringstream ss;
        ss << "R" << i;
        pr32(ss.str(), reg[i]);
    }

    pr32("LO", lo);
    pr32("HI", hi);
    pr32("IP", ip);
}
