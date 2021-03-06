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
    virtual Instruction assemble(
        const std::vector<std::string> & str) const = 0;
    virtual std::string disassemble(Instruction instr) const = 0;
    virtual void execute(Core & core,
                         std::vector<Instruction> & memory,
                         Instruction instr) const = 0;

    virtual std::string get_string() const = 0;
    virtual uint32_t get_id_code() const = 0;
    virtual OpType get_op_type() const = 0;

    virtual std::string get_tooltip() const {
        return "";
    }
};

template <typename T, int id>
class SpecificInstructionDescriptor : public InstructionDescriptor {
public:
    uint32_t get_id_code() const override {
        return id;
    }
};

template <int id>
class RInstructionDescriptor
    : public SpecificInstructionDescriptor<InstructionR, id> {
public:
    Instruction assemble(const std::vector<std::string> & str) const override {
        return assemble_specific(str);
    }

    std::string disassemble(Instruction instr) const override {
        return disassemble_specific(instr);
    }

    void execute(Core & core,
                 std::vector<Instruction> & memory,
                 Instruction instr) const override {
        int32_t & rs = core.reg[instr.r.rs];
        int32_t & rt = core.reg[instr.r.rt];
        int32_t & rd = core.reg[instr.r.rd];
        execute_specific(core, memory, rs, rt, rd, instr.r.shamt);
    }

    virtual void execute_specific(Core & core,
                                  std::vector<Instruction> & memory,
                                  int32_t & rs,
                                  int32_t & rt,
                                  int32_t & rd,
                                  uint32_t shamt) const {};

    virtual InstructionR assemble_specific(
        const std::vector<std::string> & str) const {
        if (str.size() != 3) {
            throw std::runtime_error("instruction takes three arguments");
        }
        InstructionR r;
        r.op = 0x0;
        r.funct =
            SpecificInstructionDescriptor<InstructionR, id>::get_id_code();
        r.rd = parse_register(str[0]);
        r.rs = parse_register(str[1]);
        r.rt = parse_register(str[2]);
        r.shamt = 0x0;
        return r;
    }

    virtual std::string disassemble_specific(InstructionR instr) const {
        std::stringstream ss;
        ss << "R" << instr.rd << " R" << instr.rs << " R" << instr.rt;
        return ss.str();
    }

    OpType get_op_type() const override {
        return OpType::R;
    }
    std::string get_string() const override {
        return str;
    }

private:
    static const std::string str;
};

template <int id>
class IInstructionDescriptor
    : public SpecificInstructionDescriptor<InstructionI, id> {
public:
    Instruction assemble(const std::vector<std::string> & str) const override {
        return assemble_specific(str);
    }

    std::string disassemble(Instruction instr) const override {
        return disassemble_specific(instr);
    }

    void execute(Core & core,
                 std::vector<Instruction> & memory,
                 Instruction instr) const override {
        int32_t & rs = core.reg[instr.i.rs];
        int32_t & rt = core.reg[instr.i.rt];
        execute_specific(core, memory, rs, rt, instr.i.immediate);
    }

    virtual void execute_specific(Core & core,
                                  std::vector<Instruction> & memory,
                                  int32_t & rs,
                                  int32_t & rt,
                                  int32_t immediate) const = 0;

    virtual InstructionI assemble_specific(
        const std::vector<std::string> & str) const {
        if (str.size() != 3) {
            throw std::runtime_error("instruction takes three arguments");
        }
        InstructionI r;
        r.op = SpecificInstructionDescriptor<InstructionI, id>::get_id_code();
        r.rt = parse_register(str[0]);
        r.rs = parse_register(str[1]);
        r.immediate = parse_immediate(str[2]);
        return r;
    }

    virtual std::string disassemble_specific(InstructionI instr) const {
        std::stringstream ss;
        ss << "R" << instr.rt << " R" << instr.rs << " " << instr.immediate;
        return ss.str();
    }

    OpType get_op_type() const override {
        return OpType::I;
    }
    std::string get_string() const override {
        return str;
    }

private:
    static const std::string str;
};

template <int id>
class JInstructionDescriptor
    : public SpecificInstructionDescriptor<InstructionJ, id> {
public:
    Instruction assemble(const std::vector<std::string> & str) const override {
        return assemble_specific(str);
    }

    std::string disassemble(Instruction instr) const override {
        return disassemble_specific(instr);
    }

    void execute(Core & core,
                 std::vector<Instruction> & memory,
                 Instruction instr) const override {
        execute_specific(core, memory, instr.j.address);
    }

    virtual void execute_specific(Core & core,
                                  std::vector<Instruction> & memory,
                                  int32_t address) const = 0;

    virtual InstructionJ assemble_specific(
        const std::vector<std::string> & str) const {
        if (str.size() != 1) {
            throw std::runtime_error("instruction takes one argument");
        }
        InstructionJ r;
        r.op = SpecificInstructionDescriptor<InstructionJ, id>::get_id_code();
        r.address = parse_address(str[0]);
        return r;
    }

    virtual std::string disassemble_specific(InstructionJ instr) const {
        std::stringstream ss;
        ss << instr.address;
        return ss.str();
    }

    OpType get_op_type() const override {
        return OpType::J;
    }
    std::string get_string() const override {
        return str;
    }

private:
    static const std::string str;
};
