#include "instructions.h"

Instruction::Instruction(uint32_t raw)
        : raw(raw) {
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
