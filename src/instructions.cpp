#include "instructions.h"

Instruction::Instruction()
    : raw(0) {
}
Instruction::Instruction(InstructionR instr)
    : r(instr) {
}
Instruction::Instruction(InstructionI instr)
    : i(instr) {
}
Instruction::Instruction(InstructionJ instr)
    : j(instr) {
}

Instruction::operator InstructionR() const {
    return r;
}
Instruction::operator InstructionI() const {
    return i;
}
Instruction::operator InstructionJ() const {
    return j;
}

OpType get_op_type(uint32_t operation) {
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
