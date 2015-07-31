#include "instruction_set.h"

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#include <iostream>

using namespace std;

#define R_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody, tooltip) \
    void Name::execute_specific(Core &core, vector<Instruction> &memory,      \
                                int32_t &rs, int32_t &rt, int32_t &rd,        \
                                uint32_t shamt) const {                       \
        functionbody;                                                         \
    }                                                                         \
    string Name::get_tooltip() const {                                        \
        return tooltip;                                                       \
    }                                                                         \
    template <>                                                               \
    const string RInstructionDescriptor<Id>::str = String;

#define I_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody, tooltip) \
    void Name::execute_specific(Core &core, vector<Instruction> &memory,      \
                                int32_t &rs, int32_t &rt, int32_t im) const { \
        functionbody;                                                         \
    }                                                                         \
    string Name::get_tooltip() const {                                        \
        return tooltip;                                                       \
    }                                                                         \
    template <>                                                               \
    const string IInstructionDescriptor<Id>::str = String;

#define J_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody, tooltip) \
    void Name::execute_specific(Core &core, vector<Instruction> &memory,      \
                                int32_t address) const {                      \
        functionbody;                                                         \
    }                                                                         \
    string Name::get_tooltip() const {                                        \
        return tooltip;                                                       \
    }                                                                         \
    template <>                                                               \
    const string JInstructionDescriptor<Id>::str = String;

#define BINARY_INSTRUCTION_IMPLEMENTATION(Name, Id, Op, tooltip)               \
    R_INSTRUCTION_IMPLEMENTATION(Name##_R, #Name, Id, rd = rs Op rt, tooltip); \
    I_INSTRUCTION_IMPLEMENTATION(Name##_I, #Name, Id, rt = rs Op im, tooltip);

void send_osc(int port, const string &prefix, const string &address, int32_t a,
              int32_t b, int32_t c) {
    using namespace osc;

    UdpTransmitSocket socket(IpEndpointName(address.c_str(), port));

    array<char, 1024> buffer;
    OutboundPacketStream packet_stream(buffer.data(), buffer.size());
    packet_stream << BeginBundleImmediate << BeginMessage(prefix.c_str()) << a
                  << b << c << EndMessage << EndBundle;

    socket.Send(packet_stream.Data(), packet_stream.Size());
}

InstructionR NOP::assemble_specific(const vector<string> &str) const {
    if (str.size() != 0) {
        throw runtime_error("NOP does not take arguments");
    }

    InstructionR ret;
    ret.op = 0;
    ret.rs = 0;
    ret.rt = 0;
    ret.rd = 0;
    ret.shamt = 0;
    ret.funct = NOP::get_id_code();
    return ret;
}
string NOP::disassemble_specific(InstructionR instr) const {
    return "NOP";
}
string NOP::get_tooltip() const {
    return "do nothing!";
}
template <>
const string RInstructionDescriptor<0x00>::str = "NOP";

BINARY_INSTRUCTION_IMPLEMENTATION(ADD, 0x01, +, "ADD    a b c   -> a = b + c");
BINARY_INSTRUCTION_IMPLEMENTATION(SUB, 0x02, -, "SUB    a b c   -> a = b - c");
BINARY_INSTRUCTION_IMPLEMENTATION(MUL, 0x03, *, "MUL    a b c   -> a = b * c");
BINARY_INSTRUCTION_IMPLEMENTATION(DIV, 0x04, /, "DIV    a b c   -> a = b / c");
BINARY_INSTRUCTION_IMPLEMENTATION(BAND, 0x05, &, "BNAD   a b c   -> a = b & c");
BINARY_INSTRUCTION_IMPLEMENTATION(BOR, 0x06, |, "BOR    a b c   -> a = b | c");
BINARY_INSTRUCTION_IMPLEMENTATION(BXOR, 0x0A, ^, "BXOR   a b c   -> a = b ^ c");
BINARY_INSTRUCTION_IMPLEMENTATION(AND, 0x0B, &&,
                                  "AND    a b c   -> a = b && c");
BINARY_INSTRUCTION_IMPLEMENTATION(OR, 0x0C, ||, "OR     a b c   -> a = b || c");

R_INSTRUCTION_IMPLEMENTATION(OSC_R, "OSC", 0x0F,
                             send_osc(osc_out_port, osc_out_prefix,
                                      osc_out_address, rd, rs, rt),
                             "OSC    a b c   -> send osc");
I_INSTRUCTION_IMPLEMENTATION(OSC_I, "OSC", 0x0F,
                             send_osc(osc_out_port, osc_out_prefix,
                                      osc_out_address, rt, rs, im),
                             "OSC    a b c   -> send osc");
J_INSTRUCTION_IMPLEMENTATION(JUMP, "JUMP", 0x12, core.ip = address,
                             "JUMP   a       -> go to a");
I_INSTRUCTION_IMPLEMENTATION(JE, "JE", 0x13, if (rs == rt) core.ip = im,
                             "JE     a b c   -> if a == b go to c");
I_INSTRUCTION_IMPLEMENTATION(JNE, "JNE", 0x14, if (rs != rt) core.ip = im,
                             "JNE    a b c   -> if a != b go to c");
I_INSTRUCTION_IMPLEMENTATION(JL, "JL", 0x15, if (rs < rt) core.ip = im,
                             "JL     a b c   -> if a < b go to c");
I_INSTRUCTION_IMPLEMENTATION(JLE, "JLE", 0x16, if (rs <= rt) core.ip = im,
                             "JLE    a b c   -> if a <= b go to c");
I_INSTRUCTION_IMPLEMENTATION(JG, "JG", 0x17, if (rs > rt) core.ip = im,
                             "JG     a b c   -> if a > b go to c");
I_INSTRUCTION_IMPLEMENTATION(JGE, "JGE", 0x18, if (rs >= rt) core.ip = im,
                             "JGE    a b c   -> if a >= b go to c");

I_INSTRUCTION_IMPLEMENTATION(LOAD, "LOAD", 0x10, rt = memory[rs + im].raw,
                             "LOAD   a b c   -> a = memory[b + c]");
I_INSTRUCTION_IMPLEMENTATION(SAVE, "SAVE", 0x11, memory[rs + im].raw = rt,
                             "SAVE   a b c   -> memory[b + c] = a");

R_INSTRUCTION_IMPLEMENTATION(BNOT_R, "BNOT", 0x07, rd = ~rs,
                             "BNOT   a b     -> a = ~b");
I_INSTRUCTION_IMPLEMENTATION(BNOT_I, "BNOT", 0x07, rt = ~im,
                             "BNOT   a b     -> a = ~b");
R_INSTRUCTION_IMPLEMENTATION(NOT_R, "NOT", 0x0D, rd = !rs,
                             "NOT    a b     -> a = !b");
I_INSTRUCTION_IMPLEMENTATION(NOT_I, "NOT", 0x0D, rt = !im,
                             "NOT    a b     -> a = !b");
R_INSTRUCTION_IMPLEMENTATION(MOVE_R, "MOVE", 0x0E, rd = rs,
                             "MOVE   a b     -> a = b");
I_INSTRUCTION_IMPLEMENTATION(MOVE_I, "MOVE", 0x0E, rt = im,
                             "MOVE   a b     -> a = b");
R_INSTRUCTION_IMPLEMENTATION(RND, "RND", 0x19, rd = rand(),
                             "RND    a       -> a = rand()");

InstructionR BNOT_R::assemble_specific(const vector<string> &str) const {
    if (str.size() != 2) {
        throw runtime_error("BNOT takes two arguments");
    }
    InstructionR r;
    r.op = 0x0;
    r.funct = get_id_code();
    r.rd = parse_register(str[0]);
    r.rs = parse_register(str[1]);
    r.rt = 0;
    r.shamt = 0;
    return r;
}
InstructionI BNOT_I::assemble_specific(const vector<string> &str) const {
    if (str.size() != 2) {
        throw runtime_error("BNOT takes two arguments");
    }
    InstructionI r;
    r.op = get_id_code();
    r.rs = parse_register(str[0]);
    r.immediate = parse_register(str[1]);
    r.rt = 0;
    return r;
}
string BNOT_R::disassemble_specific(InstructionR instr) const {
    std::stringstream ss;
    ss << "R" << instr.rd << " R" << instr.rs;
    return ss.str();
}
string BNOT_I::disassemble_specific(InstructionI instr) const {
    std::stringstream ss;
    ss << "R" << instr.rs << " " << instr.immediate;
    return ss.str();
}

InstructionR NOT_R::assemble_specific(const vector<string> &str) const {
    if (str.size() != 2) {
        throw runtime_error("NOT takes two arguments");
    }
    InstructionR r;
    r.op = 0x0;
    r.funct = get_id_code();
    r.rd = parse_register(str[0]);
    r.rs = parse_register(str[1]);
    r.rt = 0;
    r.shamt = 0;
    return r;
}
InstructionI NOT_I::assemble_specific(const vector<string> &str) const {
    if (str.size() != 2) {
        throw runtime_error("NOT takes two arguments");
    }
    InstructionI r;
    r.op = get_id_code();
    r.rs = parse_register(str[0]);
    r.immediate = parse_register(str[1]);
    r.rt = 0;
    return r;
}
string NOT_R::disassemble_specific(InstructionR instr) const {
    std::stringstream ss;
    ss << "R" << instr.rd << " R" << instr.rs;
    return ss.str();
}
string NOT_I::disassemble_specific(InstructionI instr) const {
    std::stringstream ss;
    ss << "R" << instr.rs << " " << instr.immediate;
    return ss.str();
}

InstructionR MOVE_R::assemble_specific(const vector<string> &str) const {
    if (str.size() != 2) {
        throw runtime_error("MOVE takes two arguments");
    }
    InstructionR r;
    r.op = 0x0;
    r.funct = get_id_code();
    r.rd = parse_register(str[0]);
    r.rs = parse_register(str[1]);
    r.rt = 0;
    r.shamt = 0;
    return r;
}
InstructionI MOVE_I::assemble_specific(const vector<string> &str) const {
    if (str.size() != 2) {
        throw runtime_error("MOVE takes two arguments");
    }
    InstructionI r;
    r.op = get_id_code();
    r.rs = parse_register(str[0]);
    r.immediate = parse_register(str[1]);
    r.rt = 0;
    return r;
}
string MOVE_R::disassemble_specific(InstructionR instr) const {
    std::stringstream ss;
    ss << "R" << instr.rd << " R" << instr.rs;
    return ss.str();
}
string MOVE_I::disassemble_specific(InstructionI instr) const {
    std::stringstream ss;
    ss << "R" << instr.rs << " " << instr.immediate;
    return ss.str();
}

InstructionR RND::assemble_specific(const vector<string> &str) const {
    if (str.size() != 1) {
        throw runtime_error("RND takes one register as an argument");
    }

    InstructionR r;
    r.op = 0x0;
    r.funct = RND::get_id_code();
    r.rd = parse_register(str[0]);
    r.rs = 0;
    r.rt = 0;
    r.shamt = 0;
    return r;
}
string RND::disassemble_specific(InstructionR instr) const {
    std::stringstream ss;
    ss << "R" << instr.rd;
    return ss.str();
}

OSC_R::OSC_R(int osc_out_port, const std::string &osc_out_prefix,
             const std::string &osc_out_address)
    : osc_out_port(osc_out_port)
    , osc_out_prefix(osc_out_prefix)
    , osc_out_address(osc_out_address) {
}

OSC_I::OSC_I(int osc_out_port, const std::string &osc_out_prefix,
             const std::string &osc_out_address)
    : osc_out_port(osc_out_port)
    , osc_out_prefix(osc_out_prefix)
    , osc_out_address(osc_out_address) {
}
