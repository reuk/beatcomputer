#include "instruction_descriptor.h"

//  R instructions

class Instruction__ADD : public InstructionRDescriptor {
public:
    Instruction__ADD();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__AND : public InstructionRDescriptor {
public:
    Instruction__AND();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__DIV : public InstructionRDescriptor {
public:
    Instruction__DIV();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__JR : public InstructionRDescriptor {
public:
    Instruction__JR();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__MFHI : public InstructionRDescriptor {
public:
    Instruction__MFHI();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__MFLO : public InstructionRDescriptor {
public:
    Instruction__MFLO();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__MULT : public InstructionRDescriptor {
public:
    Instruction__MULT();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__NOR : public InstructionRDescriptor {
public:
    Instruction__NOR();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__XOR : public InstructionRDescriptor {
public:
    Instruction__XOR();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__OR : public InstructionRDescriptor {
public:
    Instruction__OR();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__SLT : public InstructionRDescriptor {
public:
    Instruction__SLT();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__SLL : public InstructionRDescriptor {
public:
    Instruction__SLL();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__SRL : public InstructionRDescriptor {
public:
    Instruction__SRL();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__SUB : public InstructionRDescriptor {
public:
    Instruction__SUB();
    void execute_specific(Core & core, InstructionR inst) const override;
    InstructionR parse_specific(const std::vector<std::string> & str) const override;
};

//  I instructions

class Instruction__ADDI : public InstructionIDescriptor {
public:
    Instruction__ADDI();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__ANDI : public InstructionIDescriptor {
public:
    Instruction__ANDI();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__BEQ : public InstructionIDescriptor {
public:
    Instruction__BEQ();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__BNE : public InstructionIDescriptor {
public:
    Instruction__BNE();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__LW : public InstructionIDescriptor {
public:
    Instruction__LW();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__LUI : public InstructionIDescriptor {
public:
    Instruction__LUI();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__ORI : public InstructionIDescriptor {
public:
    Instruction__ORI();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__SLTI : public InstructionIDescriptor {
public:
    Instruction__SLTI();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__SW : public InstructionIDescriptor {
public:
    Instruction__SW();
    void execute_specific(Core & core, InstructionI inst) const override;
    InstructionI parse_specific(const std::vector<std::string> & str) const override;
};

//  J instructions

class Instruction__J : public InstructionJDescriptor {
public:
    Instruction__J();
    void execute_specific(Core & core, InstructionJ inst) const override;
    InstructionJ parse_specific(const std::vector<std::string> & str) const override;
};

class Instruction__JAL : public InstructionJDescriptor {
public:
    Instruction__JAL();
    void execute_specific(Core & core, InstructionJ inst) const override;
    InstructionJ parse_specific(const std::vector<std::string> & str) const override;
};
