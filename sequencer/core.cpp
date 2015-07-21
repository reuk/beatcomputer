#include "core.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <bitset>

using namespace std;

Core::Core()
    : reg()
    , sp(reg[29])
    , ip(0) {
}

void Core::print_state() const {
    auto pr32 = [](auto name, auto i) {
        cout << setw(8) << name << ": " << setw(16) << i << " ("
             << bitset<32>(i) << ")" << endl;
    };

    for (auto i = 0; i != reg.size(); ++i) {
        stringstream ss;
        ss << "R" << i;
        pr32(ss.str(), reg[i]);
    }

    pr32("IP", ip);
}

bool Core::operator==(const Core & rhs) const {
    return !memcmp(this, &rhs, sizeof(Core));
}
