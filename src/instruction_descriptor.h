#pragma once

#include "instructions.h"
#include "core.h"
#include "trim.h"
#include "parse_functions.h"

#include <string>
#include <vector>
#include <sstream>

class InstructionDescriptor {
public:
    InstructionDescriptor(const std::string & str, uint32_t id);

    virtual Instruction assemble(const std::vector<std::string> & str) const = 0;
    virtual std::string disassemble(Instruction instr) const = 0;
    virtual void execute(Core & core, std::vector<Instruction> & memory,
                         Instruction instr) const = 0;

    std::string get_string() const;
    uint32_t get_id_code() const;

    void set_string(const std::string & str);
    void set_id_code(uint32_t id);

private:
    std::string str;
    uint32_t id;
};

template <typename T>
class SpecificInstructionDescriptor : public InstructionDescriptor {
public:
    SpecificInstructionDescriptor(const std::string & str, uint32_t id)
        : InstructionDescriptor(str, id) {
    }
};

template <>
class SpecificInstructionDescriptor<InstructionR>
    : public InstructionDescriptor {
public:
    SpecificInstructionDescriptor(const std::string & str, uint32_t id)
        : InstructionDescriptor(str, id) {
    }

    Instruction assemble(const std::vector<std::string> & str) const override {
        return assemble_specific(str);
    }

    std::string disassemble(Instruction instr) const override {
        return disassemble_specific(instr);
    }

    void execute(Core & core, std::vector<Instruction> & memory,
                 Instruction instr) const override {
        uint32_t & rs = core.reg[instr.r.rs];
        uint32_t & rt = core.reg[instr.r.rt];
        uint32_t & rd = core.reg[instr.r.rd];
        execute_specific(core, memory, rs, rt, rd, instr.r.shamt);
    }

    virtual void execute_specific(Core & core,
                                  std::vector<Instruction> & memory,
                                  uint32_t & rs, uint32_t & rt, uint32_t & rd,
                                  uint32_t shamt) const = 0;

    virtual InstructionR assemble_specific(
        const std::vector<std::string> & str) const {
        InstructionR r;
        r.op = 0x0;
        r.funct = get_id_code();
        r.rd = parse_register(str[1]);
        r.rs = parse_register(str[2]);
        r.rt = parse_register(str[3]);
        r.shamt = 0x0;
        return r;
    }

    virtual std::string disassemble_specific(InstructionR instr) const {
        std::stringstream ss;

        ss << get_string()
           << " R" << instr.rd
           << " R" << instr.rs
           << " R" << instr.rt
           << std::endl;

        return ss.str();
    }
};

template <>
class SpecificInstructionDescriptor<InstructionI>
    : public InstructionDescriptor {
public:
    SpecificInstructionDescriptor(const std::string & str, uint32_t id)
        : InstructionDescriptor(str, id) {
    }

    Instruction assemble(const std::vector<std::string> & str) const override {
        return assemble_specific(str);
    }

    std::string disassemble(Instruction instr) const override {
        return disassemble_specific(instr);
    }

    void execute(Core & core, std::vector<Instruction> & memory,
                 Instruction instr) const override {
        uint32_t & rs = core.reg[instr.i.rs];
        uint32_t & rt = core.reg[instr.i.rt];
        execute_specific(core, memory, rs, rt, instr.i.immediate);
    }

    virtual void execute_specific(Core & core,
                                  std::vector<Instruction> & memory,
                                  uint32_t & rs, uint32_t & rt,
                                  uint32_t immediate) const = 0;

    virtual InstructionI assemble_specific(
        const std::vector<std::string> & str) const {
        InstructionI r;
        r.op = get_id_code();
        r.rt = parse_register(str[1]);
        r.rs = parse_register(str[2]);
        r.immediate = parse_immediate(str[3]);
        return r;
    }

    virtual std::string disassemble_specific(InstructionI instr) const {
        std::stringstream ss;

        ss << get_string()
           << " R" << instr.rt
           << " R" << instr.rs
           << " " << instr.immediate
           << std::endl;

        return ss.str();
    }
};

template <>
class SpecificInstructionDescriptor<InstructionJ>
    : public InstructionDescriptor {
public:
    SpecificInstructionDescriptor(const std::string & str, uint32_t id)
        : InstructionDescriptor(str, id) {
    }

    Instruction assemble(const std::vector<std::string> & str) const override {
        return assemble_specific(str);
    }

    std::string disassemble(Instruction instr) const override {
        return disassemble_specific(instr);
    }

    void execute(Core & core, std::vector<Instruction> & memory,
                 Instruction instr) const override {
        execute_specific(core, memory, instr.j.address);
    }

    virtual void execute_specific(Core & core,
                                  std::vector<Instruction> & memory,
                                  uint32_t address) const = 0;

    virtual InstructionJ assemble_specific(
        const std::vector<std::string> & str) const {
        InstructionJ r;
        r.op = get_id_code();
        r.address = parse_address(str[1]);
        return r;
    }

    virtual std::string disassemble_specific(InstructionJ instr) const {
        std::stringstream ss;

        ss << get_string()
           << " " << instr.address
           << std::endl;

        return ss.str();
    }
};

using InstructionRDescriptor = SpecificInstructionDescriptor<InstructionR>;
using InstructionIDescriptor = SpecificInstructionDescriptor<InstructionI>;
using InstructionJDescriptor = SpecificInstructionDescriptor<InstructionJ>;
