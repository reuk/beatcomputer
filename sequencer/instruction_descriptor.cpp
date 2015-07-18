#include "instruction_descriptor.h"

using namespace std;

InstructionDescriptor::InstructionDescriptor(const std::string & str,
                                             uint32_t id)
    : str(str)
    , id(id) {
}

std::string InstructionDescriptor::get_string() const {
    return str;
}
uint32_t InstructionDescriptor::get_id_code() const {
    return id;
}

void InstructionDescriptor::set_string(const std::string & _str) {
    str = _str;
}
void InstructionDescriptor::set_id_code(uint32_t _id) {
    id = _id;
}
