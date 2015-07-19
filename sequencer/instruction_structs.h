#include "instruction_descriptor.h"

using B__Instruction__ADD   = RInstructionDescriptor<0x20>;
using B__Instruction__AND   = RInstructionDescriptor<0x24>;
using B__Instruction__DIV   = RInstructionDescriptor<0x1A>;
using B__Instruction__JR    = RInstructionDescriptor<0x08>;
using B__Instruction__MFHI  = RInstructionDescriptor<0x10>;
using B__Instruction__MFLO  = RInstructionDescriptor<0x12>;
using B__Instruction__MULT  = RInstructionDescriptor<0x18>;
using B__Instruction__NOR   = RInstructionDescriptor<0x27>;
using B__Instruction__XOR   = RInstructionDescriptor<0x26>;
using B__Instruction__OR    = RInstructionDescriptor<0x25>;
using B__Instruction__SLT   = RInstructionDescriptor<0x2A>;
using B__Instruction__SLL   = RInstructionDescriptor<0x00>;
using B__Instruction__SRL   = RInstructionDescriptor<0x02>;
using B__Instruction__SUB   = RInstructionDescriptor<0x22>;
using B__Instruction__ADDI  = IInstructionDescriptor<0x28>;
using B__Instruction__ANDI  = IInstructionDescriptor<0x0C>;
using B__Instruction__BEQ   = IInstructionDescriptor<0x04>;
using B__Instruction__BNE   = IInstructionDescriptor<0x05>;
using B__Instruction__LW    = IInstructionDescriptor<0x23>;
using B__Instruction__LUI   = IInstructionDescriptor<0x0F>;
using B__Instruction__ORI   = IInstructionDescriptor<0x0D>;
using B__Instruction__SLTI  = IInstructionDescriptor<0x0A>;
using B__Instruction__SW    = IInstructionDescriptor<0x2B>;
using B__Instruction__J     = JInstructionDescriptor<0x02>;
using B__Instruction__JAL   = JInstructionDescriptor<0x03>;

class Instruction__ADD : public B__Instruction__ADD {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__AND : public B__Instruction__AND {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__DIV : public B__Instruction__DIV {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__JR : public B__Instruction__JR {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__MFHI : public B__Instruction__MFHI {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__MFLO : public B__Instruction__MFLO {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__MULT : public B__Instruction__MULT {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__NOR : public B__Instruction__NOR {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__XOR : public B__Instruction__XOR {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__OR : public B__Instruction__OR {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__SLT : public B__Instruction__SLT {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__SLL : public B__Instruction__SLL {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__SRL : public B__Instruction__SRL {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__SUB : public B__Instruction__SUB {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt, uint32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;
};

class Instruction__ADDI : public B__Instruction__ADDI {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__ANDI : public B__Instruction__ANDI {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__BEQ : public B__Instruction__BEQ {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__BNE : public B__Instruction__BNE {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__LW : public B__Instruction__LW {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__LUI : public B__Instruction__LUI {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__ORI : public B__Instruction__ORI {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__SLTI : public B__Instruction__SLTI {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__SW : public B__Instruction__SW {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t & rs, uint32_t & rt,
                          uint32_t immediate) const override;
    std::string get_tooltip() const override;
};

class Instruction__J : public B__Instruction__J {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t address) const override;
    std::string get_tooltip() const override;
};

class Instruction__JAL : public B__Instruction__JAL {
public:
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          uint32_t address) const override;
    std::string get_tooltip() const override;
};
