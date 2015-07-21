#pragma once

#include "instructions.h"

#include <cstdint>
#include <vector>
#include <array>

class Core {
public:
    Core();

    void print_state() const;

    std::array<int32_t, 32> reg;

    int32_t sp;  //  stack pointer
    int32_t ip;  //  instruction pointer

    bool operator==(const Core & rhs) const;
};
