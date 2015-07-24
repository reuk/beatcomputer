#include "parse_functions.h"
#include "logger.h"

#include <regex>
#include <cstdlib>

using namespace std;

int32_t parse_number(const string & str) {
    regex reg("(?:\\-|\\+)?(?:(?:0x[0-9a-fA-F]*)|(?:[0-9]*))");
    smatch m;
    if (!regex_match(str, m, reg)) {
        throw runtime_error("no such immediate");
    }
    return stol(str, 0, 0);
}

uint32_t parse_register(const string & str) {
    Logger::log("parse register: ", str);

    regex reg("R([1-9][0-9]+|[0-9])");
    smatch m;

    if (!regex_match(str, m, reg)) {
        throw runtime_error("no such register");
    }

    auto MAX_REGISTERS = 32;
    auto ret = atoi(m[1].str().c_str());
    if (ret >= MAX_REGISTERS) {
        throw runtime_error("no such register");
    }
    return ret;
}

int32_t parse_immediate(const string & str) {
    try {
        return parse_number(str);
    } catch (const runtime_error & re) {
        throw;
    } catch (...) {
        throw runtime_error("unable to parse immediate");
    }
}

uint32_t parse_address(const string & str) {
    try {
        auto ret = parse_number(str);
        if (ret < 0)
            throw runtime_error("negative address not allowed");
        return ret;
    } catch (const runtime_error & re) {
        throw;
    } catch (...) {
        throw runtime_error("unable to parse immediate");
    }
}
