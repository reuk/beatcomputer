#pragma once

#include <string>
#include <cstdint>

uint32_t parse_register(const std::string & str);
int32_t parse_immediate(const std::string & str);
uint32_t parse_address(const std::string & str);
