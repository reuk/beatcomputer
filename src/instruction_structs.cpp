#include "instruction_structs.h"

using namespace std;

Instruction__ADD::Instruction__ADD()
    : InstructionRDescriptor("ADD", 0x20) {
}
void Instruction__ADD::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs + rt;
}

Instruction__AND::Instruction__AND()
    : InstructionRDescriptor("AND", 0x24) {
}
void Instruction__AND::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs & rt;
}

Instruction__DIV::Instruction__DIV()
    : InstructionRDescriptor("DIV", 0x1A) {
}
void Instruction__DIV::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    core.lo = rs / rt;
    core.hi = rs % rt;
}

Instruction__JR::Instruction__JR()
    : InstructionRDescriptor("JR", 0x08) {
}
void Instruction__JR::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t & rd, uint32_t shamt) const {
    core.ip = rs;
}

Instruction__MFHI::Instruction__MFHI()
    : InstructionRDescriptor("MFHI", 0x10) {
}
void Instruction__MFHI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t & rd, uint32_t shamt) const {
    rd = core.hi;
}

Instruction__MFLO::Instruction__MFLO()
    : InstructionRDescriptor("MFLO", 0x12) {
}
void Instruction__MFLO::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t & rd, uint32_t shamt) const {
    rd = core.lo;
}

Instruction__MULT::Instruction__MULT()
    : InstructionRDescriptor("MULT", 0x18) {
}
void Instruction__MULT::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t & rd, uint32_t shamt) const {
    uint64_t rs_ = rs;
    uint64_t rt_ = rt;
    core.lo = ((rs_ * rt_) << 32) >> 32;
    core.hi = (rs_ * rt_) >> 32;
}

Instruction__NOR::Instruction__NOR()
    : InstructionRDescriptor("NOR", 0x27) {
}
void Instruction__NOR::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = ~(rs | rt);
}

Instruction__XOR::Instruction__XOR()
    : InstructionRDescriptor("XOR", 0x26) {
}
void Instruction__XOR::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs ^ rt;
}

Instruction__OR::Instruction__OR()
    : InstructionRDescriptor("OR", 0x25) {
}
void Instruction__OR::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t & rd, uint32_t shamt) const {
    rd = rs | rt;
}

Instruction__SLT::Instruction__SLT()
    : InstructionRDescriptor("SLT", 0x2A) {
}
void Instruction__SLT::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs < rt;
}

Instruction__SLL::Instruction__SLL()
    : InstructionRDescriptor("SLL", 0x00) {
}
void Instruction__SLL::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs << shamt;
}

Instruction__SRL::Instruction__SRL()
    : InstructionRDescriptor("SRL", 0x02) {
}
void Instruction__SRL::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs >> shamt;
}

Instruction__SUB::Instruction__SUB()
    : InstructionRDescriptor("SUB", 0x22) {
}
void Instruction__SUB::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs - rt;
}

Instruction__ADDI::Instruction__ADDI()
    : InstructionIDescriptor("ADDI", 0x08) {
}
void Instruction__ADDI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t immediate) const {
    rt = rs + immediate;
}

Instruction__ANDI::Instruction__ANDI()
    : InstructionIDescriptor("ANDI", 0x0C) {
}
void Instruction__ANDI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t immediate) const {
    rt = rs & immediate;
}

Instruction__BEQ::Instruction__BEQ()
    : InstructionIDescriptor("BEQ", 0x04) {
}
void Instruction__BEQ::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    if (rs == rt)
        core.ip = immediate;
}

Instruction__BNE::Instruction__BNE()
    : InstructionIDescriptor("BNE", 0x05) {
}
void Instruction__BNE::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    if (rs != rt)
        core.ip = immediate;
}

Instruction__LW::Instruction__LW()
    : InstructionIDescriptor("LW", 0x23) {
}
void Instruction__LW::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t immediate) const {
    rt = memory[rs + immediate].raw;
}

Instruction__LUI::Instruction__LUI()
    : InstructionIDescriptor("LUI", 0x0F) {
}
void Instruction__LUI::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    rt = immediate << 16;
}

Instruction__ORI::Instruction__ORI()
    : InstructionIDescriptor("ORI", 0x0D) {
}
void Instruction__ORI::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    rt = rs | immediate;
}

Instruction__SLTI::Instruction__SLTI()
    : InstructionIDescriptor("SLTI", 0x0A) {
}
void Instruction__SLTI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t immediate) const {
    rt = rs < immediate;
}

Instruction__SW::Instruction__SW()
    : InstructionIDescriptor("SW", 0x2B) {
}
void Instruction__SW::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t immediate) const {
    memory[rs + immediate].raw = rt;
}

Instruction__J::Instruction__J()
    : InstructionJDescriptor("J", 0x02) {
}
void Instruction__J::execute_specific(Core & core,
                                      std::vector<Instruction> & memory,
                                      uint32_t address) const {
    core.ip = address;
}

Instruction__JAL::Instruction__JAL()
    : InstructionJDescriptor("JAL", 0x03) {
}
void Instruction__JAL::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t address) const {
    core.reg[31] = core.ip + 1;
    core.ip = address;
}
