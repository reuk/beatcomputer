#include "assembler.h"
#include "core.h"
#include "trim.h"

//#include <ncurses.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char ** argv) {
    try {
        if (argc != 2) {
            throw runtime_error("expected one argument");
        }

        Assembler assembler;

        ifstream infile(argv[1]);

        auto index = 0;
        vector<Instruction> memory(256);

        for (string line; getline(infile, line);) {
            if (!trim(line).empty()) {
                auto instr = assembler.assemble(line);
                memory[index++] = instr;
            }
        }

        Core core;

        for (auto i = 0; i != 10; ++i) {
            core.execute_instruction(memory);
            core.print_state();

            cout << endl << endl;
        }
    } catch (runtime_error & re) {
        cout << re.what() << endl;
        return 1;
    }

    return 0;
}
