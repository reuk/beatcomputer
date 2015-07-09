#pragma once

#include "instructions.h"

#include <cstdint>
#include <vector>
#include <array>

class Core {
public:
    Core();

    void print_state() const;

    std::array<uint32_t, 32> reg;
    uint32_t lo;
    uint32_t hi;

    uint32_t & sp;  //  stack pointer
    uint32_t ip;    //  instruction pointer
};
