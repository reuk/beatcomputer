#include "parse_functions.h"
#include "logger.h"

#include <regex>
#include <cstdlib>

using namespace std;

uint32_t parse_register(const string & str) {
    //  TODO some registers could have aliases

    Logger::log("parse register: ", str);

    regex reg("R([1-9][0-9]+|[0-9])");
    smatch m;

    if (!regex_match(str, m, reg)) {
        throw runtime_error("no such register");
    }

    return atoi(m[1].str().c_str());
}

uint32_t parse_immediate(const string & str) {
    try {
        return stol(str, 0, 0);
    } catch (...) {
        throw runtime_error("unable to parse immediate");
    }
}

uint32_t parse_address(const string & str) {
    //  TODO some addresses come from the symbol table
    try {
        return stol(str, 0, 0);
    } catch (...) {
        throw runtime_error("unable to parse immediate");
    }
}
