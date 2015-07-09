#include "parse_functions.h"

#include <regex>
#include <cstdlib>

using namespace std;

uint32_t parse_register(const string & str) {
    //  TODO some registers could have aliases
    regex reg("R([1-9][0-9]+|[0-9])");
    smatch m;

    if (!regex_match(str, m, reg)) {
        throw runtime_error("no such register");
    }

    return atoi(m[1].str().c_str());
}

uint32_t parse_immediate(const string & str) {
    return strtol(str.c_str(), NULL, 0);
}

uint32_t parse_address(const string & str) {
    //  TODO some addresses come from the symbol table
    return strtol(str.c_str(), NULL, 0);
}
