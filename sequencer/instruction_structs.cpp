#include "instruction_structs.h"

using namespace std;

void Instruction__ADD::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs + rt;
}
string Instruction__ADD::get_tooltip() const {
    return "ADD a b c -> a = b + c";
}

template <>
const string B__Instruction__ADD::str = "ADD";

void Instruction__AND::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs & rt;
}
string Instruction__AND::get_tooltip() const {
    return "AND a b c -> a = b & c";
}
template <>
const string B__Instruction__AND::str = "AND";

void Instruction__DIV::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    core.lo = rs / rt;
    core.hi = rs % rt;
}
string Instruction__DIV::get_tooltip() const {
    return "DIV a b -> LO = a / b, HI = a % b";
}
template <>
const string B__Instruction__DIV::str = "DIV";

void Instruction__JR::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t & rd, uint32_t shamt) const {
    core.ip = rs;
}
string Instruction__JR::get_tooltip() const {
    return "JR a -> IP = a";
}
template <>
const string B__Instruction__JR::str = "JR";

void Instruction__MFHI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t & rd, uint32_t shamt) const {
    rd = core.hi;
}
string Instruction__MFHI::get_tooltip() const {
    return "MFHI a -> a = HI";
}
template <>
const string B__Instruction__MFHI::str = "MFHI";

void Instruction__MFLO::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t & rd, uint32_t shamt) const {
    rd = core.lo;
}
string Instruction__MFLO::get_tooltip() const {
    return "MFLO a -> a = LO";
}
template <>
const string B__Instruction__MFLO::str = "MFLO";

void Instruction__MULT::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t & rd, uint32_t shamt) const {
    uint64_t rs_ = rs;
    uint64_t rt_ = rt;
    core.lo = ((rs_ * rt_) << 32) >> 32;
    core.hi = (rs_ * rt_) >> 32;
}
string Instruction__MULT::get_tooltip() const {
    return "MULT a b -> LO = ((a * b) << 32) >> 32, HI = (a * b) >> 32";
}
template <>
const string B__Instruction__MULT::str = "MULT";

void Instruction__NOR::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = ~(rs | rt);
}
string Instruction__NOR::get_tooltip() const {
    return "NOR a b c -> a = ~(b | c)";
}
template <>
const string B__Instruction__NOR::str = "NOR";

void Instruction__XOR::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs ^ rt;
}
string Instruction__XOR::get_tooltip() const {
    return "XOR a b c -> a = b ^ c";
}
template <>
const string B__Instruction__XOR::str = "XOR";

void Instruction__OR::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t & rd, uint32_t shamt) const {
    rd = rs | rt;
}
string Instruction__OR::get_tooltip() const {
    return "OR a b c -> a = b | c";
}
template <>
const string B__Instruction__OR::str = "OR";

void Instruction__SLT::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs < rt;
}
string Instruction__SLT::get_tooltip() const {
    return "SLT a b c -> a = b < c";
}
template <>
const string B__Instruction__SLT::str = "SLT";

void Instruction__SLL::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs << shamt;
}
string Instruction__SLL::get_tooltip() const {
    return "SLL a b c -> a = b << c";
}
template <>
const string B__Instruction__SLL::str = "SLL";

void Instruction__SRL::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs >> shamt;
}
string Instruction__SRL::get_tooltip() const {
    return "SRL a b c -> a = b >> c";
}
template <>
const string B__Instruction__SRL::str = "SRL";

void Instruction__SUB::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t & rd, uint32_t shamt) const {
    rd = rs - rt;
}
string Instruction__SUB::get_tooltip() const {
    return "SUB a b c -> a = b - c";
}
template <>
const string B__Instruction__SUB::str = "SUB";

void Instruction__ADDI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t immediate) const {
    rt = rs + immediate;
}
string Instruction__ADDI::get_tooltip() const {
    return "ADDI a b c -> a = b + c";
}
template <>
const string B__Instruction__ADDI::str = "ADDI";

void Instruction__ANDI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t immediate) const {
    rt = rs & immediate;
}
string Instruction__ANDI::get_tooltip() const {
    return "ANDI a b c -> a = b & c";
}
template <>
const string B__Instruction__ANDI::str = "ANDI";

void Instruction__BEQ::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    if (rs == rt)
        core.ip = immediate;
}
string Instruction__BEQ::get_tooltip() const {
    return "BEQ a b c -> if (a == b) then IP = c";
}
template <>
const string B__Instruction__BEQ::str = "BEQ";

void Instruction__BNE::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    if (rs != rt)
        core.ip = immediate;
}
string Instruction__BNE::get_tooltip() const {
    return "BNE a b c -> if (a != b) then IP = c";
}
template <>
const string B__Instruction__BNE::str = "BNE";

void Instruction__LW::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t immediate) const {
    rt = memory[rs + immediate].raw;
}
string Instruction__LW::get_tooltip() const {
    return "LW a b c -> a = memory[b + c]";
}
template <>
const string B__Instruction__LW::str = "LW";

void Instruction__LUI::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    rt = immediate << 16;
}
string Instruction__LUI::get_tooltip() const {
    return "LUI a b -> a = b << 16";
}
template <>
const string B__Instruction__LUI::str = "LUI";

void Instruction__ORI::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t & rs, uint32_t & rt,
                                        uint32_t immediate) const {
    rt = rs | immediate;
}
string Instruction__ORI::get_tooltip() const {
    return "ORI a b c -> a = b | c";
}
template <>
const string B__Instruction__ORI::str = "ORI";

void Instruction__SLTI::execute_specific(Core & core,
                                         std::vector<Instruction> & memory,
                                         uint32_t & rs, uint32_t & rt,
                                         uint32_t immediate) const {
    rt = rs < immediate;
}
string Instruction__SLTI::get_tooltip() const {
    return "SLTI a b c -> a = b < c";
}
template <>
const string B__Instruction__SLTI::str = "SLTI";

void Instruction__SW::execute_specific(Core & core,
                                       std::vector<Instruction> & memory,
                                       uint32_t & rs, uint32_t & rt,
                                       uint32_t immediate) const {
    memory[rs + immediate].raw = rt;
}
string Instruction__SW::get_tooltip() const {
    return "SW a b c -> memory[b + c] = a";
}
template <>
const string B__Instruction__SW::str = "SW";

void Instruction__J::execute_specific(Core & core,
                                      std::vector<Instruction> & memory,
                                      uint32_t address) const {
    core.ip = address;
}
string Instruction__J::get_tooltip() const {
    return "J a -> IP = a";
}
template <>
const string B__Instruction__J::str = "J";

void Instruction__JAL::execute_specific(Core & core,
                                        std::vector<Instruction> & memory,
                                        uint32_t address) const {
    core.reg[31] = core.ip + 1;
    core.ip = address;
}
string Instruction__JAL::get_tooltip() const {
    return "JAL a -> SP = IP + 1, IP = a";
}
template <>
const string B__Instruction__JAL::str = "JAL";
