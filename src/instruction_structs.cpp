#include "instruction_structs.h"

#include "opcodes.h"

using namespace std;

Instruction__ADD::Instruction__ADD() : InstructionRDescriptor("ADD", ADD) {}
void Instruction__ADD::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs + rt;
}

Instruction__AND::Instruction__AND() : InstructionRDescriptor("AND", AND) {}
void Instruction__AND::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs & rt;
}

Instruction__DIV::Instruction__DIV() : InstructionRDescriptor("DIV", DIV) {}
void Instruction__DIV::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    core.lo = rs / rt;
    core.hi = rs % rt;
}

Instruction__JR::Instruction__JR() : InstructionRDescriptor("JR", JR) {}
void Instruction__JR::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    core.ip = rs;
}

Instruction__MFHI::Instruction__MFHI() : InstructionRDescriptor("MFHI", MFHI) {}
void Instruction__MFHI::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = core.hi;
}

Instruction__MFLO::Instruction__MFLO() : InstructionRDescriptor("MFLO", MFLO) {}
void Instruction__MFLO::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = core.lo;
}

Instruction__MULT::Instruction__MULT() : InstructionRDescriptor("MULT", MULT) {}
void Instruction__MULT::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    uint64_t rs_ = rs;
    uint64_t rt_ = rt;
    core.lo = ((rs_ * rt_) << 32) >> 32;
    core.hi = (rs_ * rt_) >> 32;
}

Instruction__NOR::Instruction__NOR() : InstructionRDescriptor("NOR", NOR) {}
void Instruction__NOR::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = ~(rs | rt);
}

Instruction__XOR::Instruction__XOR() : InstructionRDescriptor("XOR", XOR) {}
void Instruction__XOR::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs ^ rt;
}

Instruction__OR::Instruction__OR() : InstructionRDescriptor("OR", OR) {}
void Instruction__OR::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs | rt;
}

Instruction__SLT::Instruction__SLT() : InstructionRDescriptor("SLT", SLT) {}
void Instruction__SLT::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs < rt;
}

Instruction__SLL::Instruction__SLL() : InstructionRDescriptor("SLL", SLL) {}
void Instruction__SLL::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs << shamt;
}

Instruction__SRL::Instruction__SRL() : InstructionRDescriptor("SRL", SRL) {}
void Instruction__SRL::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs >> shamt;
}

Instruction__SUB::Instruction__SUB() : InstructionRDescriptor("SUB", SUB) {}
void Instruction__SUB::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t & rd, uint32_t shamt) const {
    rd = rs - rt;
}

Instruction__ADDI::Instruction__ADDI() : InstructionIDescriptor("ADDI", ADDI) {}
    void Instruction__ADDI::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    rt = rs + immediate;
}

Instruction__ANDI::Instruction__ANDI() : InstructionIDescriptor("ANDI", ANDI) {}
    void Instruction__ANDI::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    rt = rs & immediate;
}

Instruction__BEQ::Instruction__BEQ() : InstructionIDescriptor("BEQ", BEQ) {}
    void Instruction__BEQ::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    if (rs == rt)
        core.ip = immediate;
}

Instruction__BNE::Instruction__BNE() : InstructionIDescriptor("BNE", BNE) {}
    void Instruction__BNE::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    if (rs != rt)
        core.ip = immediate;
}

Instruction__LW::Instruction__LW() : InstructionIDescriptor("LW", LW) {}
    void Instruction__LW::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    rt = memory[rs + immediate].raw;
}

Instruction__LUI::Instruction__LUI() : InstructionIDescriptor("LUI", LUI) {}
    void Instruction__LUI::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    rt = immediate << 16;
}

Instruction__ORI::Instruction__ORI() : InstructionIDescriptor("ORI", ORI) {}
    void Instruction__ORI::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    rt = rs | immediate;
}

Instruction__SLTI::Instruction__SLTI() : InstructionIDescriptor("SLTI", SLTI) {}
    void Instruction__SLTI::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    rt = rs < immediate;
}

Instruction__SW::Instruction__SW() : InstructionIDescriptor("SW", SW) {}
    void Instruction__SW::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t & rs, uint32_t & rt, uint32_t immediate) const {
    memory[rs + immediate].raw = rt;
}

Instruction__J::Instruction__J() : InstructionJDescriptor("J", J) {}
void Instruction__J::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t address) const {
    core.ip = address;
}

Instruction__JAL::Instruction__JAL() : InstructionJDescriptor("JAL", JAL) {}
void Instruction__JAL::execute_specific(Core & core, std::vector<Instruction> & memory, uint32_t address) const {
    core.reg[31] = core.ip + 1;
    core.ip = address;
}

