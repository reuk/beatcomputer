#pragma once

#include "instruction_descriptor.h"

#define R_INSTRUCTION_DEFINITION(Name, Id)                                    \
    struct Name : RInstructionDescriptor<Id> {                                \
        void execute_specific(Core & core, std::vector<Instruction> & memory, \
                              int32_t & rs, int32_t & rt, int32_t & rd,       \
                              uint32_t shamt) const override;                 \
        std::string get_tooltip() const override;                             \
    };

#define I_INSTRUCTION_DEFINITION(Name, Id)                                    \
    struct Name : IInstructionDescriptor<Id> {                                \
        void execute_specific(Core & core, std::vector<Instruction> & memory, \
                              int32_t & rs, int32_t & rt,                     \
                              int32_t immediate) const override;              \
        std::string get_tooltip() const override;                             \
    };

#define R_INSTRUCTION_DEFINITION_SPECIAL(Name, Id)                            \
    struct Name : RInstructionDescriptor<Id> {                                \
        void execute_specific(Core & core, std::vector<Instruction> & memory, \
                              int32_t & rs, int32_t & rt, int32_t & rd,       \
                              uint32_t shamt) const override;                 \
        InstructionR assemble_specific(                                       \
            const std::vector<std::string> & str) const override;             \
        std::string disassemble_specific(InstructionR inst) const override;   \
        std::string get_tooltip() const override;                             \
    };

#define I_INSTRUCTION_DEFINITION_SPECIAL(Name, Id)                            \
    struct Name : IInstructionDescriptor<Id> {                                \
        void execute_specific(Core & core, std::vector<Instruction> & memory, \
                              int32_t & rs, int32_t & rt,                     \
                              int32_t immediate) const override;              \
        InstructionI assemble_specific(                                       \
            const std::vector<std::string> & str) const override;             \
        std::string disassemble_specific(InstructionI inst) const override;   \
        std::string get_tooltip() const override;                             \
    };

#define BINARY_INSTRUCTION_DEFINITION(Name, Id) \
    R_INSTRUCTION_DEFINITION(Name##_R, Id)      \
    I_INSTRUCTION_DEFINITION(Name##_I, Id)

#define BINARY_INSTRUCTION_DEFINITION_SPECIAL(Name, Id) \
    R_INSTRUCTION_DEFINITION_SPECIAL(Name##_R, Id)      \
    I_INSTRUCTION_DEFINITION_SPECIAL(Name##_I, Id)

struct NOP : RInstructionDescriptor<0x00> {
    InstructionR assemble_specific(
        const std::vector<std::string> & str) const override;
    std::string disassemble_specific(InstructionR inst) const override;
    std::string get_tooltip() const override;
};

BINARY_INSTRUCTION_DEFINITION(ADD, 0x01);
BINARY_INSTRUCTION_DEFINITION(SUB, 0x02);
BINARY_INSTRUCTION_DEFINITION(MUL, 0x03);
BINARY_INSTRUCTION_DEFINITION(DIV, 0x04);
BINARY_INSTRUCTION_DEFINITION(BAND, 0x05);
BINARY_INSTRUCTION_DEFINITION(BOR, 0x06);
BINARY_INSTRUCTION_DEFINITION_SPECIAL(BNOT, 0x07);
BINARY_INSTRUCTION_DEFINITION(BXOR, 0x0A);
BINARY_INSTRUCTION_DEFINITION(AND, 0x0B);
BINARY_INSTRUCTION_DEFINITION(OR, 0x0C);
BINARY_INSTRUCTION_DEFINITION_SPECIAL(NOT, 0x0D);
BINARY_INSTRUCTION_DEFINITION_SPECIAL(MOVE, 0x0E);

BINARY_INSTRUCTION_DEFINITION_SPECIAL(JUMP, 0x12);

I_INSTRUCTION_DEFINITION(LOAD, 0x10);
I_INSTRUCTION_DEFINITION(SAVE, 0x11);
I_INSTRUCTION_DEFINITION(JE, 0x13);
I_INSTRUCTION_DEFINITION(JNE, 0x14);
I_INSTRUCTION_DEFINITION(JL, 0x15);
I_INSTRUCTION_DEFINITION(JLE, 0x16);
I_INSTRUCTION_DEFINITION(JG, 0x17);
I_INSTRUCTION_DEFINITION(JGE, 0x18);

R_INSTRUCTION_DEFINITION_SPECIAL(RND, 0x19);

struct OSC_R : RInstructionDescriptor<0x0F> {
    OSC_R(int osc_out_port, const std::string & osc_out_prefix,
          const std::string & osc_out_address);
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          int32_t & rs, int32_t & rt, int32_t & rd,
                          uint32_t shamt) const override;
    std::string get_tooltip() const override;

private:
    const int osc_out_port;
    const std::string osc_out_prefix;
    const std::string osc_out_address;
};

struct OSC_I : IInstructionDescriptor<0x0F> {
    OSC_I(int osc_out_port, const std::string & osc_out_prefix,
          const std::string & osc_out_address);
    void execute_specific(Core & core, std::vector<Instruction> & memory,
                          int32_t & rs, int32_t & rt,
                          int32_t immediate) const override;
    std::string get_tooltip() const override;

private:
    const int osc_out_port;
    const std::string osc_out_prefix;
    const std::string osc_out_address;
};
