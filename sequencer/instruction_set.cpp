#include "instruction_set.h"

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

using namespace std;

#define R_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody)      \
    void Name::execute_specific(Core &core, vector<Instruction> &memory,  \
                                int32_t &rs, int32_t &rt, int32_t &rd,    \
                                uint32_t shamt) const {                   \
        functionbody;                                                     \
    }                                                                     \
    template <>                                                           \
    const std::string RInstructionDescriptor<Id>::str = String;

#define I_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody)     \
    void Name::execute_specific(Core &core, vector<Instruction> &memory, \
                                int32_t &rs, int32_t &rt, int32_t im)    \
        const {                                                          \
        functionbody;                                                    \
    }                                                                    \
    template <>                                                          \
    const std::string IInstructionDescriptor<Id>::str = String;

#define J_INSTRUCTION_IMPLEMENTATION(Name, String, Id, functionbody)     \
    void Name::execute_specific(Core &core, vector<Instruction> &memory, \
                                int32_t address) const {                 \
        functionbody;                                                    \
    }                                                                    \
    template <>                                                          \
    const std::string JInstructionDescriptor<Id>::str = String;

#define BINARY_INSTRUCTION_IMPLEMENTATION(Name, String, Id, Op)        \
    R_INSTRUCTION_IMPLEMENTATION(Name##_R, String, Id, rd = rs Op rt); \
    I_INSTRUCTION_IMPLEMENTATION(Name##_I, String, Id, rt = rs Op im);

void send_osc(int32_t a, int32_t b, int32_t c) {
    using namespace osc;

    auto address = "127.0.0.1";
    auto port = 7001;
    UdpTransmitSocket socket(IpEndpointName(address, port));

    array<char, 1024> buffer;
    OutboundPacketStream packet_stream(buffer.data(), buffer.size());
    packet_stream << BeginBundleImmediate << BeginMessage("/beatcomputer")
                  << a << b << c << EndMessage << EndBundle;

    socket.Send(packet_stream.Data(), packet_stream.Size());
}

R_INSTRUCTION_IMPLEMENTATION(NOP, "NOP", 0x00, );
BINARY_INSTRUCTION_IMPLEMENTATION(ADD, "ADD", 0x01, +);
BINARY_INSTRUCTION_IMPLEMENTATION(SUB, "SUB", 0x02, -);
BINARY_INSTRUCTION_IMPLEMENTATION(MUL, "MUL", 0x03, *);
BINARY_INSTRUCTION_IMPLEMENTATION(DIV, "DIV", 0x04, / );
BINARY_INSTRUCTION_IMPLEMENTATION(BAND, "BAND", 0x05, &);
BINARY_INSTRUCTION_IMPLEMENTATION(BOR, "BOR", 0x06, | );
BINARY_INSTRUCTION_IMPLEMENTATION(BXOR, "BXOR", 0x0A, ^);
BINARY_INSTRUCTION_IMPLEMENTATION(AND, "AND", 0x0B, &&);
BINARY_INSTRUCTION_IMPLEMENTATION(OR, "OR", 0x0C, || );
R_INSTRUCTION_IMPLEMENTATION(OSC_R, "OSC", 0x0F, send_osc(rd, rs, rt));
I_INSTRUCTION_IMPLEMENTATION(OSC_I, "OSC", 0x0F, send_osc(rt, rs, im));
J_INSTRUCTION_IMPLEMENTATION(JUMP, "JUMP", 0x12, core.ip = address);
I_INSTRUCTION_IMPLEMENTATION(JE, "JE", 0x13, if (rs == rt) core.ip = im);
I_INSTRUCTION_IMPLEMENTATION(JNE, "JNE", 0x14, if (rs != rt) core.ip = im);
I_INSTRUCTION_IMPLEMENTATION(JL, "JL", 0x15, if (rs < rt) core.ip = im);
I_INSTRUCTION_IMPLEMENTATION(JLE, "JLE", 0x16, if (rs <= rt) core.ip = im);
I_INSTRUCTION_IMPLEMENTATION(JG, "JG", 0x17, if (rs > rt) core.ip = im);
I_INSTRUCTION_IMPLEMENTATION(JGE, "JGE", 0x18, if (rs >= rt) core.ip = im);

R_INSTRUCTION_IMPLEMENTATION(BNOT_R, "BNOT", 0x07, );
I_INSTRUCTION_IMPLEMENTATION(BNOT_I, "BNOT", 0x07, );
R_INSTRUCTION_IMPLEMENTATION(NOT_R, "NOT", 0x0D, );
I_INSTRUCTION_IMPLEMENTATION(NOT_I, "NOT", 0x0D, );
R_INSTRUCTION_IMPLEMENTATION(MOVE_R, "MOVE", 0x0E, );
I_INSTRUCTION_IMPLEMENTATION(MOVE_I, "MOVE", 0x0E, );
I_INSTRUCTION_IMPLEMENTATION(LOAD, "LOAD", 0x10, rt = memory[rs + im].raw);
I_INSTRUCTION_IMPLEMENTATION(SAVE, "SAVE", 0x11, memory[rs + im].raw = rt);
R_INSTRUCTION_IMPLEMENTATION(RND, "RND", 0x18, rd = rand());
